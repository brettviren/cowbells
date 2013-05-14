#!/usr/bin/env python
'''
The "magic box" detector.

This is a rectangular cell holding a liquid sample.  Its side cross
section is approximately square.  Acrylic windows at the bottom of the
two sides perpendicular to the beam allow PMTs to view the sample, and
each other.  The PMTs axis are colinear and parallel with but below
the nominal beam.  

The detector is defined with the Z-axis parallel to the beam and the
Y-axis pointing upward.  The origin is at the center of the volume
that defines the outer dimensions of the box.

The cell walls are "black" with small but specular reflection.  There
is a thin-walled hemispherical glass dome "bulb" protruding through
the lid into the sample (test tube holding LED).

         ^
 <-- Z   |   . X
         Y
     +------+
  ---|------|---- beam
     |      |
     +      +
     |      |     windows/pmts
     +      +
     +------+

Dimension labels are as if you are looking that the side view.

X-Z plane: bases (top/bottom) are inset in side and face walls
Y-Z plane: sides run top-to-bottom inside face walls
X-Y plane: faces fully span 

'''
import cowbells
import base
from cowbells.geom import materials, surfaces, sensitive
from cowbells.geom.shapes import Boolean, Box, Tubs, Polycone
from cowbells.geom.volumes import LogicalVolume
from cowbells.geom.placements import PhysicalVolume

from cowbells.units import inch, meter, mm, hbarc, degree

