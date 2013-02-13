#!/usr/bin/env python
'''
Rotations and Positions.

Used by both volumes (Boolean Shape) and placements.
'''
from cowbells import units
def pod(rot, pos):
    ret = {}
    if rot:            # dictionary
        newrot = {}
        for k,v in self.rot.items():
            newrot[k] = '%f *radian' % (v/units.radian)
        ret['rot'] = newrot
    if pos:
        ret['pos'] = ['%f * mm' % (l/units.mm) for l in pos]
    return ret
