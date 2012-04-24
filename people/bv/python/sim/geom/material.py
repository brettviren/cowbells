#!/usr/bin/env python
'''
Get materials
'''

import ROOT

def make_mixture(name, parts, density):
    'Make a mixture'

    geo = ROOT.gGeoManager

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
    geo = ROOT.gGeoManager

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
    return med


def init():
    'Called on import'

    print 'Initializing materials'

    acrylic_parts = [ ("Carbon", 0.59984), ("Hydrogen", 0.08055), ("Oxygen", 0.31961) ]
    make_mixture("Acrylic",acrylic_parts,1.18)

    wbls_parts = [('Hydrogen',0.659),('Oxygen',0.309),('Sulfur',0.0009),
                  ('Nitrogen', 0.000058),('Carbon', 0.031),]
    make_mixture('WBLS', wbls_parts, 0.9945)

    return

if __name__ == '__main__':
    from sim import geo

    import water
    water.register(ROOT.gMC)

    
