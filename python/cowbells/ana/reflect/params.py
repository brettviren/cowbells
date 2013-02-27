#!/usr/bin/env python
'''
Ingredients of the study
'''

from cowbells.ana.util import StringParams
from cowbells.units import inch, mm, MeV

def get(**kwds):
    '''
    Get the parameters of the study.  Any defaults can be overridden
    by the keywords.  All parameters of the study should be in here.
    '''

    default = dict(
        # for 'config' stage
        study='reflect',
        reflectivity='0.02',
        sample='Water', 
        particle='mu-',
        tub='Aluminum',

        # for 'sim' stage
        modules = 'kine,hits,steps,stacks',
        physics = "op,em",
        energy=500*MeV,  # MeV
        nevents=10,
         x=50*mm,  y=0,  z=100,
        dx=0,     dy=0, dz=-1,

        # for 'plot' stage
        material_number = 1,
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
    
                   
