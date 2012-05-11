#!/usr/bin/env python
'''
A test cowbells application.  Each test should be run in order in the
same job.
'''


import os
import cowbells

interface = None

def test_create():
    'Create the Cowbells::Interface'

    print 'Making Cowbells.Interface'
    inter = cowbells.interface()
    print inter

    global interface
    interface = inter
    return

def test_configure():
    'Initialize the geometry/mc'

    global interface
    print 'Configuring cowbells'
    geofile = 'geo.root'
    assert os.path.exists(geofile), 'File does not exist: "%s" (run cowbells/prep/gen.py?)' % geofile
    interface.configure(geofile)
    return

def test_initialize():
    'Initialize'
    global interface    
    print 'Initializing Cowbells'
    interface.initialize()
    interface.register_lvsd("PC")
    return

def test_run():
    'Run some events'
    global interface    
    print 'Running some events'
    interface.simulate()
    return
    
if __name__ == '__main__':
    test_create()
    test_configure()
    test_initialize()
    test_run()
