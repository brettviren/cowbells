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
# T. Akiri: Values from Skdetsim, via WCSim
absorption = [
    16.1419*cm,  18.278*cm, 21.0657*cm, 24.8568*cm, 30.3117*cm, 
    38.8341*cm, 54.0231*cm, 81.2306*cm, 120.909*cm, 160.238*cm, 
    193.771*cm, 215.017*cm, 227.747*cm,  243.85*cm, 294.036*cm, 
    321.647*cm,  342.81*cm, 362.827*cm, 378.041*cm, 449.378*cm,
    739.434*cm, 1114.23*cm, 1435.56*cm, 1611.06*cm, 1764.18*cm, 
    2100.95*cm,  2292.9*cm, 2431.33*cm,  3053.6*cm, 4838.23*cm, 
    6539.65*cm, 7682.63*cm, 9137.28*cm, 12220.9*cm, 15270.7*cm, 
    19051.5*cm, 23671.3*cm, 29191.1*cm, 35567.9*cm,   42583*cm,
    49779.6*cm, 56465.3*cm,   61830*cm, 65174.6*cm, 66143.7*cm,   
      64820*cm,   61635*cm, 57176.2*cm, 52012.1*cm, 46595.7*cm, 
    41242.1*cm, 36146.3*cm, 31415.4*cm, 27097.8*cm, 23205.7*cm, 
    19730.3*cm, 16651.6*cm, 13943.6*cm, 11578.1*cm, 9526.13*cm
    ]

# T. Akiri: Values from Skdetsim, via WCSim
rayleigh = [
    386929*cm,  366249*cm,  346398*cm,  327355*cm,  309097*cm,  
    291603*cm,  274853*cm,  258825*cm,  243500*cm,  228856*cm,  
    214873*cm,  201533*cm,  188816*cm,  176702*cm,  165173*cm,
    154210*cm,  143795*cm,  133910*cm,  124537*cm,  115659*cm,  
    107258*cm, 99318.2*cm, 91822.2*cm,   84754*cm, 78097.3*cm, 
   71836.5*cm,   65956*cm, 60440.6*cm, 55275.4*cm, 50445.6*cm,
     45937*cm, 41735.2*cm, 37826.6*cm, 34197.6*cm, 30834.9*cm,
   27725.4*cm, 24856.6*cm, 22215.9*cm, 19791.3*cm, 17570.9*cm,   
     15543*cm, 13696.6*cm, 12020.5*cm, 10504.1*cm, 9137.15*cm,
   7909.45*cm,  6811.3*cm, 5833.25*cm,  4966.2*cm, 4201.36*cm, 
   3530.28*cm, 2944.84*cm, 2437.28*cm, 2000.18*cm,  1626.5*cm, 
   1309.55*cm, 1043.03*cm, 821.016*cm,  637.97*cm, 488.754*cm
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
