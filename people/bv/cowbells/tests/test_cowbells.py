#!/usr/bin/env python
'''
A test cowbells application.  
'''


import ROOT
import cowbells
Cowbells = ROOT.Cowbells        # namespace

root_file = "geo.root"
gdml_file = "geo.gdml"

geofile = gdml_file
#geofile = root_file

def test_cowbells():

    rm = ROOT.G4RunManager()

    pl = ROOT.Cowbells.PhysicsList()
    ROOT.SetOwnership(pl,0)
    rm.SetUserInitialization(pl)

    pg = Cowbells.PrimaryGenerator()
    ROOT.SetOwnership(pg,0)
    rm.SetUserAction(pg)

    do_test = False
    if do_test:
        detcon = Cowbells.TestDetectorConstruction()
        detcon.add_sensdet("Bubble")
    else:
        if 'root' in geofile:
            detcon = Cowbells.BuildFromRoot(geofile)
        elif 'gdml' in geofile:                   
            detcon = Cowbells.BuildFromGdml(geofile, root_file)
        else:
            raise RuntimeError, 'Unknown geometry file: "%s"' % geofile
        detcon.add_sensdet("PC")
        pass

    ROOT.SetOwnership(detcon,0)
    rm.SetUserInitialization(detcon)


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
