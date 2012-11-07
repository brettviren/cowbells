#!/usr/bin/env python
'''
Describe shapes and logical volumes.
'''

import base, materials

### SHAPES ###

# currently this is shema-less data to be interpreted by the consumer
class Shape(object):
    def __init__(self, name, type, **kwds):
        self.__dict__ = dict(name=name, type=type)
        self.__dict__.update(kwds)
        return
    pass


### LOGICAL VOLUMES ###

store = []

class LogicalVolume(base.Base):
    def __init__(self, name, matname, shape):
        if isinstance(matname, materials.Material):
            matname = matname.name
        assert materials.get(matname), 'No material: %s' % matname
        self.__dict__ = dict(name=name, matname=matname, shape=shape)
        store.append(self)
        return

    def pod(self):
        '''
        Return self as a plain old data structure.
        '''
        me = dict(self.__dict__)
        me['shape'] = self.shape.__dict__
        return me

    pass

def get(name):
    if isinstance(name, LogicalVolume):
        name = name.name
    for lv in store:
        if lv.name == name:
            return lv
        continue
    return None



def pod(): return base.pod(store)
