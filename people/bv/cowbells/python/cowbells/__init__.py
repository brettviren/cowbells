#!/usr/bin/env python
'''
Main entry to the cowbells (COsmic WB(el)LS) simulation modules

'''

import ROOT

import boot
boot.everything()               # boot all the things!

units = ROOT.units


_geo = None
def geo():
    '''
    Get the geo manager.

    You can get this from ROOT.gROOT.GetGeometry("cowbells_geometry")
    but this function needs to be called before that (or the cowbells
    module imported somewhere).
    '''
    global _geo
    if _geo: return _geo
    _geo = ROOT.TGeoManager('cowbells_geometry', 
                           'Geometry for COsmic WB(el)LS detector')
    ROOT.SetOwnership(_geo,0)
    return _geo
_geo = geo()


def app(propertiesfile = 'cowbells_properties.root'):
    'Make a Cowbells MC application with an optional properties file'
    app = ROOT.CowMCapp('cowbells','COsmic WB(el)LS simulation')
    app.SetPropertiesFile(propertiesfile)
    ROOT.SetOwnership(app,0)
    return app

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




if __name__ == '__main__':
    print geo()
    print geo()
