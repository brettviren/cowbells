#!/usr/bin/env python
'''
Describe optical material properties
'''

import base, materials

store = []

class MaterialProperty(base.Base):
    def __init__(self, matname, propname, x, y):
        if isinstance(matname, materials.Material):
            matname = matname.name
        self.__dict__ = dict(matname=matname, propname=propname, 
                             x=map(float,x), y=map(float,y))
        store.append(self)
        return
    def __str__(self):
        return '<MaterialProperty "%s/%s" [%d](%f,%f)>' % \
            (self.matname, self.propname, len(self.x), self.y[0],self.y[-1])
    pass

def by_material(mat):
    '''
    Return dictionary indexed by property name for all properties for the given material
    '''
    if isinstance(mat, materials.Material): 
        mat = mat.name

    ret = {}
    for mp in store:
        if mp.matname == mat:
            ret[mat.propname] = mat
    return ret

def get(mat, prop):
    '''
    Return the optical material property.
    '''
    if isinstance(mat, materials.Material): 
        mat = mat.name
    for mp in store:
        if mp.matname == mat and mp.propname == prop:
            return mp
    return None

def pod(): return base.pod(store)

