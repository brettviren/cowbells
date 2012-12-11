#!/usr/bin/env python
'''
A test cowbells application.  
'''


import ROOT
import cowbells
Cowbells = ROOT.Cowbells        # namespace

geofile = "geo.root"

def rootnew(name,*args):
    klass = eval('ROOT.' + name)
    obj = klass(*args)
    ROOT.SetOwnership(obj,0)
    return obj

def test_cowbells():

    rm = rootnew('G4RunManager')

    pl = rootnew('Cowbells.PhysicsList')
    rm.SetUserInitialization(pl)

    pg = rootnew('Cowbells.PrimaryGenerator')
    rm.SetUserAction(pg)

    do_test = False
    if do_test:
        detcon = rootnew('Cowbells.TestDetectorConstruction')
        detcon.add_sensdet("Bubble")
    else:
        detcon = rootnew('Cowbells.BuildFromRoot',geofile)
        geo = ROOT.TGeoManager.Import(geofile)
        top = geo.GetTopNode()
        touchables = cowbells.geo.touchable_paths(top, 'PC')
        detcon.add_sensdet("PC", touchables)
        geo.Delete()
        pass
    rm.SetUserInitialization(detcon)

    #dr = rootnew('Cowbells.DataRecorder',"test_cowbells.root")
    dr = None

    # user actions.  run before event

    #ura = rootnew('Cowbells.RunAction')
    #if dr: ura.set_recorder(dr)
    #rm.SetUserAction(ura)

    ea = rootnew('Cowbells.EventAction')
    if dr: ea.set_recorder(dr)
    rm.SetUserAction(ea)

    usa = rootnew('Cowbells.TestStackingAction')
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
