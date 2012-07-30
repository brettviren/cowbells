#!/usr/bin/env python
'''
Add water properties to a prop file
'''
name = 'Water'                  # I yam what I yam

import cowbells
import util

cm = cowbells.units.cm
eV = cowbells.units.eV
gram = cowbells.units.gram
cm3 = cowbells.units.cm3

parts = [
    ('Hydrogen',2),
    ('Oxygen',1),
    ]
density = 1.0 # don't use units here!  vmc?

# These properties are taken from WCSim.
# http://svn.phy.duke.edu/repos/neutrino/dusel/WCSim/trunk/src/WCSimConstructMaterials.cc

# energy bins
energy = [x*eV for x in [
    1.56962, 1.58974, 1.61039, 1.63157, 
    1.65333, 1.67567, 1.69863, 1.72222, 
    1.74647, 1.77142, 1.79710, 1.82352, 
    1.85074, 1.87878, 1.90769, 1.93749, 
    1.96825, 1.99999, 2.03278, 2.06666,
    2.10169, 2.13793, 2.17543, 2.21428, 
    2.25454, 2.29629, 2.33962, 2.38461, 
    2.43137, 2.47999, 2.53061, 2.58333, 
    2.63829, 2.69565, 2.75555, 2.81817, 
    2.88371, 2.95237, 3.02438, 3.09999,
    3.17948, 3.26315, 3.35134, 3.44444, 
    3.54285, 3.64705, 3.75757, 3.87499, 
    3.99999, 4.13332, 4.27585, 4.42856, 
    4.59258, 4.76922, 4.95999, 5.16665, 
    5.39129, 5.63635, 5.90475, 6.19998,
    ]]

# index of refraction
# WCSim comment: M Fechner : new ; define the water refraction index using refsg.F 
#                from skdetsim using the whole range.   
rindex = [
    1.32885, 1.32906, 1.32927, 1.32948, 1.3297, 1.32992, 1.33014, 
    1.33037, 1.3306, 1.33084, 1.33109, 1.33134, 1.3316, 1.33186, 1.33213,
    1.33241, 1.3327, 1.33299, 1.33329, 1.33361, 1.33393, 1.33427, 1.33462,
    1.33498, 1.33536, 1.33576, 1.33617, 1.3366, 1.33705, 1.33753, 1.33803,
    1.33855, 1.33911, 1.3397, 1.34033, 1.341, 1.34172, 1.34248, 1.34331,
    1.34419, 1.34515, 1.3462, 1.34733, 1.34858, 1.34994, 1.35145, 1.35312,
    1.35498, 1.35707, 1.35943, 1.36211, 1.36518, 1.36872, 1.37287, 1.37776,
    1.38362, 1.39074, 1.39956, 1.41075, 1.42535,
    ]

# absorption length
# WCSim comment: T. Akiri: Values from Skdetsim 
absorption = [x*cm for x in [
    16.1419,  18.278, 21.0657, 24.8568, 30.3117, 
    38.8341, 54.0231, 81.2306, 120.909, 160.238, 
    193.771, 215.017, 227.747,  243.85, 294.036, 
    321.647,  342.81, 362.827, 378.041, 449.378,
    739.434, 1114.23, 1435.56, 1611.06, 1764.18, 
    2100.95,  2292.9, 2431.33,  3053.6, 4838.23, 
    6539.65, 7682.63, 9137.28, 12220.9, 15270.7, 
    19051.5, 23671.3, 29191.1, 35567.9,   42583,
    49779.6, 56465.3,   61830, 65174.6, 66143.7,   
    64820,   61635, 57176.2, 52012.1, 46595.7, 
    41242.1, 36146.3, 31415.4, 27097.8, 23205.7, 
    19730.3, 16651.6, 13943.6, 11578.1, 9526.13,
    ]]

# Rayleigh scattering length 
# WCSim comment: T. Akiri: Values from Skdetsim 
rayleigh = [x*cm for x in [
      386929,  366249,  346398,  327355,  309097,  
      291603,  274853,  258825,  243500,  228856,  
      214873,  201533,  188816,  176702,  165173,
      154210,  143795,  133910,  124537,  115659,  
      107258, 99318.2, 91822.2,   84754, 78097.3, 
     71836.5,   65956, 60440.6, 55275.4, 50445.6,
       45937, 41735.2, 37826.6, 34197.6, 30834.9, 
     27725.4, 24856.6, 22215.9, 19791.3, 17570.9,   
       15543, 13696.6, 12020.5, 10504.1, 9137.15,
     7909.45,  6811.3, 5833.25,  4966.2, 4201.36, 
     3530.28, 2944.84, 2437.28, 2000.18,  1626.5, 
     1309.55, 1043.03, 821.016,  637.97, 488.754,
      ]]

def materials(geo):
    'Make any materials'
    mat = util.make_mixture(geo, name, parts, density)
    med = util.make_medium(geo, mat)
    

def properties(pf):
    'Save data into a properties file'
    pf.add(name, 'RINDEX',    zip(energy,rindex), ('Energy (MeV)',None))
    pf.add(name, 'ABSLENGTH', zip(energy,absorption), ('Energy (MeV)', 'Absorption Length (mm)'))
    pf.add(name, 'RAYLEIGH',  zip(energy,rayleigh), ('Energy (MeV)', 'Rayleigh Scattering (mm)'))
    return
