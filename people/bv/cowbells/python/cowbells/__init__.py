#!/usr/bin/env python
'''
Main entry to the cowbells (COsmic WB(el)LS) simulation modules
'''

import ROOT
import boot, app
boot.all()

geo = ROOT.TGeoManager("geo","Geo Manager")
print geo

mcapp = app.mcapp()

mc = ROOT.gMC
