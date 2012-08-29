#!/usr/bin/env python
'''
Generate the NSRL setup

This consists of

|   <>   ()  <>  ()  <>
W   T1   S1  T2  S2  T3

W : window (15 mil = 381 microns thick Aluminum)
Tn: trigger counter #n
Sn: sample detector #n

The world is made of air.  Breath deep.
'''
import cowbells
import properties


inch = cowbells.units.inch
cm = cowbells.units.cm
mm = cowbells.units.mm
meter = cowbells.units.meter

import ROOT


from gentubedet import TubDetBuilder

class TriggerCounterBuilder(object):
    '''
    Build a trigger counter.

    This is simply a square box of scintilator embedded in a sensitive
    detector "photocathode".
    '''
    default_params = {

        ## dimensions:

        # The width transverse to the beam
        'width': 2.0*cm,
        # The depth of the scint in the direction of the beam (Z)
        'depth': 1.0*cm,
        # Thickness of "photocathode" wrapping
        'thickness': 1*mm,

        ## materials:

        'Scintillator':'Scintillator',
        'PhotoCathode': 'Glass',

        };

    def __init__(self, geo, **params):
        self.geo = geo
        self.params = {}
        self._top = None
        self.params.update(TriggerCounterBuilder.default_params)
        self.params.update(params)
        return

    def get_med(self, mat_name):
        '''
        Return a medium for given material name
        '''
        med_name = self.params[mat_name]
        med = geo.GetMedium(med_name)
        if not med:
            raise ValueError, 'Bogus medium name "%s"' % med_name
        return med
    
    def top(self):
        '''
        Build geometry, return top node.
        '''
        if self._top: return self._top

        p = self.params

        hwidth = 0.5*p['width']
        hdepth = 0.5*p['depth']
        thick = p['thickness']

        tc = geo.MakeBox('TriggerCounter', self.get_med('Scintillator'),
                         hwidth, hwidth, hdepth)
        pc.SetVisibility(1)
        pc.SetLineColor(2)

        pc = geo.MakeBox('PC',self.get_med('PhotoCathode'),
                         hwidth+thick, hwidth+thick, hdepth+thick)
        pc.SetVisibility(1)
        pc.SetLineColor(1)

        pc.AddNode(tc,1)

        self_top = pc
        return pc

    pass

def fill(geo, filename = 'nsrldet.root'):
    '''
    Fill the given TGeo manager with geometry for the NSRL setup.
    '''
    s1 = TubDetBuilder(geo)
    s2 = TubDetBuilder(geo,Tub='Aluminum')
    tc = TriggerCounterBuilder(geo)

    from cowbells.prep import propmods
    for mod in propmods:
        mod.materials(geo)
        continue


    air = geo.GetMedium('Air')
    top = geo.MakeBox("Top", air, 10*meter, 10*meter, 10*meter)
    top.AddNode(tdb.top(), 1)
    top.SetVisibility(1)

    geo.SetTopVolume(top)

    fp = ROOT.TFile.Open(filename, "update")
    geo.Write("geometry")
    fp.Close()

    properties.fill(filename)

    import os
    gdmlfile = os.path.splitext(filename)[0] + '.gdml'
    geo.Export(gdmlfile)



if __name__ == '__main__':
    import sys
    geo = ROOT.TGeoManager('cowbells_geometry', 
                           'Geometry for COsmic WB(el)LS detector')
    fill(geo, sys.argv[1])
