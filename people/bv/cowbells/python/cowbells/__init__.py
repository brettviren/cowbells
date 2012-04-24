#!/usr/bin/env python
'''
Main entry to the cowbells (COsmic WB(el)LS) simulation modules
'''

import ROOT

import boot
boot.everything()               # boot all the things!


_geo = None
def geo():
    'Get the geo manager'
    global _geo
    if _geo: return _geo
    _geo = ROOT.TGeoManager('cowbells_geometry', 
                           'Geometry for COsmic WB(el)LS detector')
    ROOT.SetOwnership(_geo,0)
    return _geo

def do_not_call():
    import app
    mcapp = app.app()

    import ROOT
    mc = ROOT.gMC
    g4vmc = app._geant4

   #import ROOT
   #geo = ROOT.TGeoManager("geo","Geo Manager")
   # _geometry_name = "E06_geometry" # fixme: change to something relevant
   #geo = ROOT.gROOT.GetGeometry(_geometry_name)
    geo = ROOT.TGeoManager(mcapp.GetName() + '_geometry', 
                           'Geometry for ' + mcapp.GetTitle())
    ROOT.SetOwnership(geo,0)
    return




