#!/usr/bin/env python
'''
Generate the geometry for the E06pregeo example.

usage from the command line as run from E06-pregeo/:

  python python/genpregeo.py ex06pregeo.root

In E06 proper, this is done inside the VMC app.  Here we make an
intermediate ROOT file which will be loaded by the E06pregeo app.
run_g4.C will assume this named file is in the CWD.

The code below a straight-forward translation to PyROOT of code from
Ex06DetectorConstruction.
'''

import ROOT
from array import array

fExpHallSize = 1000  # 10*m    
fTankSize = 500.0    # 5*m
fBubbleSize = 50.0   # 0.5*m

fImedAir = 1
fImedWater = 2

def disown(obj):
    'Disown object created by an implicit ROOT constructor'
    ROOT.SetOwnership(obj,0)
    return obj

def make_element(name, sym, z, a):
    'Helper to make an element'
    ele  = ROOT.TGeoElement(name, sym, z, a)
    return disown(ele)


def make_mixture(name, density, parts):
    'Helper to make a mixture'
    print 'Making mixture "%s"' % name
    mix = ROOT.TGeoMixture(name, len(parts), density)
    for ele, frac in parts:
        print '\tadd element: %s %s' %(ele,str(frac))
        mix.AddElement(ele, frac)
        continue
    assert ROOT.gGeoManager.GetMaterial(name), 'Failed to get newly made material "%s"' % name
    return disown(mix)

def make_medium(name, numed, mat, params = None):
    'Helper to make a medium'
    imat = ROOT.gGeoManager.GetMaterialIndex(mat.GetName())
    print 'Making medium #%d "%s" from mat #%d "%s"' % (numed,name,imat,mat.GetName())
    if params:
        med = ROOT.TGeoMedium(name, numed, imat, *params)
    else:
        med = ROOT.TGeoMedium(name, numed, mat)
    assert ROOT.gGeoManager.GetMedium(name), 'Failed to get newly made medium "%s"' % name
    return disown(med)

def construct_materials():
    'Construct materials using TGeo modeller'

    # Create Root geometry manager 

    geo = ROOT.TGeoManager("E06_geometry", "E06 VMC example geometry")
    disown(geo)

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

def dump_geo(geo):
    print 'GeoManager: %s (%s)' % (geo.GetName(), geo.GetTitle())
    print 'Materials:'
    for matname in ['Air','Water']:
        mat = geo.GetMaterial(matname)
        print '\t%d %s' % (mat.GetIndex(), mat.GetName())
    print 'Media:'
    for medname in ['Air','Water']:
        med = geo.GetMedium(medname)
        print '\t%d %s' % (med.GetId(), med.GetName())
    return

def construct_geometry():
    'Contruct volumes using TGeo modeller'

    # The experimental Hall
    ubuf = array('d',[0]*20)    # ?

    geo = ROOT.gGeoManager      # short hand
    dump_geo(geo)

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
    ROOT.gGeoManager.Export(output)

