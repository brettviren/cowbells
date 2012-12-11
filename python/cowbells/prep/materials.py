#!/usr/bin/env python
'''
Generate and dump out materials
'''

import cowbells
from cowbells.prep import propmods


geo = cowbells.geo()

def fill(filename):
    for mod in propmods:
        mod.materials(geo)

    import ROOT
    fp = ROOT.TFile.Open(filename, "update")
    geo.Write("geometry")       # fixme, this probably collides once geometry.py is written....
    fp.Close()

if __name__ == '__main__':
    import sys
    fill(sys.argv[1])
    
