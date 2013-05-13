#!/usr/bin/env python
'''
The "magic box" detector.

This detector is rationally symmetric about the Z-axis.  It has a
rectangular sample cell viewed by one upstream and one downstream PMT
that run parallel to the beam (and X-axis).  The PMTs view the sample
through acrylic windows.  The cell walls are "black" with small but
specular reflection.  There is a thin-walled hemispherical glass dome
protruding through the lid into the sample (test tube holding LED).

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
