#!/usr/bin/env python
'''
Ingredients of the study
'''

from cowbells.ana.util import StringParams
from cowbells.units import inch, mm, MeV

def get(**kwds):
    default = dict(
        # for 'config' stage
        study='prod',
        sample='water', 
        tub='teflon',

        # for 'sim' stage
        particle='proton',
        physics='em,op',
        energy=2000*MeV,
        nevents=10,
        x=-100*mm,y=0,z=0,
        dx=1,dy=0,dz=0,

        # for 'plot' stage
        tree = 'cowbells',
        )
    default.update(kwds)
    sp = StringParams(default)
    sp.lock()
    return sp

if __name__ == '__main__':
    sp = get()
    print sp
    print sp.does_not_exist     # should raise exception
    
                   
