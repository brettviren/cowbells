#!/usr/bin/env python

from array import array
from util import make_medium, make_mixture
from cowbells.units import eV, cm, m

parts = [
    ('Hydrogen',2),
    ('Oxygen',1),
    ]
density = 1.0

# properties
energy = [
    1.56962*eV, 1.58974*eV, 1.61039*eV, 1.63157*eV, 
    1.65333*eV, 1.67567*eV, 1.69863*eV, 1.72222*eV, 
    1.74647*eV, 1.77142*eV, 1.7971*eV, 1.82352*eV, 
    1.85074*eV, 1.87878*eV, 1.90769*eV, 1.93749*eV, 
    1.96825*eV, 1.99999*eV, 2.03278*eV, 2.06666*eV,
    2.10169*eV, 2.13793*eV, 2.17543*eV, 2.21428*eV, 
    2.25454*eV, 2.29629*eV, 2.33962*eV, 2.38461*eV, 
    2.43137*eV, 2.47999*eV, 2.53061*eV, 2.58333*eV, 
    2.63829*eV, 2.69565*eV, 2.75555*eV, 2.81817*eV, 
    2.88371*eV, 2.95237*eV, 3.02438*eV, 3.09999*eV,
    3.17948*eV, 3.26315*eV, 3.35134*eV, 3.44444*eV, 
    3.54285*eV, 3.64705*eV, 3.75757*eV, 3.87499*eV, 
    3.99999*eV, 4.13332*eV, 4.27585*eV, 4.42856*eV, 
    4.59258*eV, 4.76922*eV, 4.95999*eV, 5.16665*eV, 
    5.39129*eV, 5.63635*eV, 5.90475*eV, 6.19998*eV,
    ]
rindex = [
    1.32885, 1.32906, 1.32927, 1.32948, 
    1.3297, 1.32992, 1.33014, 1.33037, 
    1.3306, 1.33084, 1.33109, 1.33134, 
    1.3316, 1.33186, 1.33213, 1.33241, 
    1.3327, 1.33299, 1.33329, 1.33361, 
    1.33393, 1.33427, 1.33462, 1.33498, 
    1.33536, 1.33576, 1.33617, 1.3366, 
    1.33705, 1.33753, 1.33803, 1.33855, 
    1.33911, 1.3397, 1.34033, 1.341, 
    1.34172, 1.34248, 1.34331, 1.34419, 
    1.34515, 1.3462, 1.34733, 1.34858, 
    1.34994, 1.35145, 1.35312, 1.35498, 
    1.35707, 1.35943, 1.36211, 1.36518, 
    1.36872, 1.37287, 1.37776, 1.38362, 
    1.39074, 1.39956, 1.41075, 1.42535
    ]
absorption = [
    7.9741*cm, 9.02933*cm, 10.4065*cm, 12.2793*cm, 
    14.974*cm, 19.184*cm, 26.6874*cm, 40.128*cm, 
    59.7291*cm, 79.1575*cm, 95.7229*cm, 106.218*cm, 
    112.507*cm, 120.462*cm, 145.254*cm, 158.894*cm, 
    169.348*cm, 179.237*cm, 186.752*cm, 221.993*cm, 
    365.279*cm, 550.429*cm, 709.167*cm, 795.864*cm, 
    871.504*cm, 1037.87*cm, 1132.69*cm, 1201.08*cm, 
    1508.48*cm, 2390.09*cm, 3230.6*cm, 3795.21*cm, 
    4513.81*cm, 6037.13*cm, 6544.36*cm, 5550.56*cm, 
    5040.82*cm, 4574.07*cm, 4116.67*cm, 3714.29*cm, 
    3250*cm, 2806.82*cm, 2386.47*cm, 1937.25*cm, 
    1520*cm, 1064.66*cm, 500.507*cm, 370.037*cm, 
    225.571*cm, 137.989*cm, 106.58*cm, 22.1118*cm, 
    13.3941*cm, 12.6969*cm, 12.8165*cm, 13.1955*cm, 
    13.2277*cm, 12.6725*cm, 13.429*cm, 16.1433*cm,
    ]
rayleigh = [
    167024*cm, 158727*cm, 150742*cm, 143062*cm, 135680*cm, 
    128587*cm, 121776*cm, 115239*cm, 108969*cm, 102959*cm, 
    97200.4*cm, 91686.9*cm, 86411.3*cm, 81366.8*cm, 76546.4*cm, 
    71943.5*cm, 67551.3*cm, 63363.4*cm, 59373.2*cm, 55574.6*cm, 
    51961.2*cm, 48527*cm, 45265.9*cm, 42171.9*cm, 39239.4*cm, 
    36462.5*cm, 33835.7*cm, 31353.4*cm, 29010.3*cm, 26801*cm, 
    24720.4*cm, 22763.4*cm, 20924.9*cm, 19200.1*cm, 17584.2*cm, 
    16072.5*cm, 14660.4*cm, 13343.5*cm, 12117.3*cm, 10977.7*cm, 
    9920.42*cm, 8941.41*cm, 8036.71*cm, 7202.47*cm, 6434.93*cm, 
    5730.43*cm, 5085.43*cm, 4496.47*cm, 3960.21*cm, 3473.41*cm, 
    3032.94*cm, 2635.75*cm, 2278.91*cm, 1959.59*cm, 1675.06*cm, 
    1422.71*cm, 1200*cm, 1004.53*cm, 830*cm, 686.106*cm
    ]

def medium():
    'Return the water medium'
    mat = make_mixture("Water", parts, density)
    if not mat: return None
    return make_medium(mat)

def register(mc):
    'Register water with the Monte Carlo'

    assert mc, 'Null MC passed'

    med = medium()
    wid = med.GetId()           # Must. Love. The. Interface.

    nentries = len(energy)
    array_type = 'd'
    efficiency = array(array_type,[0.0]*nentries)
    print 'energy:',energy
    print 'abs:',absorption
    print 'eff:',efficiency
    print 'rind:',rindex
    mc.SetCerenkov(wid, nentries, 
                   array(array_type,energy), 
                   array(array_type,absorption),
                   efficiency, 
                   array(array_type,rindex))
                   
    mc.SetMaterialProperty(wid, "RAYLEIGH", nentries, energy, rayleigh)

    # gMC->SetMaterialProperty(fImedWater, 
    #                          "FASTCOMPONENT", nEntries, photonEnergy, scintilFast);
    # gMC->SetMaterialProperty(fImedWater, 
    #                          "SLOWCOMPONENT", nEntries, photonEnergy, scintilSlow);

    # gMC->SetMaterialProperty(fImedWater, "SCINTILLATIONYIELD", 50.e03);  // 50./MeV
    # gMC->SetMaterialProperty(fImedWater, "RESOLUTIONSCALE",  1.0);
    # gMC->SetMaterialProperty(fImedWater, "FASTTIMECONSTANT",  1.0e-09);  // 1.*ns
    # gMC->SetMaterialProperty(fImedWater, "SLOWTIMECONSTANT", 10.0e-09); // 10.*ns
    # gMC->SetMaterialProperty(fImedWater, "YIELDRATIO", 0.8);
    
    return

def test():
    'Test registering water, requires ROOT.gMC to be ready'
    import ROOT
    register(ROOT.gMC)

if __name__ == '__main__':
    test()
