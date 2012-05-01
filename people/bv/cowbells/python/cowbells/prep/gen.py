#!/usr/bin/env python
'''
Generate the properties file
'''

import cowbells
import geometry, properties

geo = cowbells.geo()

def fill(filename):
    import water, wbls, acrylic, glass
    for mod in water, wbls, acrylic, glass:
        mod.materials(geo)

    cbgb = geometry.CowbellGeometryBuilder()
    top = cbgb.top(geo)
    geo.SetTopVolume(top)

    print 'Writing geometry'
    import ROOT
    fp = ROOT.TFile.Open(filename, "update")
    geo.Write("geometry")       # fixme, this probably collides once geometry.py is written....
    fp.Close()

    properties.fill(filename)

    return

if __name__ == '__main__':
    import sys
    fill(sys.argv[1])
