#!/usr/bin/env python

import math
import cowbells
units = cowbells.units

def test_angles():
    '''Make sure this bug is not with us:
    http://bugzilla-geant4.kek.jp/show_bug.cgi?id=1307
    '''
    chk = abs(180 - math.pi*units.radian/units.degree) < 0.001
    msg = 'Failed to get expected angular conversion: radian = %f, degree = %f' %(units.radian, units.degree)
    assert chk, msg

def test_spew():
    'Print some unit conversion'
    print '1 parsec = %e angstrom' % (1.0*units.parsec/units.angstrom)
    print type(units.parsec)
    print units.angstrom

def test_density():
    'Print some densities'
    water = units.gram/units.cm3
    print 'Density of water is %f g/cm3, gram=%f, cm3=%f' % (water, units.gram, units.cm3)

if __name__ == '__main__':
    test_angles()
    test_spew()
    test_density()
