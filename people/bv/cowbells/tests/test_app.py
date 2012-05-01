#!/usr/bin/env python

import sample

def test_app():

    print 'Making geometry'
    geo = sample.load_geom("geo.root","geometry")

    print 'Making app'
    app = sample.app()

    # fixme: run prep/gen.py to make an example
    propfile = "prop.root"
    print 'Setting properties file "%s"' % propfile 
    app.SetPropertiesFile(propfile)

    mc = sample.mc()

    print 'Initialize MC, triggers C++ detector construction'
    mc.Init()

    print 'Build MC physics'
    mc.BuildPhysics()

    print 'Running 10'
    mc.ProcessRun(10)

    print 'And, I am out'
    return

if __name__ == '__main__':
    test_app()

