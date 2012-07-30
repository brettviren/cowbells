#!/usr/bin/env python
'''
Functions to help build materials
'''

import ROOT

import cowbells


def get_stuff(geo,stuff):
    'Try to return element or material'
    ele = geo.GetElementTable().FindElement(stuff)
    if ele: 
        ROOT.SetOwnership(ele,0)
        return ele

    mat = geo.GetMaterial(stuff)
    if mat: return mat

    return None

def make_mixture(geo, name, parts, density):
    'Make a mixture'

    if not parts:
        mix = ROOT.TGeoMaterial(name, 0, 0, density)
    else:
        mix = ROOT.TGeoMixture(name, len(parts), density)
    ROOT.SetOwnership(mix,0)
    print 'Mixture: %s' % name
    for part in parts:
        stuff = get_stuff(geo, part[0])
        if not stuff:
            raise ValueError,'No such stuff: "%s"' % part[0]
        print '\twith stuff: (%s)%s %s' % (type(stuff), part[0], str(part[1]))
        mix.AddElement(stuff, part[1])

        continue
    return mix

# Allocate media so we can make sure to provide a unique number.
_media = []
def make_medium(geo, mat, name = None, params = None):
    'Make a medium from a material'

    if name is None: 
        name = mat.GetName()

    med = geo.GetMedium(name)
    if med: return med

    global _media
    _media.append(med)          # let no ID 
    numed = len(_media)         # be zero!

    if params:
        med = ROOT.TGeoMedium(name, numed, mat, params)
    else:
        med = ROOT.TGeoMedium(name, numed, mat)

    ROOT.SetOwnership(med,0)
    print 'Medium: %s #%d' % (name,numed)
    return med

def make_translation(x,y,z):
    tran = ROOT.TGeoTranslation(x,y,z)
    ROOT.SetOwnership(tran,0)
    return tran

def make_rotation(phi,theta,psi):
    rot = ROOT.TGeoRotation()   # avoid having to give a name
    rot.SetAngles(phi,theta,psi) # what new level of hell?
    ROOT.SetOwnership(rot,0)
    return rot


def test():
    print geo
    
if __name__ == '__main__':
    test()

