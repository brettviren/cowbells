#!/usr/bin/env python
'''
Describe materials
'''

from cowbells import units
gpercm3 = units.gram/units.cm3

import base, elements

store = []
def get(name):
    for m in store:
        if m.name == name: 
            return m
    return None

class Material(base.Base):
    def __init__(self, name, density, elelist = None, matlist = None):
        store.append(self)
        self.__dict__ = dict(name=name, density = density)
        
        if elelist:
            parts={}
            for e,n in elelist:
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
            pass
            
        if matlist:
            parts = {}
            for m,n in matlist:
                if isinstance(m,Material):
                    parts[m.name] = n
                    continue
                mat = get(m)
                if mat:
                    parts[mat.name] = n
                    continue
                raise ValueError, 'No material defined named "%s"' % m
            self.materials = parts
            pass
        return

    def __str__(self):
        return '<Material "%s" dens=%.1f elements=[%s] materials=[%s]>' % \
            (self.name, self.density,
             ','.join(['(%s:%s)'%c for c in self.elements.iteritems()]),
             ','.join(['(%s:%s)'%c for c in self.materials.iteritems()]))

    def pod(self):
        p = dict(self.__dict__)
        p['density'] = '%f * gram/cm3' % (self.density / gpercm3)
        return p

    pass

def pod(): return base.pod(store)

