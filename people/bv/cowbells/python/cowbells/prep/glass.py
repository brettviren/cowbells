#!/usr/bin/env python
'''
Glass material and properties
'''
name = 'Glass'

import cowbells
import util
import water

gram = cowbells.units.gram
cm = cowbells.units.cm
cm3 = cowbells.units.cm3

energy = water.energy
rindex = [1.6]*len(energy)
abslength = [1e9*cm]*len(energy)

def materials(geo):
    util.make_mixture('SiO2', [("Silicon",1),('Oxygen',2)],  2.20*gram/cm3)
    util.make_mixture('B2O3', [("Boron",2),('Oxygen',3)],    2.46*gram/cm3)
    util.make_mixture('Na2O', [("Sodium",2),('Oxygen',1)],   2.27*gram/cm3)
    util.make_mixture('Al2O3',[("Aluminium",2),('Oxygen',3)], 4.00*gram/cm3)

    mat = util.make_mixture('Glass',[('SiO2',0.806),
                                     ('B2O3',0.130),
                                     ('Na2O',0.040),
                                     ('Al2O3',0.024)], 2.23*gram/cm3)
    util.make_medium(mat)
    return

def properties(pf):
    '''
    Save data into a properties file
    '''
    pf.add(name, 'RINDEX',    zip(energy,rindex))
    pf.add(name, 'ABSLENGTH', zip(energy,abslength))

    return

