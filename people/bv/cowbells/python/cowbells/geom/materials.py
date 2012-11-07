#!/usr/bin/env python
'''
Describe materials
'''

import base, elements

store = []
def get(name):
    for m in store:
        if m.name == name: 
            return m
    return None

class Material(base.Base):
    def __init__(self, name, density, constituents):
        self.__dict__ = dict(name=name, density = density)
        parts={}
        for e,n in constituents:
            if isinstance(e,elements.Element): 
                parts[e.symbol] = n
                continue
            ele = elements.get(e)
            if ele:
                parts[ele.symbol] = n
                continue
            ele = elements.get(e)
            if ele:
                parts[ele.symbol] = n
                continue
            raise ValueError, 'No element defined named "%s"' % e
        self.elements = parts
        store.append(self)
        return
    def __str__(self):
        return '<Material "%s" dens=%.1f [%s]>' % \
            (self.name, self.density, ','.join(['(%s:%s)'%c for c in self.elements.iteritems()]))

    pass

def pod(): return base.pod(store)

