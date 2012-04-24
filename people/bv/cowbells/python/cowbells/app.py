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
    _app = ROOT.Ex06MCApplication('cowbells','COsmic WB(el)LS simulation')
    ROOT.SetOwnership(_app,0)

    run_config = ROOT.TG4RunConfiguration("geomRootToGeant4", "emStandard+optical")
    ROOT.SetOwnership(run_config,0)

    global _geant4
    _geant4 = ROOT.TGeant4("TGeant4", "The Geant4 Monte Carlo", run_config)
    ROOT.SetOwnership(_geant4,0)

    return _app

if __name__ == '__main__':
    import boot
    boot.everything()
    print app()

