#!/usr/bin/env python
'''
Test making some water
'''
import ROOT
from cowbells.material import water
import sample

def test_water():
    'Make some water medium'

    print 'Making geometry'
    top = sample.topvol()

    print 'Making app'
    app = sample.app()

    water.register(ROOT.gMC)

    print 'Initialize MC, triggers C++ detector construction'
    ROOT.gMC.Init()

    print 'Build MC physics'
    ROOT.gMC.BuildPhysics()

    print 'Running 10'
    ROOT.gMC.ProcessRun(10)

    print 'And, I am out'
    return

if __name__ == '__main__':
    test_water()
