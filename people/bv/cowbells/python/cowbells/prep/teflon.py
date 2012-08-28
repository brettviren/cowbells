#!/usr/bin/env python
'''
Define Teflon
'''
name = 'Teflon'

import cowbells
import util



cm = cowbells.units.cm
mm = cowbells.units.mm

hbarc = cowbells.units.clhep_units.hbarc

def materials(geo):
    mat = util.make_mixture(geo, 'Teflon', [('Carbon',0.759814),
                                            ('Fluorine',0.240186)], 2.2)
    util.make_medium(geo, mat)
    return


# IEEE TRANSACTIONS ON NUCLEAR SCIENCE, VOL. 59, NO. 3, JUNE 2012
# Reflectivity Spectra for Commonly Used Reflectors
# Martin Janecek
# Figure 10.
# 8-layers reflectivity: 90% @ 250nm, 96% @ 500nm, 87% @ 800 nm.

energy = [hbarc/800., hbarc/500., hbarc/250.]
rindex = [1.3]*3
abslength = [1*mm]*3
#reflectivity = [.87, .96, .90]


def properties(pf):
    # bulk
    pf.add(name, 'RINDEX',    zip(energy,rindex))
    pf.add(name, 'ABSLENGTH', zip(energy,abslength))

    
    #pf.add(name, 'REFLECTIVITY', zip(energy,reflectivity))
    #pf.add(name, 'EFFICIENCY', zip(energy,reflectivity))
