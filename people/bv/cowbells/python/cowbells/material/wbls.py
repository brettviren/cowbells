#!/usr/bin/env python
'''
Water-based Liquid Scintilator stuff
'''

import water
import util

parts =   [('Hydrogen', 0.659),
           ('Oxygen', 0.309),
           ('Sulfur', 0.0009),
           ('Nitrogen', 0.000058),
           ('Carbon', 0.031)]
density = 0.9945

energy = water.energy
rindex = water.rindex

# Minfang's absorption length for Water LS, via WCSim_WbLS
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

# Minfang' new scattering length for WA-LS, via WCSim_WbLS
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

def medium = util.generic_medium
def register = util.generic_register

