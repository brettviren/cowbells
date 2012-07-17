#!/usr/bin/env python
'''
Test the Test* classes from N06.  

This tries to be the equivalent to exampleN06.cc
'''

import ROOT
import cowbells

Cowbells = ROOT.Cowbells        # namespace


def test_frompy():
    if False:
        tm = Cowbells.TestMain()
        tm.physics_list()
        rm = tm.run_manager()
    else:
        rm = ROOT.G4RunManager()
        #ROOT.SetOwnership(rm,0)
        pl = ROOT.Cowbells.TestPhysicsList()
        ROOT.SetOwnership(pl,0)
        rm.SetUserInitialization(pl)

    pg = Cowbells.TestPrimaryGeneratorAction()
    ROOT.SetOwnership(pg,0)
    rm.SetUserAction(pg)

    dc = Cowbells.TestDetectorConstruction()
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

    return

def test_fromcpp():
    tm = Cowbells.TestMain()
    tm.physics_list()
    rm = tm.run_manager()

    pg = tm.primary_generator_action()
    rm.SetUserAction(pg)

    dc = tm.detector_construction()
    rm.SetUserInitialization(dc)

    ura = tm.user_run_action()
    rm.SetUserAction(ura)

    usa = tm.user_stack_action()
    rm.SetUserAction(usa)

    rm.Initialize()

    UI = tm.ui_manager()
    UI.ApplyCommand("/run/verbose 1");
    UI.ApplyCommand("/event/verbose 1");
    UI.ApplyCommand("/tracking/verbose 1");

    rm.BeamOn(3)

    del (rm)

    return

def test_whole():
    Cowbells.test_main()
    return


if __name__ == '__main__':
    import sys
    which = sys.argv[1]
    func = eval ("test_%s" % which)
    func()

