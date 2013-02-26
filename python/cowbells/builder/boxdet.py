#!/usr/bin/env python
'''
Generate geometry for a box detector.

This is otheriwse like tubdet but a smaller and rectangular.
::

       side
      +-----+        +Y
      ++---++         ^
      || _ ||         |
 face ||(_)|| face   width
      ||   ||         |
      ++---++         v
      +-----+        -Y
       side

 -X <--thickness--> +X
                    +Z
      +-+ +-+       ^
      +++_+++       |
      ||   ||       |
      ||   ||     height
      ||   ||       |
      ++---++       |
      +-----+       v
                    -Z
'''

import cowbells
import base
import world
from cowbells.geom import materials, surfaces, sensitive
from cowbells.geom.shapes import Boolean, Box, Tubs, Polycone
from cowbells.geom.volumes import LogicalVolume
from cowbells.geom.placements import PhysicalVolume

from cowbells.units import inch, meter, mm, hbarc

class Builder(base.Builder):
    default_params = {
        'inner_thickness': 2.0*inch,
        'inner_width':     6.0*inch,
        'inner_height':    6.0*inch,
        
        'lid_thickness':  0.75*inch,
        'base_thickness': 0.25*inch,
        'side_thickness': 0.25*inch,
        'face_thickness': 0.25*inch,

        # Fraction of lid thickness from bottom of window to step
        'step_fraction': 1.0/3.0,
        # Diameters of window before and after step
        'window_full_diameter': 2.25*inch,
        'window_step_diameter': 2.00*inch,
        # amount the window extends into the sample
        'window_extend': 1*mm,
        # Photocathode diameter and thickness
        'photocathode_diameter': 2.0*inch,
        'photocathode_thickness': 0.01*inch,

        # Color of the teflon coating (or material)
        'teflon_color' : 'black',

        # Provide an extra name in order to make unique if needed and
        # not otherwise achieved.
        'base_name' : "Box",
        }
        
    default_parts = {
        'Base': 'Aluminum',
        'Side': 'Aluminum',
        'Face': 'Aluminum',
        'Lid': 'Aluminum',
        'Window' : 'Acrylic',
        'PhotoCathode': 'Bialkali',
        'Sample': 'Water',
        }

    def basename(self):
        return self.params['base_name']

    def make_logical_volumes(self):
        '''
        The sample comprises the mother with lid, base and two faces
        and sides pushed together inside the mother to make a box.
        '''
        parms,parts = self.pp()

        thickness = parms.inner_thickness + 2*parms.face_thickness
        width =     parms.inner_width     + 2*parms.side_thickness
        height =    parms.inner_height    + parms.base_thickness + parms.lid_thickness

        window_small_radius = 0.5*(parms.window_step_diameter)
        window_large_radius = 0.5*(parms.window_full_diameter)
        window_step_z = parms.lid_thickness * parms.step_fraction

        shape = Box(self.shapename('Sample'), x=thickness/2.0, y=width/2.0, z=height/2.0)
        sample_lv = LogicalVolume(self.lvname('Sample'),
                                  matname = parts.Sample, shape = shape)

        x = thickness/2.0
        y = width/2.0
        z = parms.base_thickness/2.0
        shape = Box(self.shapename('Base'), x=x, y=y, z=z)
        base_lv = LogicalVolume(self.lvname('Base'), 
                                matname = parts.Base, shape = shape)

        # ----------------------------------- lid
        x = thickness/2.0
        y = width/2.0
        z = parms.lid_thickness/2.0
        lidm_shape = Box(self.shapename('Lid'), x=x, y=y, z=z)

        win_shape = Polycone(
            self.shapename('Window'), 
            zplane = [-parms.window_extend, window_step_z, window_step_z, parms.lid_thickness],
            rinner = [0.0]*4,
            router = [window_small_radius, window_small_radius, 
                      window_large_radius, window_large_radius])

        lid_shape = Boolean(self.shapename('LidWithHole'), 'subtraction', lidm_shape, win_shape, 
                            pos = [0,0, - (0.5*parms.lid_thickness + parms.window_extend)])
        lid_lv = LogicalVolume(self.lvname('Lid'), 
                               matname = parts.Lid, shape = lid_shape)

        win_lv = LogicalVolume(self.lvname('Window'), 
                               matname = parts.Window, shape = win_shape)

        shape = Tubs(self.shapename('PhotoCathode'), 
                     dz = 0.5*parms.photocathode_thickness,
                     rmax = 0.5*parms.photocathode_diameter)
        pc_lv = LogicalVolume(self.lvname('PhotoCathode'), 
                              matname = parts.PhotoCathode, shape = shape)
        # -----------------------------------


        x = parms.face_thickness / 2.0
        y = (width - 2*parms.side_thickness) / 2.0
        z = (height - parms.lid_thickness - parms.base_thickness) / 2.0
        shape = Box(self.shapename('Face'), x=x, y=y, z=z)
        face_lv = LogicalVolume(self.lvname('Face'), 
                                matname = parts.Face, shape = shape)
        
        x = thickness / 2.0
        y = parms.side_thickness / 2.0
        z = (height - parms.lid_thickness - parms.base_thickness) / 2.0
        shape = Box(self.shapename('Side'), x=x, y=y, z=z)
        side_lv = LogicalVolume(self.lvname('Side'), 
                                matname = parts.Side, shape = shape)

        return sample_lv

    def place(self):
        '''
        Do internal placements.
        '''
        p = self.pp()[0]

        total_thickness = p.inner_thickness + 2*p.face_thickness # x
        total_width = p.inner_width + 2*p.side_thickness         # y
        total_height = p.inner_height + p.lid_thickness + p.base_thickness

        lid_zoffset = +0.5*(total_height - p.lid_thickness)
        win_zoffset =  0.5*total_height - p.lid_thickness
        pc_zoffset = p.lid_thickness - 0.5*p.photocathode_thickness

        face_xoffset = 0.5*(total_thickness - p.face_thickness)
        side_yoffset = 0.5*(total_width     - p.side_thickness)
        face_side_zoffset = 0.5*(p.base_thickness - p.lid_thickness)

        lid_zoffset = +0.5*(total_height - p.lid_thickness)
        base_zoffset = -0.5*(total_height - p.base_thickness)

        PhysicalVolume(self.pvname('Window'),
                       self.lvname('Window'),self.lvname('Sample'),
                       pos=[0.0, 0.0, win_zoffset])

        PhysicalVolume(self.pvname('PhotoCathode'),
                       self.lvname('PhotoCathode'), self.lvname('Window'),
                       pos=[0.0, 0.0, pc_zoffset])

        PhysicalVolume(self.pvname('Lid'),
                       self.lvname('Lid'),self.lvname('Sample'),
                       pos=[0.0, 0.0, lid_zoffset])


        PhysicalVolume(self.pvname('Base'),
                       self.lvname('Base'),self.lvname('Sample'),
                       pos=[0.0, 0.0, base_zoffset])

        PhysicalVolume(self.pvname('RightSide'),
                       self.lvname('Side'),self.lvname('Sample'),
                       pos=[0.0, -side_yoffset, face_side_zoffset], copy=1)
        PhysicalVolume(self.pvname('LeftSide'),
                       self.lvname('Side'),self.lvname('Sample'),
                       pos=[0.0, +side_yoffset, face_side_zoffset], copy=2)

        PhysicalVolume(self.pvname('FrontFace'),
                       self.lvname('Face'),self.lvname('Sample'),
                       pos=[-face_xoffset, 0.0, face_side_zoffset], copy=1)
        PhysicalVolume(self.pvname('BackFace'),
                       self.lvname('Face'),self.lvname('Sample'),
                       pos=[+face_xoffset, 0.0, face_side_zoffset], copy=2)


        self._surface()
        return

    # fixme: this was largely cut-and-pasted from tubdet.py.  Needs refactoring
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

        for part in ['Base', 'Face', 'Side', 'Lid']:
            s = make_surf(self.surfname(part), first = self.lvname(part),
                          model="glisur", type='dielectric_metal', finish="polished")
            s.add_property("REFLECTIVITY",  x=energy, y=reflectivity)
            s.add_property("TRANSMITTANCE", x=energy, y=transmittance)
            continue

        print 'Config for "%s" box' % color
        return

    def sensitive(self):
        what = 'PhotoCathode'
        sdname = self.sensname(what)
        hcname = self.hitcolname(what)
        lvname = self.lvname(what)
        sd = sensitive.SensitiveDetector(sdname,hcname,lvname)
        print sdname, 'touchables:', sd.touchables()

