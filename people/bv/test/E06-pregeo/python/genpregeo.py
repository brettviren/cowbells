#!/usr/bin/env python
'''
Generate the geometry for the E06pregeo example.

usage from the command line:

  genpregeo.py filename.root

In E06 proper, this is done inside the VMC app.  Here we make an
intermediate ROOT file which will be loaded by the E06pregeo app.

This is largely a straight-forward copy of code from
Ex06DetectorConstruction.
'''

import ROOT
from array import array

fExpHallSize = 1000  # 10*m    
fTankSize = 500.0    # 5*m
fBubbleSize = 50.0   # 0.5*m

fImedAir = 1
fImedWater = 2

def make_element(name, sym, z, a):
    'Helper to make an element'
    ele  = ROOT.TGeoElement(name, sym, z, a)
    ROOT.SetOwnership(ele, 0)
    return ele

def make_mixture(name, density, parts):
    'Helper to make a mixture'
    mix = ROOT.TGeoMixture(name, len(parts), density)
    ROOT.SetOwnership(mix,0)
    for ele, frac in parts:
        mix.AddElement(ele, frac)
        continue
    return mix

def make_medium(name, numed, mat, params = None):
    'Helper to make a medium'
    if params:
        imat = ROOT.gGeoManager.GetMaterialIndex(mat.GetName())
        med = ROOT.TGeoMedium(name, numed, imat, *params)
    else:
        med = ROOT.TGeoMedium(name, numed, mat)
    ROOT.SetOwnership(med, 0)
    return med

def construct_materials():
    'Construct materials using TGeo modeller'

    # Create Root geometry manager 

    geo = ROOT.TGeoManager("E06_geometry", "E06 VMC example geometry")
    ROOT.SetOwnership(geo, 0)

    # Elements

    elH = make_element("Hydrogen", "H", 1,  1.01)
    elN = make_element("Nitrogen", "N", 7, 14.01)
    elO = make_element("Oxygen"  , "O", 8, 16.00)

    # Materials

    matAir = make_mixture("Air", 1.29e-03, [(elN, 0.7),(elO, 0.3)])
    matH2O = make_mixture("Water", 1.000, [(elH, 2), (elO, 1)])
  

    # Tracking media
    
    param = [
        0,      # isvol  - Not used
        2,      # ifield - User defined magnetic field
        10.,    # fieldm - Maximum field value (in kiloGauss)
        -20.,   # tmaxfd - Maximum angle due to field deflection 
        -0.01,  # stemax - Maximum displacement for multiple scat 
        -.3,    # deemax - Maximum fractional energy loss, DLS 
        .001,   # epsil - Tracking precision
        -.8,    # stmin
        ]
    make_medium("Air", fImedAir, matAir, param)
    make_medium("Water", fImedWater, matH2O, param)

    return

def construct_geometry():
    'Contruct volumes using TGeo modeller'

    # The experimental Hall
    ubuf = array('d',[0]*20)    # ?

    geo = ROOT.gGeoManager      # short hand

    expHall = array('d',[fExpHallSize, fExpHallSize, fExpHallSize])
    expHallV = geo.Volume("WRLD","BOX", fImedAir, expHall, 3)
    geo.SetTopVolume(expHallV)
   
    # The Water Tank
    waterTank = array('d',[fTankSize, fTankSize, fTankSize])
    geo.Volume("TANK","BOX", fImedWater, waterTank, 3)
   
    geo.Node("TANK", 1 ,"WRLD", 0.0, 0.0, 0.0, 0, True, ubuf)
  
    # The Air Bubble 
    bubbleAir = array('d',[fBubbleSize, fBubbleSize, fBubbleSize])
    geo.Volume("BUBL","BOX", fImedAir, bubbleAir, 3)
    geo.Node("BUBL", 1 ,"TANK", 0.0, 250.0, 0.0, 0, True, ubuf)

    geo.CloseGeometry()

    return
    
if __name__ == '__main__':
    import sys
    output = sys.argv[1]

    construct_materials()
    construct_geometry()
    geo = ROOT.gGeoManager
    geo.Export(output)
