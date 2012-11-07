#!/usr/bin/env python
'''
Describe detector geometry
'''

class Base(object): pass


### ELEMENTS ###

elements = []
def get_element_by_name(name):
    for e in elements: 
        if e.name == name.capitalize(): 
            return e
    return None
def get_element_by_symbol(symbol):
    for e in elements: 
        if e.symbol == symbol.capitalize():
            return e
    return None

class Element(Base):
    def __init__(self, name, symbol, z, a):
        self.__dict__ = dict(name=name.capitalize(), symbol=symbol.capitalize(), z=z, a=a)
        elements.append(self)
        return
    def __str__(self):
        return '<Element "%s" (%s) a=%d z=%.1f>' % (self.name, self.symbol, self.z, self.a)
    pass


### MATERIALS ###

materials = []
def get_material(name):
    for m in materials:
        if m.name == name: 
            return m
    return None

class Material(Base):
    def __init__(self, name, density, constituents):
        self.__dict__ = dict(name=name, density = density)
        parts={}
        for e,n in constituents:
            if isinstance(e,Element): 
                parts[e.symbol] = n
                continue
            ele = get_element_by_name(e)
            if ele:
                parts[ele.symbol] = n
                continue
            ele = get_element_by_symbol(e)
            if ele:
                parts[ele.symbol] = n
                continue
            raise ValueError, 'No element defined named "%s"' % e
        self.elements = parts
        materials.append(self)
        return
    def __str__(self):
        return '<Material "%s" dens=%.1f [%s]>' % \
            (self.name, self.density, ','.join(['(%s:%s)'%c for c in self.elements.iteritems()]))

    pass


### MATERIAL PROPERTIES ###

material_properties = []

class MaterialProperty(Base):
    def __init__(self, matname, propname, x, y):
        self.__dict__ = dict(matname=matname, propname=propname, x=x, y=y)
        material_properties.append(self)
        return
    def __str__(self):
        return '<MaterialProperty "%s/%s" [%d](%f,%f)>' % \
            (self.matname, self.propname, len(self.x), self.y[0],self.y[-1])
    pass



### OPTICAL SURFACES ###

optical_surfaces = []

class OpticalSurface(Base):
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
        optical_surfaces.append(self)
        return

    def add_parameter(self, key, value):
        assert key in self.known_parameters, \
            'Unknown parameter given to surface %s: "%s"' % (self.name, key)
        self.parameters[key] = value
        return

    def add_property(self, propname, x, y):
        self.properties[propname] = {'x':x, 'y':y}
        return

    pass
    

### LOGICAL VOLUMES ###

logical_volumes = []
def get_logical_volume(name):
    for lv in logical_volumes:
        if lv.name == name:
            return lv
        continue
    return None

class Shape(Base):
    def __init__(self, name, type, **kwds):
        self.__dict__ = dict(name=name, type=type)
        self.__dict__.update(kwds)
        return
    pass

class LogicalVolume(Base):
    def __init__(self, name, matname, shape):
        self.__dict__ = dict(name=name, matname=matname, shape=shape)
        logical_volumes.append(self)
        return
    pass

### PLACEMENTS/PHYSICAL VOLUMES ###

physical_volumes = []

class PhysicalVolume(Base):
    def __init__(self, name, lvmother, lvdaughter, rot=None, pos=None, copy=0):
        for lv in [lvmother,lvdaughter]:
            assert get_logical_volume(lv), 'No logical volume "%s"' lv

        self.__dict__=dict(name=name, lvmother=lvmother, lvdaughter=lvdaughter,
                           rot=rot, pos=pos, copy=copy):
        physical_volumes.append(self)
        return
    pass




def all_data():
    mps = {}
    for mp in material_properties:
        matprop = mps.get(mp.matname)
        if not matprop: 
            matprop = {}
            mps[mp.matname] = matprop
        matprop[mp.propname] = { 'x':mp.x, 'y':mp.y }
        continue
    dat = {
        'elements': [e.__dict__ for e in elements],
        'materials': [m.__dict__ for m in materials],
        'matprops': mps,
        'surfaces': [s.__dict__ for s in optical_surfaces]
        }
    return dat

def dumps_json():
    import json
    return json.dumps(all_data(), indent=2)

if '__main__' == __name__:
    h = Element('hydrogen','h',1, 1.01)
    o = Element('oxygen',  'o',8,16.0)
    water = Material('Water',1.0, [('h',2), (o,1)])
    print ' '.join([str(e) for e in elements])
    print ' '.join([str(m) for m in materials])

    mp = MaterialProperty('Water', 'RINDEX', x=[1.329,1.425], y=[1.589e-6,6.20e-6])

    surf = OpticalSurface('TestSurface', model='glisur', 
                          type='dielectric_metal', finish='polished')
    surf.add_parameter('first','pvFirstVolume')
    surf.add_parameter('second','pvSecondVolume')
    surf.add_property("REFLECTIVITY", x=[1,6], y=[1,1])

    print 'JSON:'
    print dumps_json()
