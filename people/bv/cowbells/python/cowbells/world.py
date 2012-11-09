#!/usr/bin/env python
'''
Generate geometry for the world.
'''

import cowbells
from geom import materials
from geom.volumes import Shape, LogicalVolume
from geom.placements import PhysicalVolume

meter = cowbells.units.meter

class Builder(object):
    '''
    Build the top level world.

    The world is a simple cube.
    '''
    def __init__(self, world_size = 10*meter, material = 'Air'):
        assert materials.get(material), 'No material "%s" for world' % mat
        self.size = world_size
        self.material = material
        self._top = None
        return

    def top(self):
        if self._top: return self._top

        shape = Shape('world_shape', 'box', x=self.size, y=self.size, z=self.size)
        lv = LogicalVolume('lvWorld', matname = self.material, shape=shape)
        pv = PhysicalVolume('pvWorld', 'lvWorld')
        self._top = lv
        return self._top
    pass

