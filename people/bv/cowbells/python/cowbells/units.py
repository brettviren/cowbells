#!/usr/bin/env python
'''
Define a system of units (matches G4's)

import units
myinch = 2.54*units.cm
print 'My Inch is %f meters' % myinch/units.meter

'''

energy = gev = GeV = 1
eV = ev = GeV*1e-9


length = cm = centimeter = 1.0
m = meter = 100.0*cm
