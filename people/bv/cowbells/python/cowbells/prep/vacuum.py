#!/usr/bin/env python
'''
Define vacuum
'''
name = 'Vacuum'

import util

def materials(geo):
    mix = util.make_mixture(name, [], 0.0)
    util.make_medium(mix)
    return
def properties(pf):
    return

