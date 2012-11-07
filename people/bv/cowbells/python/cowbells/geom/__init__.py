#!/usr/bin/env python
'''
Describe detector geometry
'''

import elements, materials, optical, volumes, placements, surfaces, sensitive
mods = [elements, materials, optical, volumes, placements, surfaces, sensitive]


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

