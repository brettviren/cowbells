#!/usr/bin/env python
'''
Describe shapes and logical volumes.
'''

import math
import base, materials

from cowbells import units

### SHAPES ###

# added as needed.  Each new shape should pass up its name and all
# defining measurements to the Shape parent and implement pod() adding
# these measurements as strings with units.

class Shape(object):
    def __init__(self, name, **kwds):
        self.__dict__ = dict(name=name, type=self.__class__.__name__.lower())
        self.__dict__.update(kwds)
        return
    def pod(self):
        'Return data as dictionary of plain old data'

        p = dict(self.__dict__)

        # Look for length dimensions:
        for l in ('x','y','z','rmin','rmax','dz'):
            if not p.has_key(l): continue
            p[l] = "%f * mm" % (p[l] / units.mm)

        # Look for angle dimensions
        for a in ['sphi','dphi']:
            if not p.has_key(a): continue
            p[a] = "%.14f * radian" % (p[a] / units.radian)

        return p

    pass

class Box(Shape):
    def __init__(self, name, x, y, z):
        super(Box, self).__init__(name,x=x,y=y,z=z)

class Tubs(Shape):
    def __init__(self, name, rmax, dz, rmin=0.0, sphi=0.0, dphi=math.pi*2*units.radian):
        super(Tubs, self).__init__(name, rmax=rmax, dz=dz, rmin=rmin, sphi=sphi, dphi=dphi)

class Polycone(Shape):
    def __init__(self, name, zplane, rinner, router, sphi=0.0, dphi=math.pi*2*units.radian):
        super(Polycone, self).__init__(name, zplane=zplane, rinner=rinner, router=router,
                                       sphi=sphi, dphi=dphi)
    def pod(self):
        p = super(Polycone, self).pod()
        p['zplane'] = ['%f * mm'%(l/units.mm) for l in self.zplane]
        p['rinner'] = ['%f * mm'%(l/units.mm) for l in self.rinner]
        p['router'] = ['%f * mm'%(l/units.mm) for l in self.router]
        return p


### LOGICAL VOLUMES ###

store = []

class LogicalVolume(base.Base):
    def __init__(self, name, matname, shape):
        if isinstance(matname, materials.Material):
            matname = matname.name
        assert materials.get(matname), 'No material: %s' % matname
        self.__dict__ = dict(name=name, matname=matname, shape=shape)
        store.append(self)
        return

    def pod(self):
        '''
        Return self as a plain old data structure.
        '''
        me = dict(self.__dict__)
        me['shape'] = self.shape.pod()
        return me

    def __str__(self):
        return '<LogicalVolume "%s" mat:"%s" shape:"%s">' % \
            (self.name, self.matname, self.shape.name)
    pass

def get(lv):
    '''
    Return the LogicalVolume with a name matching the given one.

    If lv itself is a LogicalVolume its name will be used for the search.
    '''

    if isinstance(lv, LogicalVolume):
        return lv
    for vol in store:
        if vol.name == lv:
            return vol
        continue
    return None



def pod(): return base.pod(store)
