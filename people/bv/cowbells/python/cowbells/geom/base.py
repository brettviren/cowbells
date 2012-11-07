#!/usr/bin/env python
'''
Base functionality
'''

class Base(object):
    def pod(self): return self.__dict__

def pod(store=None):
    '''
    Return the store of a geom module as plain-old-data.

    Do not call base.pod directly, call it through the module.
    '''
    return [o.pod() for o in store]
