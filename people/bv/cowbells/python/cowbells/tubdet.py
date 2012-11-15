#!/usr/bin/env python
'''
Generate geometry for a tub detector.
'''

import cowbells
from geom import materials, surfaces, sensitive
from geom.volumes import Tubs, Polycone, LogicalVolume
from geom.placements import PhysicalVolume

hbarc = cowbells.units.clhep_units.hbarc
inch = cowbells.units.inch
meter = cowbells.units.meter
mm = cowbells.units.mm

class Struct:
    def __init__(self, **entries): 
        self.update(entries)
    def update(self, d):
        self.__dict__.update(d)

class Builder(object):
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
        }

    # map part to material
    default_parts = {
        'TubBottom' : 'Teflon',
        'TubSide' : 'Teflon',
        'TubLid' : 'Teflon',
        'TubWindow' : 'Acrylic',
        'PhotoCathode': 'Acrylic',
        'Sample': 'WBLS01',
        }

    def __init__(self, **params):
        self._top = None
        self.params = dict(Builder.default_params)
        used = set()
        for k,v in params.items():
            if k in self.params.keys():
                self.params[k] = v
                used.add(k)
            
        self.parts = dict(Builder.default_parts)
        for k,v in params.items():
            if k in self.parts.keys():
                self.parts[k] = v
                used.add(k)

        unused = set(params.keys()).difference(used)
        if unused:
            print 'Warning: unused arguments: %s' % unused

        # check all needed materials are defined
        for part,mat in self.parts.items():
            assert materials.get(mat), 'No material "%s" for part "%s"' % (mat, part)
            continue

        return

    def top(self):
        '''
        Return top logical volume.
        '''
        if self._top: return self._top
        
        _top = self.make_logical_volumes()
        return _top

    def pvname(self,part):
        assert part in self.parts.keys(), 'Unkown part: %s'%part
        return 'pv'+self.parts[part]+part

    def lvname(self,part):
        assert part in self.parts.keys(), 'Unkown part: %s'%part
        return 'lv'+self.parts[part]+part

    def shapename(self, part):
        assert part in self.parts.keys(), 'Unkown part: %s'%part
        return self.parts[part].lower()+'_'+part.lower()+'_shape'

    def surfname(self, part):
        assert part in self.parts.keys(), 'Unkown part: %s'%part
        return self.parts[part].lower()+'_'+part.lower()+'_surface'

    def make_logical_volumes(self):

        parms = Struct(**self.params) # just for abbreviation
        parts = Struct(**self.parts)  # just for abbreviation

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

        shape = Tubs(self.shapename('TubBottom'),
                     dz = 0.5*parms.tub_thickness, rmax = outer_radius)
        bottom_lv = LogicalVolume(self.lvname('TubBottom'), 
                                  matname = parts.TubBottom, shape = shape)

        shape = Tubs(self.shapename('TubSide'), 
                     dz = inner_hheight, rmin = inner_radius, rmax = outer_radius)
        side_lv = LogicalVolume(self.lvname('TubSide'), 
                                matname = parts.TubSide, shape = shape)

        shape = Polycone(
            self.shapename('TubLid'),
            zplane = [0.0,window_step_z,
                      window_step_z,parms.lid_thickness],
            rinner = [window_small_radius, window_small_radius, 
                      window_large_radius, window_large_radius],
            router = [outer_radius]*4)
        lid_lv = LogicalVolume(self.lvname('TubLid'), 
                               matname = parts.TubLid, shape = shape)

        shape = Polycone(
            self.shapename('TubWindow'), 
            zplane = [-parms.window_extend,window_step_z,
                       window_step_z,parms.lid_thickness],
            rinner = [0.0]*4,
            router = [window_small_radius, window_small_radius, 
                      window_large_radius, window_large_radius])
        win_lv = LogicalVolume(self.lvname('TubWindow'), 
                               matname = parts.TubWindow, shape = shape)

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
        p = Struct(**self.params) # just for abbreviation

        tot_height = p.lid_thickness + p.inner_height + p.tub_thickness

        lid_offset = +0.5*tot_height - p.lid_thickness # polycone builds up from 0.
        side_offset =-0.5*tot_height + p.tub_thickness + 0.5*p.inner_height
        bot_offset = -0.5*tot_height + 0.5*p.tub_thickness
        win_offset = lid_offset
        pc_offset = p.lid_thickness - 0.5*p.photocathode_thickness


        PhysicalVolume(self.pvname('TubWindow'),
                       self.lvname('TubWindow'),self.lvname('Sample'),
                       pos=[0.0, 0.0, win_offset])

        PhysicalVolume(self.pvname('PhotoCathode'),
                       self.lvname('PhotoCathode'), self.lvname('TubWindow'),
                       pos=[0.0, 0.0, pc_offset])

        PhysicalVolume(self.pvname('TubLid'),
                       self.lvname('TubLid'),self.lvname('Sample'),
                       pos=[0.0, 0.0, lid_offset])


        PhysicalVolume(self.pvname('TubBottom'),
                       self.lvname('TubBottom'),self.lvname('Sample'),
                       pos=[0.0, 0.0, bot_offset])

        PhysicalVolume(self.pvname('TubSide'),
                       self.lvname('TubSide'),self.lvname('Sample'),
                       pos=[0.0, 0.0, side_offset])



        self._surface()
        self._sensors()
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

        for part in ['TubBottom','TubSide','TubLid']:
            s = make_surf(self.surfname(part), first = self.lvname(part),
                          model="glisur", type='dielectric_metal', finish="polished")
            s.add_property("REFLECTIVITY",  x=energy, y=reflectivity)
            s.add_property("TRANSMITTANCE", x=energy, y=transmittance)
            continue

        return

    def _sensors(self):
        sd = sensitive.SensitiveDetector('SensitiveDetector', 'HC', 
                                         self.lvname('PhotoCathode'))
        print sd.touchables()


    pass

if '__main__' == __name__:
    tdb = Builder()
    print tdb.top().pod()
