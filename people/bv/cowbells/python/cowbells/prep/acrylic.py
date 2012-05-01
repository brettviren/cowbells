#!/usr/bin/env python
'''
Acrylic optical propteries
'''

name = 'Acrylic'

import cowbells
import util

mm = cowbells.units.mm
eV = cowbells.units.eV
meter = cowbells.units.meter
gram = cowbells.units.gram
cm3 = cowbells.units.cm3


parts = [
    ("Carbon",  0.59984),
    ("Hydrogen",0.08055),
    ("Oxygen",  0.31961),
    ]
density = 1.18*gram/cm3

# from Daya Bay
abslength = [x*mm for x in [8.0e3, 8.0e3,]]
abslength_energy = [x*eV for x in [1.55, 15.5]]

rayleigh = [x*meter for x in 
            [500.0,300.0,170.0,100.0,62.0,42.0,30.0,7.6,0.85,0.85,0.85,]]
rayleigh_energy = [x*eV for x in 
                   [1.55,1.7714,2.102,2.255,2.531,2.884,3.024,4.133,6.20,10.33,15.5,]]

rindex = [1.4878,1.4895,1.4925,1.4946,1.4986,1.5022,1.5065,1.5358,1.6279,1.6270,
          1.5359,1.5635,1.793,1.7199,1.6739,1.5635,1.462,1.462]
rindex_energy = [x*eV for x in [
        1.55, 1.79505, 2.10499, 2.27077, 2.55111, 2.84498, 3.06361,
        4.13281, 6.20, 6.526, 6.889, 7.294, 7.75, 8.267, 8.857,
        9.538, 10.33, 15.5 ]]


def materials(geo):
    'Make any materials'
    for modifier in ['','Black']:
        mat = util.make_mixture(modifier+name, parts, density)
        med = util.make_medium(mat)
        continue
    return

def properties(pf):
    '''
    Save data into a properties file
    '''
    pf.add(name, 'RINDEX',    zip(rindex_energy,rindex))
    pf.add(name, 'ABSLENGTH', zip(abslength_energy,abslength))
    pf.add(name, 'RAYLEIGH',  zip(rayleigh_energy,rayleigh))

    return
