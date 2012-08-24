#!/usr/bin/env python
'''
Dump geometry or material properties
'''

import ROOT

from rootutil import walk

def _dump_property(prop):
    n = prop.GetN()
    print '\t[(%.2e,%.2e) -- (%.2e,%.2e)]\t%s' % \
        (prop.GetX()[0],prop.GetY()[0],
         prop.GetX()[n-1],prop.GetY()[n-1],
         prop.GetName())
    return


def _dump_material(matdir):
    print '%s:' % matdir.GetName()
    for dirpath, subdirs, objs in walk(matdir):
        for prop in objs:
            _dump_property(prop)
            continue
        continue
    return

def dump_materials(filename):
    '''
    Dump information about the materials in the given filename
    '''
    fp = ROOT.TFile.Open(filename)
    top = fp.Get('properties')
    for dirpath, subdirs, objs in walk(top):
        for sd in subdirs:
            _dump_material(sd)
            continue
        continue
    return

if __name__ == '__main__':
    import sys
    func = eval("dump_%s" % sys.argv[1])
    func(*sys.argv[2:])


