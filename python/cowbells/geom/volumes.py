#!/usr/bin/env python
'''
Describe logical volumes.
'''

import base, materials, shapes
from cowbells import units

store = []

class LogicalVolume(base.Base):
    def __init__(self, name, matname, shape):
        if isinstance(matname, materials.Material):
            matname = matname.name
        if isinstance(shape, shapes.Shape):
            shape = shape.name
        assert materials.get(matname), 'No material: %s' % matname
        self.__dict__ = dict(name=name, matname=matname, shape=shape)
        store.append(self)
        return

    def __str__(self):
        return '<LogicalVolume "%s" mat:"%s" shape:"%s">' % \
            (self.name, self.matname, self.shape.name)
    pass

def get(lv):
    '''
    Return the LogicalVolume with a name matching the given one.

    If lv itself is a LogicalVolume its name will be used for the search.
    '''

    if isinstance(lv, LogicalVolume):
        return lv
    for vol in store:
        if vol.name == lv:
            return vol
        continue
    return None



def pod(): return base.pod(store)
