#!/usr/bin/env python
'''
Handle units in spite of VGM.
'''

import PyCintex
import ROOT

clhep_units = PyCintex.Namespace("units").CLHEP


## LENGTH

# vgm assumes lengths values are in cm, clhep in mm
vgm_length = 10.0

mm = millimeter = clhep_units.millimeter/vgm_length
cm = centimeter = mm*10.0
meter = cm*100.0
nm = 1.0e-9*meter
angstrom = nm/10.0
inch = 2.54*cm
parsec = clhep_units.parsec/vgm_length


## Time

ns = clhep_units.ns


## Energy

eV = clhep_units.eV
MeV = clhep_units.MeV
GeV = clhep_units.GeV


## Mass

gram = clhep_units.gram


## Angle

radian = clhep_units.radian
degree = clhep_units.degree
