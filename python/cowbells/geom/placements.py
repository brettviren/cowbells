#!/usr/bin/env python
'''
Describe placements (physical volumes)
'''

import base, volumes, rotpos
from cowbells import units

store = []

class PhysicalVolume(base.Base):
    '''
    Placement of a daughter volume.

    Mother should be specified unless daughter is to be the world volume.

    Rotations are specified by a dictionary with keys named after
    G4RotationMatrix's methods (eg "rotatez")

    Positions are specified with a tripple of floats.
    '''
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
    def pod(self):
        p = super(PhysicalVolume,self).pod()
        p.update(rotpos.pod(self.rot, self.pos))
        return p
    pass


def get(pv, copy=None):
    '''
    Return a list of PhysicalVolumes with name matching the given one
    and, optionally copy number.

    If pv may be a PhysicalVolume in which case its name will be used.
    '''

    pvname = pv
    if isinstance(pv, PhysicalVolume):
        pvname = pv.name

    ret = []
    for vol in store:
        if vol.name == pvname:
            if copy is None or copy == vol.copy:
                ret.append(vol)
    return ret

def placed(lv):
    '''
    Return all PhysicalVolumes objects placed into the mother lv.
    '''
    lv = volumes.get(lv)

    ret = []
    for pv in store:
        if pv.mother == lv.name:
            ret.append(pv)
    return ret

def walk(top):
    '''
    Iterate through the physical volume hierarchy.  

    Returns a sequence of [pvs],lv where [pvs] is a list of parent
    PhysicalVolume objects and lv is a daughter LogicalVolume object.

    Elements are PhysicalVolume objects
    '''

    for pv in get(top):
        pvs = placed(pv.daughter)
        yield [pv],volumes.get(pv.daughter)
        for daughter_pv in pvs:
            for ps,l in walk(daughter_pv):
                yield [pv]+ps,volumes.get(l)
                continue
            continue
    return

def pod(): return base.pod(store)