class AbsorberBuilder(base.Builder):
    '''
    A simple rectangular box
    '''
    default_params = {
        'thickness': 10*mm,
        'width': 6.0*inch,
        'height': 6.0*inch,

        'base_name': 'Absorber',
        }

    default_parts = {
        'Absorber': 'Lead',
        }

    def basename(self):
        return self.params['base_name']

    def make_logical_volumes(self):
        parms,parts = self.pp()

        shape = Box(self.shapename('Absorber'), 
                    x=parms.thickness/2.0, y=parms.width/2.0, z=parms.height/2.0)
        lv = LogicalVolume(self.lvname('Absorber'),
                           matname = parts.Absorber, shape = shape)
        return lv

    pass

# fixme: this is also largely copied from tubdet.  Needs refactoring.
class World(base.Builder):
    '''
    Put a single boxdet in the world
    '''
    default_params = {
        'sample': 'Water',
        'box': 'Teflon',
        'layout': 'single',     # to be added

        # layout:'multi',
        'ndets': 3,               # number of detectors
        'absorber_material': 'Lead',
        'absorber_thickness': 10*mm,
        'period': 2*10*mm + 10*mm + 2*inch, # spacing between two detectors
        }

    def make_logical_volumes(self):
        p = self.pp()[0]

        if p.box.lower() == 'teflon':
            teflon_color = 'white'
        elif p.box.lower() == 'aluminum':
            teflon_color = 'black'
        else:
            raise ValueError, 'Unknown box material: "%s"' % p.box

        self.builders = [
            world.Builder( size = 1*meter),

            Builder( Lid=p.box, Face=p.box, Side=p.box, Base=p.box,
                     Sample = p.sample, teflon_color = teflon_color)
            ]

        if p.layout != 'single':
            self.builders.append(AbsorberBuilder(Absorber=p.absorber_material, 
                                                 thickness=p.absorber_thickness))

        self.lvs = [b.top() for b in self.builders]
        return self.lvs[0]

    def place(self):
        p = self.pp()[0]
        print 'Boxdet layout: "%s"' % p.layout
        meth = getattr(self,'place_%s' % p.layout)
        meth()
        for b in self.builders:
            b.place()
        return

    def place_single(self):
        print 'Single boxdet'
        world_lv = self.lvs[0]
        lv = self.lvs[1]
        name = lv.name.replace('lv','pv',1)
        PhysicalVolume(name, lv, world_lv)
        return

    def place_multi(self):
        p = self.pp()[0]
        print 'Multiple (%d) boxdets' % p.ndets
        world_lv = self.lvs[0]
        det_lv = self.lvs[1]
        abs_lv = self.lvs[2]

        for ndet in range(p.ndets):
            x_det = ndet*p.period
            if ndet:
                x_abs = x_det - 0.5*p.period
                name = abs_lv.name.replace('lv','pv',1)
                PhysicalVolume(name, abs_lv, world_lv, pos=[x_abs,0,0], copy=ndet)
            name = det_lv.name.replace('lv','pv',1)
            PhysicalVolume(name, det_lv, world_lv, pos=[x_det,0,0], copy=ndet+1)
                
        return

    def sensitive(self):
        for b in self.builders:
            b.sensitive()
        
            

if '__main__' == __name__:
    from cowbells import default, geom
    default.all()
    tdb = World()
    print geom.dumps_json()
