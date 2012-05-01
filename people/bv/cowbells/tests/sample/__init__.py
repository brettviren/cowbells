#!/usr/bin/env python
'''
Some sample code to faciliate testing
'''

import cowbells

def geom():
    'Make some trivial geometry'
    return cowbells.geo()

def load_geom(filename):
    'Import geometry from given file name and TDirectory name'
    geo = geom()
    geo.Import(filename)

def medium():
    'Return sample medium (water)'
    import cowbells.material.util as util
    mix = util.make_mixture("Water",[('Hydrogen',2),('Oxygen',1)],1.0)
    return util.make_medium(mix)

def topvol():
    'Create, set and return sample top volume'
    geo = geom()
    med = medium()

    from array import array
    worldboxsize = array('f',[10.0]*3)
    world = geo.Volume("World","BOX", med.GetId(), worldboxsize,3)
    geo.SetTopVolume(world)
    print 'Set top volume "%s" (id:%d") for "%s"' % \
        (med.GetName(), med.GetId(), geo.GetName())
    return world

def app():
    import cowbells
    return cowbells.app()

def mc():
    import cowbells
    return cowbells.mc()
