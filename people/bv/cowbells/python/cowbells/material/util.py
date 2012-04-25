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

# Set to module's medium method if generic
def generic_medium():
    'Return the WBLS medium'
    mat = make_mixture("WBLS", parts, density)
    if not mat: return None
    return make_medium(mat)



# Set to module's register method if generic
def generic_register(mc):
    "Register material with MC"

    assert mc, 'Null MC passed'

    med = medium()
    wid = med.GetId()           # Must. Love. The. Interface.

    nentries = len(energy)
    array_type = 'd'
    efficiency = array(array_type,[0.0]*nentries)
    print 'energy:',energy
    print 'abs:',absorption
    print 'eff:',efficiency
    print 'rind:',rindex
    mc.SetCerenkov(wid, nentries, 
                   array(array_type,energy), 
                   array(array_type,absorption),
                   efficiency, 
                   array(array_type,rindex))
                   
    mc.SetMaterialProperty(wid, "RAYLEIGH", nentries, energy, rayleigh)

    # gMC->SetMaterialProperty(fImedWater, 
    #                          "FASTCOMPONENT", nEntries, photonEnergy, scintilFast);
    # gMC->SetMaterialProperty(fImedWater, 
    #                          "SLOWCOMPONENT", nEntries, photonEnergy, scintilSlow);

    # gMC->SetMaterialProperty(fImedWater, "SCINTILLATIONYIELD", 50.e03);  // 50./MeV
    # gMC->SetMaterialProperty(fImedWater, "RESOLUTIONSCALE",  1.0);
    # gMC->SetMaterialProperty(fImedWater, "FASTTIMECONSTANT",  1.0e-09);  // 1.*ns
    # gMC->SetMaterialProperty(fImedWater, "SLOWTIMECONSTANT", 10.0e-09); // 10.*ns
    # gMC->SetMaterialProperty(fImedWater, "YIELDRATIO", 0.8);
    
    return


def test():
    print geo
    
if __name__ == '__main__':
    test()

