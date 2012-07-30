#!/usr/bin/env python
'''
Define vacuum
'''
name = 'Vacuum'

import util

def materials(geo):
    mix = util.make_mixture(geo, name, [], 0.0)
    util.make_medium(geo, mix)
    return
def properties(pf):
    return

