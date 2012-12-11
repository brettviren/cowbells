#!/usr/bin/env python
'''
Test setting the detector construction class from python
'''

import cowbells
import ROOT


inter = cowbells.interface()

def test_dc():
    print 'Making and setting detector construction'
    dc = ROOT.Cowbells.TestDetectorConstruction()
    ROOT.SetOwnership(dc,0)
    inter.runMgr().SetUserInitialization(dc)
    print dc
    return

def test_pl():
    inter.activate_physics_list()
    return

def test_pg():
    print 'Making and setting primary generator'
    pg = ROOT.Cowbells.PrimaryGenerator()
    ROOT.SetOwnership(pg,0)
    inter.runMgr().SetUserAction(pg)
    print pg
    return

def test_init():
    inter.initialize()
    inter.dump_geometry()
    return

def test_run():
    inter.simulate()
    return

if __name__ == '__main__':
    test_dc()
    test_pl()
    test_pg()
    test_init()
    test_run()

    print '\n\nEXITING\n\n'
