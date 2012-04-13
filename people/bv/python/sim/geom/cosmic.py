#!/usr/bin/env python
'''
The cosmic ray test detector
'''

import ROOT

import material

def top():
    'Return top volume'

    geo = ROOT.gGeoManager
    material.init()

    radius = 2.5                    # cm
    height = 70.0                   # cm
    thick = 0.5                     # cm

    acrylic = geo.GetMaterial('Acrylic')
    acrylic = material.make_medium(acrylic)
    acrylic = geo.MakeTube("AcrylicTube", acrylic, 0., radius, 0.5*height)

    wbls = geo.GetMaterial('WBLS')
    wbls = material.make_medium(wbls)
    wbls = geo.MakeTube('WBLS', wbls, 0., radius - thick, 0.5*height - thick)
    acrylic.AddNode(wbls,1)
    return acrylic

def test_draw():
    t = top()
    geo = ROOT.gGeoManager
    geo.SetTopVolume(t)
    geo.CloseGeometry()
    t.SetLineColor(2)
    geo.SetTopVisible()
    c = ROOT.TCanvas("c","canvas",500,500)
    t.Draw()
    c.Print("test_draw.ps")
    return


if __name__ == '__main__':
    geo = ROOT.TGeoManager("geo","Geo Manager")
    test_draw()
