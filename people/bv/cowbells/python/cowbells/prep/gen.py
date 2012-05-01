#!/usr/bin/env python
'''
Generate the properties file
'''

import cowbells
import geometry, properties

geo = cowbells.geo()
meter = cowbells.units.meter

def make_world(size = 10*meter):
    'Make the world volume'
    vac = geo.GetMedium("Vacuum")
    return geo.MakeBox("World",vac,size,size,size)

def fill(filename):
    from cowbells.prep import propmods
    for mod in propmods:
        mod.materials(geo)

    cbgb = geometry.CowbellGeometryBuilder()
    top = cbgb.top(geo)

    world = make_world()
    world.AddNode(top,1)
    geo.SetTopVolume(world)

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
