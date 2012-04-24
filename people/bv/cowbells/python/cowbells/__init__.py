#!/usr/bin/env python
'''
Main entry to the cowbells (COsmic WB(el)LS) simulation modules
'''

import boot
boot.everything()               # boot all the things!

import app
mcapp = app.app()

import ROOT
mc = ROOT.gMC
g4vmc = app._geant4

#import ROOT
#geo = ROOT.TGeoManager("geo","Geo Manager")
_geometry_name = "E06_geometry" # fixme: change to something relevant
geo = ROOT.gROOT.GetGeometry(_geometry_name)
