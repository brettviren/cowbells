#!/usr/bin/env python
'''
Describe placements (physical volumes)
'''

import base, volumes

store = []

class PhysicalVolume(base.Base):
    def __init__(self, name, daughter, mother=None, rot=None, pos=None, copy=0):

        if isinstance(daughter, volumes.LogicalVolume):
            daughter = daughter.name
        assert volumes.get(daughter), 'No daughter volume "%s"' % daughter

        if mother:
            if isinstance(mother, volumes.LogicalVolume):
                mother = mother.name
            assert volumes.get(mother), 'No mother volume "%s"' % mother


        self.__dict__=dict(name=name, daughter=daughter, mother=mother, 
                           rot=rot, pos=pos, copy=copy)
        store.append(self)
        return
    pass


def get(pv):
    if isinstance(pv, PhysicalVolume):
        return pv
    for vol in store:
        if vol.name == pv:
            return vol
    return None

def placed(lv):
    '''
    Return all physical volumes placed into the mother lv.
    '''
    lv = volumes.get(lv)

    ret = []
    for pv in store:
        if pv.mother == lv.name:
            ret.append(pv)
    return [p.name for p in ret]

def walk(pv):
    pv = get(pv)
    
    pvs = placed(pv.daughter)
    yield [pv.name],pv.daughter
    for daughter_pv in pvs:
        for ps,l in walk(daughter_pv):
            yield [pv.name]+ps,l
            continue
        continue
    return

def pod(): return base.pod(store)

