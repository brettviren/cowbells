#!/usr/bin/env python

import ROOT
geo = ROOT.TGeoManager("geo","Geo Manager")

def make_mix():
    parts = [('Hydrogen',0.659),('Oxygen',0.309)]
    mix = ROOT.TGeoMixture('Water', len(parts), 1.0)
    ROOT.SetOwnership(mix,0)
    for part in parts:
        ele = geo.GetElementTable().FindElement(part[0])
        mix.AddElement(ele, part[1])
        continue
    return

def make_water_box():
    water = geo.GetMaterial('Water')
    med = ROOT.TGeoMedium('Water',1,water)
    box = geo.MakeBox("Top",med,10,10,10)
    return box

def do_it():
    make_mix()
    box = make_water_box()
    geo.SetTopVolume(box)
    geo.CloseGeometry()
    box.Draw()

if __name__ == '__main__':
    do_it()


