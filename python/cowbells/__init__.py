#!/usr/bin/env python
'''
Main entry to the cowbells (COsmic WB(el)LS) simulation modules

'''

import boot
boot.everything()               # boot all the things!

import geo
import units
for key in units.clhep_units.__dict__.keys():
    units.__dict__[key] = eval("units.clhep_units.%s" % (key,))
units.inch = 2.54*units.cm
