#!/usr/bin/env python
'''
Plastic scintillator.
'''
name = 'Scintillator'
import cowbells
import util

parts = [
    ('Carbon',9),
    ('Hydrogen',10),
]
density = 1.032

def materials(geo):
    mat = util.make_mixture(geo, name, parts, density)
    med = util.make_medium(geo, mat)
    return

def properties(pf):
    return


