#!/usr/bin/env python
'''
Describe detector geometry
'''

import elements, materials, optical, volumes, placements, surfaces
mods = [elements, materials, optical, volumes, placements, surfaces]




def pod():
    '''
    Return all objects as a plain-old-data structure
    '''
    ret = {}
    for mod in mods:
        name = mod.__name__.split('.')[-1]
        ret[name] = mod.pod()
        continue
    return ret

def dumps_json():
    import json
    return json.dumps(pod(), indent=2)

if '__main__' == __name__:
    h = elements.Element('hydrogen','h',1, 1.01)
    o = elements.Element('oxygen',  'o',8,16.0)
    water = materials.Material('Water',1.0, [('h',2), (o,1)])
    print ' '.join([str(e) for e in elements.store])
    print ' '.join([str(m) for m in materials.store])

    mp = optical.MaterialProperty('Water', 'RINDEX', x=[1.329,1.425], y=[1.589e-6,6.20e-6])

    surf = surfaces.OpticalSurface('TestSurface', model='glisur', 
                                   type='dielectric_metal', finish='polished')
    surf.add_parameter('first','pvFirstVolume')
    surf.add_parameter('second','pvSecondVolume')
    surf.add_property("REFLECTIVITY", x=[1,6], y=[1,1])

    print 'JSON:'
    print dumps_json()
