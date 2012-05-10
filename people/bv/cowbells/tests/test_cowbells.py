#!/usr/bin/env python
'''
A test cowbells application.  Each test should be run in order in the
same job.
'''


import PyCintex
import ROOT
ROOT.gSystem.Load("libcowbells")
ROOT.gSystem.Load("libcowbellsDict")

interface = None

def test_create():
    'Create the Cowbells::Interface'

    print ROOT.Cowbells
    print ROOT.Cowbells.Interface
    print 'Making Cowbells.Interface'
    inter = ROOT.Cowbells.Interface()
    print inter
    global interface
    interface = inter
    return

def test_configure():
    'Initialize the geometry/mc'

    global interface
    interface.configure('geo.root')
    
    

if __name__ == '__main__':
    test_create()
