#!/usr/bin/env python
'''
Define air
'''
name = 'Air'
import util

# as per the all knowing wikipedia
# http://en.wikipedia.org/wiki/Atmosphere_of_Earth
parts = [
    ('Nitrogen',0.781),
    ('Oxygen',  0.210),
    ('Argon',   0.009)
    ]
density = 1.2e-3

def materials(geo):
    mat = util.make_mixture(geo, name, parts, density)
    med = util.make_medium(geo, mat)
    return

def properties(pf):
    return
