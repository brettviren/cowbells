#!/usr/bin/env python
'''
A test TestCB interface
'''


import ROOT
import cowbells
Cowbells = ROOT.Cowbells        # namespace

geofile = "geo.root"

def test_cb():
    cb = Cowbells.TestCB()
    rm = cb.main(geofile)
    rm.BeamOn(3)
    return

if __name__ == '__main__':
    test_cb()
