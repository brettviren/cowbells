#!/usr/bin/env python
'''
Describe the photocathode material properties
'''

import cowbells

eV = cowbells.units.eV
mm = cowbells.units.mm
nm = cowbells.units.nm
meter = cowbells.units.meter

# from dayabay
efficiency = [
    0.00010,    0.00200,    0.00500,    0.01000,    0.01700,    0.03000,    0.04000,
    0.05000,    0.06000,    0.07000,    0.08000,    0.09000,    0.10000,    0.11000,
    0.13000,    0.15000,    0.16000,    0.18000,    0.19000,    0.20000,    0.21000,
    0.22000,    0.22000,    0.23000,    0.24000,    0.24000,    0.24000,    0.23000,
    0.22000,    0.21000,    0.17000,    0.14000,    0.09000,    0.03500,    0.00500,
    0.00100,    0.00010,
    ]

eff_energy = [x*eV for x in [
    1.55000,    1.80000,    1.90000,    2.00000,    2.05000,    2.16000,    2.19000,
    2.23000,    2.27000,    2.32000,    2.36000,    2.41000,    2.46000,    2.50000,
    2.56000,    2.61000,    2.67000,    2.72000,    2.79000,    2.85000,    2.92000,
    2.99000,    3.06000,    3.14000,    3.22000,    3.31000,    3.40000,    3.49000,
    3.59000,    3.70000,    3.81000,    3.94000,    4.07000,    4.10000,    4.40000,
    5.00000,    6.20000,
]]
    
full_eff = [1.0, 1.0]
full_eff_en = [1.55*eV, 6.2*eV]


abslength = [0.1*nm]*4
rindex = [2.9]*4
kindex = [1.5]*4
index_energy = [x*eV for x in [1.55,6.20,10.33,15.5]]
    

def _optical(material = 'Bialkali'):
    from cowbells import geom
    geom.optical.MaterialProperty(material, 'RINDEX',    x=index_energy, y=rindex);
    geom.optical.MaterialProperty(material, 'KINDEX',    x=index_energy, y=kindex);
    geom.optical.MaterialProperty(material, 'QE', x=eff_energy, y=efficiency);
    #geom.optical.MaterialProperty(material, 'QE', x=full_eff_en, y=full_eff);
    geom.optical.MaterialProperty(material, 'ABSLENGTH', x=abslength, y=eff_energy);
    return
def optical(material = None):
    material = material or ['Bialkali', 'TCBialkali']
    if isinstance(material, str):
        material = [material]
    for mat in material:
        _optical(mat)
