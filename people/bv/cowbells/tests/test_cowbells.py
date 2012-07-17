#!/usr/bin/env python
'''
A test cowbells application.  
'''


import ROOT
import cowbells
Cowbells = ROOT.Cowbells        # namespace

geofile = "geo.root"

def test_cowbells():

    rm = ROOT.G4RunManager()

    pl = ROOT.Cowbells.PhysicsList()
    ROOT.SetOwnership(pl,0)
    rm.SetUserInitialization(pl)

    pg = Cowbells.PrimaryGenerator()
    ROOT.SetOwnership(pg,0)
    rm.SetUserAction(pg)

    dc = Cowbells.BuildFromRoot(geofile)
    ROOT.SetOwnership(dc,0)
    rm.SetUserInitialization(dc)

    ura = ROOT.Cowbells.TestRunAction()
    ROOT.SetOwnership(ura,0)
    rm.SetUserAction(ura)

    usa = ROOT.Cowbells.TestStackingAction()
    ROOT.SetOwnership(usa,0)
    rm.SetUserAction(usa)

    rm.Initialize()

    UI = ROOT.G4UImanager.GetUIpointer()
    UI.ApplyCommand("/run/verbose 1");
    UI.ApplyCommand("/event/verbose 1");
    UI.ApplyCommand("/tracking/verbose 1");

    rm.BeamOn(3)

    del (rm)

if __name__ == '__main__':
    test_cowbells()
