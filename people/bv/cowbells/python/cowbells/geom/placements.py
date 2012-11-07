#!/usr/bin/env python
'''
Describe placements (physical volumes)
'''

import base, volumes

store = []

class PhysicalVolume(base.Base):
    def __init__(self, name, lvmother, lvdaughter, rot=None, pos=None, copy=0):
        for lv in [lvmother,lvdaughter]:
            assert volumes.get(lv), 'No logical volume "%s"' % lv 

        if isinstance(lvmother, LogicalVolume):
            lvmother = lvmother.name
        if isinstance(lvdaughter, LogicalVolume):
            lvdaughter = lvdaughter.name

        self.__dict__=dict(name=name, lvmother=lvmother, lvdaughter=lvdaughter,
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

def pod(): return base.pod(store)

