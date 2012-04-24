#!/usr/bin/env python

import ROOT


for lib in ["RIO","Geom", "VMC", "Physics"]:
    lib = 'lib'+lib
    print 'Loading "%s"'%lib
    ok = ROOT.gSystem.Load(lib)
    if not ok:                  # wtf?
        print 'Failed first loading of "%s", trying one more time' % lib
        ok = ROOT.gSystem.Load(lib)
    assert ok, 'Failed to load "%s"' % lib
    print 'Loaded "%s"' % lib

geo = ROOT.TGeoManager("geo","Geo Manager")
mc = ROOT.gMC

assert mc, 'Failed to get ROOT.gMC'
mc_name = mc.GetName()          # catch some errors
