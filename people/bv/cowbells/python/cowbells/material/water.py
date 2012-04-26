#!/usr/bin/env python

from array import array
import cowbells
import util


eV = cowbells.units.eV
cm = cowbells.units.cm
gram = cowbells.units.gram

parts = [
    ('Hydrogen',2),
    ('Oxygen',1),
    ]
density = 1.0*gram/cm

def medium():
    'Return the water medium'
    mat = util.make_mixture("Water", parts, density)
    if not mat: return None
    return util.make_medium(mat)

