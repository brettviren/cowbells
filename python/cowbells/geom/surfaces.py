#!/usr/bin/env python
'''
Describe optical surfaces
'''

import base, volumes

store = []

class OpticalSurface(base.Base):
    # Known parameters
    known_parameters = ['type', 'model', 'finish', 'first', 'second',
                        'polish', 'sigmaalpha']

    # Known properties
    known_properties = ['RINDEX','REALRINDEX','IMAGINARYRINDEX',
                        'REFLECTIVITY','EFFICIENCY','TRANSMITTANCE',
                        'SPECULARLOBECONSTANT','SPECULARSPIKECONSTANT',
                        'BACKSCATTERCONSTANT']

    def __init__(self, name, **parameters):

        self.name = name
        self.parameters = {}
        self.properties = {}
        for k,v in parameters.iteritems():
            self.add_parameter(k,v)
            continue
        store.append(self)
        return

    def add_parameter(self, key, value):
        assert key in self.known_parameters, \
            'Unknown parameter given to surface %s: "%s"' % (self.name, key)
        if key in ['first','second']:
            if isinstance(value, volumes.LogicalVolume):
                value = value.name
        self.parameters[key] = value
        return

    def add_property(self, propname, x, y):
        self.properties[propname] = {'x':x, 'y':y}
        return

    pass
    

def get(surf):
    if isinstance(surf, OpticalSurface):
        return surf
    for s in store:
        if s.name == surf:
            return s
    return None

def pod(): return base.pod(store)

