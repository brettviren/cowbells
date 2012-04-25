#!/usr/bin/env python

import ROOT
import sample

def test_app():

    print 'Making geometry'
    top = sample.topvol()

    print 'Making app'
    app = sample.app()

    app.Start()

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