class Builder(base.Builder):
    default_params = {
        
        # interior box
        'inner_dx':  63.50*mm,
        'inner_dy': 126.78*mm,
        'inner_dz': 130.74*mm,
        
        # slab thicknesses
        'base_thickness': 0.50*inch,
        'side_thickness': 0.50*inch,
        'face_thickness': 0.125*inch,

        # The test tube.  Offset is how much it protrudes beyond the
        # plane of the lid.  >0 is downward.
        'bulb_radius':  0.25*inch,
        'bulb_offset': 1.0*mm,

        'window_diameter' : 57.15*mm,
        'photocathode_diameter': 2.0*inch,
        'photocathode_thickness': 0.01*inch,

        # distance from the inner floor to the PMT axis
        'pmt_offset': (92.28 - 34.50 - 0.5*53.0) * mm,

        # Reflectively of the walls
        'reflectivity' : 0.02,

        # Provide an extra name in order to make unique if needed and
        # not otherwise achieved.
        'base_name' : "Magic",
        }
        
    default_parts = {
        'Box': 'ABS',
        'Face': 'ABS',
        'Side': 'ABS',
        'Base': 'ABS',
        'Window' : 'Acrylic',
        'PhotoCathode': 'Bialkali',
        'Sample': 'Water',
        }

    def basename(self):
        return self.params['base_name']

    def make_logical_volumes(self):
        '''
        The sample provides the mother volume.  Walls, windows and
        bulb are embedded in the sample in order to provide correct
        optical surfaces without gaps.
        '''
        parms,parts = self.pp()
        
        # Full, outside dimensions
        full_x_dim = parms.inner_dx + 2*parms.side_thickness
        full_y_dim = parms.inner_dy + 2*parms.base_thickness
        full_z_dim = parms.inner_dz + 2*parms.face_thickness

        # Sample
        shape = Box(self.shapename('Sample'), 
                    x=full_x_dim/2.0, y=full_y_dim/2.0, z=full_z_dim/2.0)
        sample_lv = LogicalVolume(self.lvname('Sample'),
                                  matname = parts.Sample, shape = shape)

        # Window
        win_shape = Tubs(self.shapename('Window'),
                         rmax=parms.window_diameter/2.0, 
                         dz=parms.face_thickness/2.0)
        LogicalVolume(self.lvname('Window'),
                      matname = parts.Window, shape = win_shape)

        # photo-cathode 
        shape = Tubs(self.shapename('PhotoCathode'), 
                     dz = 0.5*parms.photocathode_thickness,
                     rmax = 0.5*parms.photocathode_diameter)
        LogicalVolume(self.lvname('PhotoCathode'), 
                      matname = parts.PhotoCathode, shape = shape)

        
        # Base (top/bottom)
        shape = Box(self.shapename('Base'), 
                    x=parms.inner_dx/2.0, 
                    y=parms.base_thickness/2.0,
                    z=parms.inner_dz/2.0)
        LogicalVolume(self.lvname('Base'), 
                      matname = parts.Box, shape = shape)

        # Sides
        shape = Box(self.shapename('Side'),
                    x=parms.side_thickness/2.0, 
                    y=full_y_dim/2.0, 
                    z=parms.inner_dz/2.0)
        LogicalVolume(self.lvname('Side'),
                      matname = parts.Box, shape = shape)

        # Faces (front/back) with window hole
        win_co_shape = Tubs(self.shapename('Window'),
                            rmax=parms.window_diameter/2.0, 
                            dz=parms.face_thickness) # make extra thick

        face_shape = Box(self.shapename('Face'),
                         x=full_x_dim/2.0,
                         y=full_y_dim/2.0, 
                         z=parms.face_thickness/2.0)

        self.win_offset = -full_y_dim/2.0 + parms.base_thickness + parms.pmt_offset
        fwh_shape = Boolean(self.shapename('FaceWithHole'), 'subtraction', 
                            face_shape, win_co_shape, 
                            pos = [0, self.win_offset, 0.0])
        LogicalVolume(self.lvname('Face'),
                      matname = parts.Box, shape = fwh_shape)

        return sample_lv

    def place(self):
        '''
        Do internal placements.
        '''
        p = self.pp()[0]

        # PC just at top of window (window on its side facing up)
        PhysicalVolume(self.pvname('PhotoCathode'),
                       self.lvname('PhotoCathode'), self.lvname('Window'),
                       pos = [0.0, 0.0, 0.5*(p.face_thickness - p.photocathode_thickness)])
        
        # Place windows
        PhysicalVolume(self.pvname('WindowA'),
                       self.lvname('Window'), self.lvname('Sample'),
                       pos = [0.0, self.win_offset, 0.5*(p.inner_dz + p.face_thickness)])
        PhysicalVolume(self.pvname('WindowB'),
                       self.lvname('Window'), self.lvname('Sample'),
                       pos = [0.0, self.win_offset, -0.5*(p.inner_dz + p.face_thickness)],
                       rot = {'rotatex':180*degree})

        # Place base (top/bottom)
        PhysicalVolume(self.pvname('Bottom'),
                       self.lvname('Base'), self.lvname('Sample'),
                       pos = [0.0, -0.5*(p.inner_dy + p.base_thickness), 0.0])
        PhysicalVolume(self.pvname('Top'),
                       self.lvname('Base'), self.lvname('Sample'),
                       pos = [0.0, 0.5*(p.inner_dy + p.base_thickness), 0.0])
        
        # Place sides
        PhysicalVolume(self.pvname('SideP'),
                       self.lvname('Side'), self.lvname('Sample'),
                       pos = [0.5*(p.inner_dx + p.side_thickness), 0, 0])
        PhysicalVolume(self.pvname('SideM'),
                       self.lvname('Side'), self.lvname('Sample'),
                       pos = [-0.5*(p.inner_dx + p.side_thickness), 0, 0])
                                   
        # Place faces
        PhysicalVolume(self.pvname('FaceP'),
                       self.lvname('Face'), self.lvname('Sample'),
                       pos = [0, 0, 0.5*(p.inner_dy + p.face_thickness)])
        PhysicalVolume(self.pvname('FaceM'),
                       self.lvname('Face'), self.lvname('Sample'),
                       pos = [0, 0, -0.5*(p.inner_dy + p.face_thickness)])

        self._surface()

    def _surface(self):
        '''
        Make ABS optical surface.
        '''
        p = self.pp()[0]

        make_surf = surfaces.OpticalSurface # abrev

        energies =  [250,400,500,600,800]
        reflectivity = len(energies)*[p.reflectivity]
        data = zip(energies, reflectivity)
        data.reverse()
        reflectivity, transmittance, energy = list(),list(),list()
        for nm, ref in data:
            reflectivity.append(ref)
            transmittance.append(1-ref)
            energy.append(hbarc/nm)
            continue

        for part in ['Base','Side','Face']:
            s = make_surf(self.surfname(part), first = self.lvname(part),
                          model="glisur", type='dielectric_metal', finish="polished")
            s.add_property("REFLECTIVITY",  x=energy, y=reflectivity)
            s.add_property("TRANSMITTANCE", x=energy, y=transmittance)
            continue

        return

    def sensitive(self):
        what = 'PhotoCathode'
        sdname = self.sensname(what)
        hcname = self.hitcolname(what)
        lvname = self.lvname(what)
        sd = sensitive.SensitiveDetector(sdname,hcname,lvname)
        print sdname, 'touchables:', sd.touchables()
        
