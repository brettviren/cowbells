#!/usr/bin/env python 
'''
Ordered list of materials synced to what cowbells is prepped with 
'''

materials = [
    "Water", "WBLS", "Acrylic", "BlackAcrylic", "SiO2", "B2O3",
    "Na2O", "Al2O3", "Glass", "Vacuum", "Teflon",
]

def index(name):
    for ind,matname in enumerate(materials):
        if matname.lower() == name.lower(): return ind
    raise IndexError, 'no such material: "%s"' % name
