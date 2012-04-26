#!/usr/bin/env python
'''
Generate and dump out materials
'''

import cowbells
import water, wbls, acrylic

geo = cowbells.geo()

for mod in water, wbls, acrylic:
    mod.materials(geo)

if __name__ == '__main__':
    import sys
    import ROOT
    out = sys.argv[1]
    fp = ROOT.TFile.Open(out, "update")
    geo.Write("geometry")
    fp.Close()

    
