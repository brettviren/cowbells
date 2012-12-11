#!/usr/bin/env python
'''
Define optical properties for plastic scintillator by calling optical()
'''
import cowbells

eV = cowbells.units.eV
meter = cowbells.units.meter

energy = [1.5*eV, 6.2*eV]
rindex = [1.58]*len(energy)
abslength = [1*meter]*len(energy)


def optical(material = 'Scintillator'):
    from cowbells import geom
    geom.optical.MaterialProperty(material, 'RINDEX',    x=energy, y=rindex);
    geom.optical.MaterialProperty(material, 'ABSLENGTH', x=energy, y=abslength);

    return
