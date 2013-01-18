#!/usr/bin/env python
'''
Ingredients of the study
'''

from cowbells.ana.util import StringParams

def get(**kwds):
    '''
    Get the parameters of the study.  Any defaults can be overridden
    by the keywords.  All parameters of the study should be in here.
    '''

    default = dict(
        # for 'config' stage
        study='under',
        sample='water', 
        particle='proton',
        tub='teflon',
        energy=500,  # MeV
        nevents=10,
        x=0,y=0,z=0,
        dx=1,dy=0,dz=0,

        # for 'sim' stage

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
    
                   
