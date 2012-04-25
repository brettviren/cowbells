#!/usr/bin/env python
'''
Some sample code to faciliate testing
'''

import cowbells

def geom():
    'Make some trivial geometry'
    return cowbells.geo()

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
    print 'Set top volume for "%s"' % geo.GetName()
    return world

def app():
    '''Make and return the Cowbells VMC application, this needs to
    follow creation of some geometry'''
    import cowbells.app
    return cowbells.app.app()

