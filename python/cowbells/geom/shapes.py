#!/usr/bin/env python
'''
Describe shapes.
'''
import math
import rotpos
from cowbells import units

store = []

class Shape(object):
    def __init__(self, name, **kwds):
        self.__dict__ = dict(name=name, type=self.__class__.__name__.lower())
        self.__dict__.update(kwds)
        store.append(self)
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

class Boolean(Shape):
    def __init__(self, name, booltype, shape1, shape2, rot=None, pos=None):
        if isinstance(shape1,Shape):
            shape1 = shape1.name
        if isinstance(shape2,Shape):
            shape2 = shape2.name
        booltype = booltype.lower()
        if booltype not in ['union', 'intersection', 'subtraction']:
            raise ValueError, 'Unknown boolean shape type "%s" for "%s"' % (booltype, name)
        super(Boolean, self).__init__(name, booltype=booltype, shape1=shape1, shape2=shape2,
                                      rot=rot, pos=pos)
        return
    def pod(self):
        p = super(Boolean, self).pod()
        p.update(rotpos.pod(self.rot, self.pos))
        return p
    pass


def get(sh):
    '''
    Return the shape with a name matching the given one.

    If shape itself is a Shape its name will be used for the search.
    '''

    if isinstance(sh, Shape):
        return sh
    for shape in store:
        if shape.name == sh:
            return shape
        continue
    return None

def pod(): 
    return [s.pod() for s in store]

