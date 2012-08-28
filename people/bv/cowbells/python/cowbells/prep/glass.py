#!/usr/bin/env python
'''
Glass material and properties
'''
name = 'Glass'

import cowbells
import util
import water

cm = cowbells.units.cm

energy = water.energy
rindex = [1.6]*len(energy)
abslength = [1e9*cm]*len(energy)

def materials(geo):
    util.make_mixture(geo, 'SiO2', [("Silicon",1),('Oxygen',2)],  2.20)
    util.make_mixture(geo, 'B2O3', [("Boron",2),('Oxygen',3)],    2.46)
    util.make_mixture(geo, 'Na2O', [("Sodium",2),('Oxygen',1)],   2.27)
    util.make_mixture(geo, 'Al2O3',[("Aluminium",2),('Oxygen',3)], 4.00)

    mat = util.make_mixture(geo, 'Glass',[('SiO2',0.806),
                                          ('B2O3',0.130),
                                          ('Na2O',0.040),
                                          ('Al2O3',0.024)], 2.23)
    util.make_medium(geo, mat)
    return

def properties(pf):
    '''
    Save data into a properties file
    '''
    pf.add(name, 'RINDEX',    zip(energy,rindex))
    pf.add(name, 'ABSLENGTH', zip(energy,abslength))

    return

