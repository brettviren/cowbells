#!/usr/bin/env python
'''
Plastic scintillator.
'''
name = 'Scintillator'
import cowbells
import util

meter = cowbells.units.meter
eV = cowbells.units.eV

parts = [
    ('Carbon',9),
    ('Hydrogen',10),
]
density = 1.032



energy = [1.5*eV, 6.2*eV]
rindex = [1.58]*len(energy)
abslength = [1*meter]*len(energy)

def materials(geo):
    mat = util.make_mixture(geo, name, parts, density)
    med = util.make_medium(geo, mat)
    return

def properties(pf):
    # FIXME: need scintilator properties here!
    pf.add(name, 'RINDEX',    zip(energy,rindex))
    pf.add(name, 'ABSLENGTH', zip(energy,abslength))

    return


