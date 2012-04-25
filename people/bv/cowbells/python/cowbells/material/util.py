#!/usr/bin/env python
'''
Functions to help build materials
'''

import ROOT

import cowbells

geo = cowbells.geo()

def make_mixture(name, parts, density):
    'Make a mixture'

    mix = ROOT.TGeoMixture(name, len(parts), density)
    ROOT.SetOwnership(mix,0)
    print 'Mixture: %s' % name
    for part in parts:
        ele = geo.GetElementTable().FindElement(part[0])
        mix.AddElement(ele, part[1])
        print '\telement: %s %s' % (part[0],str(part[1]))
        continue
    return mix

# Allocate media so we can make sure to provide a unique number.
_media = []
def make_medium(mat, name = None, params = None):
    'Make a medium from a material'

    if name is None: 
        name = mat.GetName()

    med = geo.GetMedium(name)
    if med: return med

    global _media
    numed = len(_media)

    if params:
        med = ROOT.TGeoMedium(name, numed, mat, params)
    else:
        med = ROOT.TGeoMedium(name, numed, mat)

    _media.append(med)
    ROOT.SetOwnership(med,0)
    return med

def test():
    print geo
    
if __name__ == '__main__':
    test()

