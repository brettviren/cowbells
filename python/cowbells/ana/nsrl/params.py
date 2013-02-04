#!/usr/bin/env python
'''
Ingredients of the study
'''

from cowbells.ana.util import StringParams
from cowbells.units import inch, mm, MeV, meter

def get(**kwds):
    default = dict(
        # for 'config' stage
        study='nsrl',
        label='nominal',        # arbitrary
        sample='water', 

        # for 'sim' stage
        particle='proton',
        physics='em,op',
        energy=2000*MeV,
        nevents=10,
        x=0,y=0,z=-5.1*meter,
        dx=0,dy=0,dz=1,

        # for 'plot' stage
        tree = 'cowbells',
        )
    default.update(kwds)
    sp = StringParams(default)
    sp.lock()
    return sp
