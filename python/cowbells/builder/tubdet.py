#!/usr/bin/env python
'''
Generate geometry for a tub detector.
'''

import cowbells
import base
import world
from cowbells.geom import materials, surfaces, sensitive
from cowbells.geom.volumes import Tubs, Polycone, LogicalVolume
from cowbells.geom.placements import PhysicalVolume

hbarc = cowbells.units.clhep_units.hbarc
from cowbells.units import inch, meter, mm


class Builder(base.Builder):
    '''
    Build a "tub" detector geometry.

    This detector is a can with a given side and bottom thickness and
    a separate lid thickness.  The lid is pierced with a stepped
    window that contains a thin photocathode wafer and extends
    slightly into the volume of the inside of the can.  This volume is
    filled with the sample material.

    The geometry is contructed by placing the can material into a
    cylinder of the sample.  This makes the sample the top level
    mother.
    '''

    # numbers taken from Harry's Rev02 drawing sent to the mailing
    # list
    # https://lists.bnl.gov/mailman/private/wbls-l/attachments/20120524/a0af1b4f/attachment-0003.bin
    default_params = {
        # Inner diameter of sample volume
        'inner_diameter': (6.0-2*0.25)*inch,
        # Inner height of sample volume
        'inner_height': 6.0*inch,
        # Thickness of wall and bottom
        'tub_thickness': 0.25*inch,
        # Thickness of lid
        'lid_thickness': 0.75*inch,
        # Fraction of lid thickness from bottom of window to step
        'step_fraction': 1.0/3.0,
        # Diameters of window before and after step
        'window_full_diameter': 2.750*inch,
        'window_step_diameter': 2.125*inch,
        # amount the window extends into the sample
        'window_extend': 1*mm,
        # Photocathode diameter and thickness
        'photocathode_diameter': 2.0*inch,
        'photocathode_thickness': 0.01*inch,

        # Color of the teflon coating (or material)
        'teflon_color' : 'white',

        # Provide an extra name in order to make unique if needed and
        # not otherwise achieved.
        'base_name' : "Tub",
        }

    # map part to material
    default_parts = {
        'Bottom' : 'Teflon',
        'Side' : 'Teflon',
        'Lid' : 'Teflon',
        'Window' : 'Acrylic',
        'PhotoCathode': 'Bialkali',
        'Sample': 'WBLS01',
        }

    def basename(self):
        return self.params['base_name']

    def make_logical_volumes(self):

        parms,parts = self.pp()

        inner_radius  = 0.5*(parms.inner_diameter)
        outer_radius  = 0.5*(parms.inner_diameter+2.0*parms.tub_thickness)
        inner_hheight = 0.5*(parms.inner_height)
        outer_hheight = 0.5*(parms.inner_height+parms.tub_thickness+parms.lid_thickness)
        window_small_radius = 0.5*(parms.window_step_diameter)
        window_large_radius = 0.5*(parms.window_full_diameter)
        window_step_z = parms.lid_thickness * parms.step_fraction

        shape = Tubs(self.shapename('Sample'), 
                     dz = outer_hheight, rmax = outer_radius)
        sample_lv = LogicalVolume(self.lvname('Sample'),
                                  matname = parts.Sample, shape = shape)

        shape = Tubs(self.shapename('Bottom'),
                     dz = 0.5*parms.tub_thickness, rmax = outer_radius)
        bottom_lv = LogicalVolume(self.lvname('Bottom'), 
                                  matname = parts.Bottom, shape = shape)

        shape = Tubs(self.shapename('Side'), 
                     dz = inner_hheight, rmin = inner_radius, rmax = outer_radius)
        side_lv = LogicalVolume(self.lvname('Side'), 
                                matname = parts.Side, shape = shape)

        shape = Polycone(
            self.shapename('Lid'),
            zplane = [0.0,window_step_z,
                      window_step_z,parms.lid_thickness],
            rinner = [window_small_radius, window_small_radius, 
                      window_large_radius, window_large_radius],
            router = [outer_radius]*4)
        lid_lv = LogicalVolume(self.lvname('Lid'), 
                               matname = parts.Lid, shape = shape)

        shape = Polycone(
            self.shapename('Window'), 
            zplane = [-parms.window_extend,window_step_z,
                       window_step_z,parms.lid_thickness],
            rinner = [0.0]*4,
            router = [window_small_radius, window_small_radius, 
                      window_large_radius, window_large_radius])
        win_lv = LogicalVolume(self.lvname('Window'), 
                               matname = parts.Window, shape = shape)

        shape = Tubs(self.shapename('PhotoCathode'), 
                     dz = 0.5*parms.photocathode_thickness,
                     rmax = 0.5*parms.photocathode_diameter)
        pc_lv = LogicalVolume(self.lvname('PhotoCathode'), 
                              matname = parts.PhotoCathode, shape = shape)
        
        return sample_lv

    def place(self):
        '''
        Do internal placements.
        '''
        p = self.pp()[0]
        tot_height = p.lid_thickness + p.inner_height + p.tub_thickness

        lid_offset = +0.5*tot_height - p.lid_thickness # polycone builds up from 0.
        side_offset =-0.5*tot_height + p.tub_thickness + 0.5*p.inner_height
        bot_offset = -0.5*tot_height + 0.5*p.tub_thickness
        win_offset = lid_offset
        pc_offset = p.lid_thickness - 0.5*p.photocathode_thickness


        PhysicalVolume(self.pvname('Window'),
                       self.lvname('Window'),self.lvname('Sample'),
                       pos=[0.0, 0.0, win_offset])

        PhysicalVolume(self.pvname('PhotoCathode'),
                       self.lvname('PhotoCathode'), self.lvname('Window'),
                       pos=[0.0, 0.0, pc_offset])

        PhysicalVolume(self.pvname('Lid'),
                       self.lvname('Lid'),self.lvname('Sample'),
                       pos=[0.0, 0.0, lid_offset])


        PhysicalVolume(self.pvname('Bottom'),
                       self.lvname('Bottom'),self.lvname('Sample'),
                       pos=[0.0, 0.0, bot_offset])

        PhysicalVolume(self.pvname('Side'),
                       self.lvname('Side'),self.lvname('Sample'),
                       pos=[0.0, 0.0, side_offset])



        self._surface()
        return

    def _surface(self):
        '''
        Make Teflon liner surface.
        '''
        make_surf = surfaces.OpticalSurface # abrev

        color = self.params['teflon_color'].lower()
        if color not in ['white','black']:
            raise ValueError, 'Unknown Teflon color: "%s"' % color

        data = None
        if color == 'white':
            data = [(250,0.90),
                    (400,0.96),
                    (500,0.95),
                    (600,0.94),
                    (800,0.87)]
        if color == 'black':
            data = [(250,0.02),
                    (400,0.02),
                    (500,0.02),
                    (600,0.02),
                    (800,0.02)]

        data.reverse()
        reflectivity, transmittance, energy = list(),list(),list()
        for nm, ref in data:
            reflectivity.append(ref)
            transmittance.append(1-ref)
            energy.append(hbarc/nm)
            continue

        for part in ['Bottom','Side','Lid']:
            s = make_surf(self.surfname(part), first = self.lvname(part),
                          model="glisur", type='dielectric_metal', finish="polished")
            s.add_property("REFLECTIVITY",  x=energy, y=reflectivity)
            s.add_property("TRANSMITTANCE", x=energy, y=transmittance)
            continue

        print 'Config for "%s" tub' % color
        return

    def sensitive(self):
        what = 'PhotoCathode'
        sdname = self.sensname(what)
        hcname = self.hitcolname(what)
        lvname = self.lvname(what)
        sd = sensitive.SensitiveDetector(sdname,hcname,lvname)
        print sdname, 'touchables:', sd.touchables()


    pass

class World(base.Builder):
    '''
    Put a single tubdet in the world
    '''
    default_params = {
        'sample': 'Water',
        'tub': 'Teflon',
        }

    def make_logical_volumes(self):
        p = self.pp()[0]

        if p.tub.lower() == 'teflon':
            teflon_color = 'white'
        elif p.tub.lower() == 'aluminum':
            teflon_color = 'black'
        else:
            raise ValueError, 'Unknown tub material: "%s"' % p.tub

        self.builders = [
            world.Builder( size = 1*meter),
            Builder( Bottom = p.tub, Side = p.tub, Lid = p.tub, Sample = p.sample,
                     teflon_color = teflon_color)
            ]

        self.lvs = [b.top() for b in self.builders]
        return self.lvs[0]

    def place(self):

        world_lv = self.lvs[0]
        for lv in self.lvs[1:]:
            name = lv.name.replace('lv','pv',1)
            PhysicalVolume(name, lv, world_lv)
            continue

        for b in self.builders:
            b.place()

    def sensitive(self):
        for b in self.builders:
            b.sensitive()
        
            

if '__main__' == __name__:
    tdb = World()
    print tdb.top().pod()
