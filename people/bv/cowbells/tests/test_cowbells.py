#!/usr/bin/env python
'''
A test cowbells application.  Each test should be run in order in the
same job.
'''


import ROOT
import PyCintex                 # turns on using REFLEX dictionaries

def test_create():
    'Create the Cowbells::Interface'
    # ROOT.gSystem.Load("libcowbellsDict")
    ROOT.gSystem.Load("libcowbells")

    print ROOT.Cowbells
    print ROOT.Cowbells.Interface
    print 'Making Cowbells.Interface'
    inter = ROOT.Cowbells.Interface()
    print inter

    

if __name__ == '__main__':
    test_create()
