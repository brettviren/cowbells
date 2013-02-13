#!/usr/bin/env python
'''
Define Teflon optical properties
'''
import cowbells

mm = cowbells.units.mm
hbarc = cowbells.units.hbarc

energy = [hbarc/800., hbarc/500., hbarc/250.]
rindex = [1.3]*3
abslength = [1*mm]*3


def optical(material = 'Teflon'):
    from cowbells import geom
    #geom.optical.MaterialProperty(material, 'RINDEX',    x=energy, y=rindex);
    #geom.optical.MaterialProperty(material, 'ABSLENGTH', x=energy, y=abslength);
    return
