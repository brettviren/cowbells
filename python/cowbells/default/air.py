#!/usr/bin/env python
'''
Define optical properties for air
'''
import cowbells

eV = cowbells.units.eV
mm = cowbells.units.mm
meter = cowbells.units.meter
km = 1000*meter

# from Daya Bay

abslength = [10*km, 10*km]  # not correct, just some big numbers
rindex = [1.000277, 1.000277]   # :)
energy = [1.55, 15.5 ]


def optical(material = 'Air'):
    from cowbells import geom
    geom.optical.MaterialProperty(material, 'RINDEX',    x=energy, y=rindex);
    geom.optical.MaterialProperty(material, 'ABSLENGTH', x=energy, y=abslength);
    return
