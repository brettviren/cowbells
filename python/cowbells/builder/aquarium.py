#!/usr/bin/env python
'''
The aquarium detector is an acrylic cylinder of ~1kL.
'''

import cowbells
import base
#import world
#from cowbells.geom import materials, surfaces, sensitive
from cowbells.geom.shapes import Tubs
from cowbells.geom.volumes import LogicalVolume
from cowbells.geom.placements import PhysicalVolume

from cowbells.units import inch, mm

class Builder(base.Builder):
    '''
    Build an aquarium
    '''

    # From "1000 liter prototype 20130702 Detector Concept document from David
    default_params = {
        'env_thickness': 100*mm,
        'cyl_diameter': 44.7*inch,
        'cyl_height': 45.0*inch,
        'wall_thickness': 0.75*inch, # or 0.375, 0.500, 0.750, 1.000
        'lid_thickness': 25*mm,      # or 9, 12, 18 or 25 mm
        }
    default_parts = {
        'Envelope': 'Air',
        'Aquarium': 'Acrylic',
        'Sample': 'Water',
        }
    

    def make_logical_volumes(self):
        parms,parts = self.pp()
        
        # symmetric cylinder onion
        outer_radius = 0.5*parms.cyl_diameter
        inner_radius = outer_radius - parms.wall_thickness
        inner_hheight =  0.5*parms.cyl_height
        outer_hheight = inner_hheight + parms.lid_thickness
        env_radius = outer_radius + parms.env_thickness
        env_hheight = outer_hheight + parms.lid_thickness + parms.env_thickness
        
        shape = Tubs(self.shapename('Envelope'), 
                     dz = env_hheight, rmax = env_radius)
        lv = LogicalVolume(self.lvname('Envelope'),
                           matname = parts.Envelope, shape = shape)

        shape = Tubs(self.shapename('Aquarium'), 
                     dz = outer_hheight, rmax = outer_radius)
        LogicalVolume(self.lvname('Aquarium'),
                      matname = parts.Aquarium, shape = shape)

        shape = Tubs(self.shapename('Sample'), 
                     dz = inner_hheight, rmax = inner_radius)
        LogicalVolume(self.lvname('Sample'),
                      matname = parts.Sample, shape = shape)

        return lv

    def place(self):
        'Do internal placements.'

        p = self.pp()[0]

        PhysicalVolume(self.pvname('Aquarium'),
                       self.lvname('Aquarium'),self.lvname('Envelope'))
        PhysicalVolume(self.pvname('Sample'),
                       self.lvname('Sample'),self.lvname('Aquarium'))

        #self._surface()
        return
