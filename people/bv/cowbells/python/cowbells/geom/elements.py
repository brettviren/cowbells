#!/usr/bin/env python
'''
Describe Atomic Elements
'''

import base

from cowbells import units
gpermole = units.gram/units.mole

store = []

class Element(base.Base):
    def __init__(self, name, symbol, z, a):
        self.__dict__ = dict(name=name.capitalize(), symbol=symbol.capitalize(), z=z, a=a)
        store.append(self)
        return
    def __str__(self):
        return '<Element "%s" (%s) a=%d z=%.1f>' % (self.name, self.symbol, self.z, self.a)
    def pod(self):
        p = dict(self.__dict__)
        p['a'] = '%f * gram/mole' % (self.a / gpermole)
        return p
    pass


def by_name(name):
    for e in store: 
        if e.name == name.capitalize(): 
            return e
    return None
def by_symbol(symbol):
    for e in store: 
        if e.symbol == symbol.capitalize():
            return e
    return None

def get(ident):
    if isinstance(ident, Element):
        return ident
    return by_name(ident) or by_symbol(ident)

def pod(): return base.pod(store)

