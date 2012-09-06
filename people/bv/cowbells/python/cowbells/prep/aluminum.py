#!/usr/bin/env python
'''
Define aluminum
'''
name = 'Aluminum'
import util

parts = [
    ('Aluminium',   1.0)        # gotta spell it like the freaks across the pond do
    ]                           # typos in element names leads to VGM giving cryptic error:
                                # No elements defined.*** Error: Aborting execution  *** 

density = 2.7                   # g/cc, implicit

def materials(geo):
    mat = util.make_mixture(geo, name, parts, density)
    med = util.make_medium(geo, mat)
    return

def properties(pf):
    return
