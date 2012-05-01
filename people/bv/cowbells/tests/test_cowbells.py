#!/usr/bin/env python
'''
A test cowbells application.  Each test should be run in order in the
same job.
'''


import os
import cowbells

def test_geo():
    'Create geo'
    g = cowbells.geo()
    assert g, 'Got null geometry'
    assert not g.GetTopVolume(), 'Surprised by non-null top volume'
    assert os.path.exists("geo.root"), 'No geo.root - fixme: generate it in this test'
    g = cowbells.geo("geo.root")    
    assert g, 'Got null geometry from geo.root'
    assert g.GetTopVolume(), 'No top volume from geo.root'
    print 'Geo "%s" ok' % g.GetName()
    return

def test_make_app():
    'Make the CowMCapp'
    a = cowbells.app()
    assert a,'Got null app'
    assert os.path.exists("geo.root"), 'No geo.root - fixme: generate it in this test'
    a.SetPropertiesFile("geo.root")
    print 'App ok'
    return
    
def test_init_mc():
    app = cowbells.app()
    mc = cowbells.mc()
    assert app and mc,'Got null app/MC'
    app.InitMC(mc)
    mc.Init()
    mc.BuildPhysics()
    print 'MC init ok'
    return

def test_run_mc():
    mc = cowbells.mc()
    assert mc,'Got null MC'
    nevents = 10
    mc.ProcessRun(nevents)
    print 'MC run %d ok' % nevents
    return

if __name__ == '__main__':
    test_geo()
    test_make_app()
    test_init_mc()
    test_run_mc()
