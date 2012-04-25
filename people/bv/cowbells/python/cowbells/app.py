#!/usr/bin/env python
'''
Access the MC application
'''

import ROOT

_app = None
_geant4 = None
def app():
    'Return the MC application'
    global _app
    if _app: return _app

    print 'Making Cowbells MC App'
    _app = ROOT.CowMCapp('cowbells','COsmic WB(el)LS simulation')
    ROOT.SetOwnership(_app,0)

    # http://root.cern.ch/root/vmc/Geant4VMC.html
    # 
    # options for first argument
    # - geomVMCtoGeant4 - geometry defined via VMC, G4 native navigation
    # - geomVMCtoRoot - geometry defined via VMC, Root navigation
    # - geomRoot - geometry defined via Root, Root navigation
    # - geomRootToGeant4 - geometry defined via Root, G4 native navigation
    # - geomGeant4 - geometry defined via Geant4, G4 native navigation
    #
    # options for second argument
    # - emStandard - standard em physics (default)
    # - emStandard+optical - standard em physics + optical physics
    # - XYZ - selected hadron physics list ( XYZ = LHEP, QGSP, ...)
    # - XYZ+optical - selected hadron physics list + optical physics

    #geom_style = "geomRootToGeant4"
    geom_style = "geomRoot"
    print 'Setting runconfig to geometry style "%s"' % geom_style
    run_config = ROOT.TG4RunConfiguration(geom_style, "emStandard+optical")
    ROOT.SetOwnership(run_config,0)

    print 'Making the TGeant4'
    global _geant4
    _geant4 = ROOT.TGeant4("TGeant4", "The Geant4 Monte Carlo", run_config)
    ROOT.SetOwnership(_geant4,0)

    return _app
