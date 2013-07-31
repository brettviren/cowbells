#!/usr/bin/env python
'''
Generate geometry for the world.
'''

import cowbells
import base

from cowbells.geom.shapes import Box
from cowbells.geom.volumes import LogicalVolume

meter = cowbells.units.meter

class Builder(base.Builder):
    '''
    Build the top level world.

    The world is a simple cube.
    '''

    default_params = {
        'size' : 10*meter
        }
    default_parts = {
        'World': 'OpaqueAir'
        }

    def make_logical_volumes(self):

        parms,parts = self.pp()

        size = parms.size
        shape = Box(self.shapename('World'), x=size, y=size, z=size)
        lv = LogicalVolume(self.lvname('World'), 
                           matname = parts.World, shape=shape)
        return lv
                           
    def place(self):
        'No internal placements'
        return

    pass

