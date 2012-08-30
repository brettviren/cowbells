#!/usr/bin/env python
'''
Define aluminum
'''
name = 'Aluminum'
import util

parts = [
    ('Aluminum',   1.0)
    ]
density = 2.7                   # g/cc, implicit

def materials(geo):
    mat = util.make_mixture(geo, name, parts, density)
    med = util.make_medium(geo, mat)
    return

def properties(pf):
    return
