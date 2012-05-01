#!/usr/bin/env python
'''
Generate and dump out materials
'''

import cowbells
import water, wbls, acrylic

geo = cowbells.geo()

def fill(filename):
    for mod in water, wbls, acrylic:
        mod.materials(geo)

    import ROOT
    fp = ROOT.TFile.Open(filename, "update")
    geo.Write("geometry")       # fixme, this probably collides once geometry.py is written....
    fp.Close()

if __name__ == '__main__':
    import sys
    fill(sys.argv[1])
    
