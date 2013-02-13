#!/usr/bin/env python
'''
Parameters for boxdet study
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
        label='nominal',
        study='boxes',
        sample='Water', 
        particle='proton',
        box='Teflon',

        # for 'sim' stage
        physics = "op,em",
        energy=500*MeV,  # MeV
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
    
                   
