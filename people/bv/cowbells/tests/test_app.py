#!/usr/bin/env python

import ROOT
import cowbells
import cowbells.app
from array import array

def sample_geom():
    'Make some trivial geometry'
    geo = cowbells.geo()

    mednum = 1

    mix = ROOT.TGeoMixture("Water",2,1.0)
    ROOT.SetOwnership(mix,0)
    for elename, elecount in [('Hydrogen',2),('Oxygen',1)]:
        ele = geo.GetElementTable().FindElement(elename)
        mix.AddElement(ele,elecount)
        continue
    
    med = ROOT.TGeoMedium('Water',mednum,mix)
    ROOT.SetOwnership(med,0)
    worldboxsize = array('f',[10.0]*3)
    world = geo.Volume("World","BOX",mednum,worldboxsize,3)
    geo.SetTopVolume(world)
    print 'Set top volume for "%s"' % geo.GetName()

    return

def test_app():

    print 'Makeing geometry'
    sample_geom()

    print 'Making app'
    app = cowbells.app.app()

    print 'Initialize MC, triggers C++ detector construction'
    ROOT.gMC.Init()

    print 'Build MC physics'
    ROOT.gMC.BuildPhysics()

    print 'Running 10'
    ROOT.gMC.ProcessRun(10)

    print 'And, I am out'
    return

if __name__ == '__main__':
    test_app()

