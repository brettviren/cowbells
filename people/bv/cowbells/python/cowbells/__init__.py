#!/usr/bin/env python
'''
Main entry to the cowbells (COsmic WB(el)LS) simulation modules

'''

import PyCintex
import ROOT

import boot
boot.everything()               # boot all the things!

PyCintex.loadDict("cowbellsDict")

units = PyCintex.Namespace("units").CLHEP

def interface():
    return ROOT.Cowbells.Interface()


_geo = None
def geo(filename = None):
    '''
    Get the geo manager.

    If a filename is given the geometry is imported from the file,
    potentially overriding any prior geometry.

    Otherwise, if a prior geometry exists it is returned or a new,
    empty one is created.

    '''
    global _geo

    if filename:
        _geo = ROOT.TGeoManager.Import(filename)

    if _geo: 
        return _geo

    _geo = ROOT.TGeoManager('cowbells_geometry', 
                            'Geometry for COsmic WB(el)LS detector')
    ROOT.SetOwnership(_geo,0)
    return _geo

_app = None
def app():
    'Make a Cowbells MC application'
    global _app
    if _app: return _app
    _app = ROOT.CowMCapp('cowbells','COsmic WB(el)LS simulation')
    ROOT.SetOwnership(_app,0)
    return _app


_geant4 = None
def mc():
    global _geant4
    if _geant4: return _geant4

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

    geom_style = "geomRootToGeant4"
    #geom_style = "geomRoot"
    print 'Setting runconfig to geometry style "%s"' % geom_style
    run_config = ROOT.TG4RunConfiguration(geom_style, "emStandard+optical")
    ROOT.SetOwnership(run_config,0)

    print 'Making the TGeant4'
    _geant4 = ROOT.TGeant4("TGeant4", "The Geant4 Monte Carlo", run_config)
    ROOT.SetOwnership(_geant4,0)

    return _geant4


if __name__ == '__main__':
    print geo()
    print geo()
