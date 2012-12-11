#!/usr/bin/env python
'''
Generate the properties file
'''

import os
import cowbells
import geometry, properties

meter = cowbells.units.meter

def make_world(geo, size = 10*meter):
    'Make the world volume'
    vac = geo.GetMedium("Vacuum")
    return geo.MakeBox("World",vac,size,size,size)

def fill(geo, filename):
    from cowbells.prep import propmods
    for mod in propmods:
        mod.materials(geo)

    cbgb = geometry.CowbellGeometryBuilder()
    top = cbgb.top(geo)

    world = make_world(geo)
    world.AddNode(top,1)
    geo.SetTopVolume(world)

    print 'Writing geometry'
    import ROOT
    fp = ROOT.TFile.Open(filename, "update")
    geo.Write("geometry")       # fixme, this probably collides once geometry.py is written....
    fp.Close()

    properties.fill(filename)

    gdmlfile = os.path.splitext(filename)[0] + '.gdml'
    geo.Export(gdmlfile)

    return

if __name__ == '__main__':
    import sys
    import ROOT
    geo = ROOT.TGeoManager('cowbells_geometry', 
                           'Geometry for COsmic WB(el)LS detector')
    fill(geo, sys.argv[1])
