#!/usr/bin/env python
'''
Generate the NSRL setup

This consists of

|   <>   ()  <>  ()  <>
W   T1   S1  T2  S2  T3

W : beam window (15 mil = 381 microns thick Aluminum)
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


from gentubdet import TubDetBuilder, TeflonSurface

class BeamWindowBuilder(object):
    '''
    Build the beam window.
    '''
    default_params = {
        ## dimensions

        'radius': 2*cm,         # made up number
        'thickness': 0.381*mm,  # from TN05-001
        
        ## materials:

        'BeamWindow': 'Aluminum',
        }

    def __init__(self, geo, **params):
        self.geo = geo
        self.params = {}
        self._top = None
        self.params.update(BeamWindowBuilder.default_params)
        self.params.update(params)
        return

    def get_med(self, mat_name):
        '''
        Return a medium for given material name
        '''
        med_name = self.params[mat_name]
        med = self.geo.GetMedium(med_name)
        if not med:
            raise ValueError, 'Bogus medium name "%s"' % med_name
        return med

    def top(self):
        '''
        Build geometry, return top node.
        '''
        if self._top: return self._top

        p = self.params

        rad = p['radius']
        thick = p['thickness']

        win = self.geo.MakeTube('BeamWindow', self.get_med('BeamWindow'),
                                0.0, rad, 0.5*thick)
        win.SetVisibility(1)
        win.SetLineColor(2)
        self._top = win
        return win

class TriggerCounterBuilder(object):
    '''
    Build a trigger counter.

    This is simply a square box of scintilator embedded in a sensitive
    detector "photocathode".
    '''
    default_params = {

        'pc_name' : 'TC_PC',

        ## dimensions:

        # The width transverse to the beam
        'width': 2.0*cm,
        # The depth of the scint in the direction of the beam (Z)
        'depth': 0.5*cm,
        # Thickness of "photocathode" wrapping
        'thickness': 1*mm,

        ## materials:

        'Scintillator': 'Scintillator',
        'PhotoCathode': 'Glass',

        }

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
        tc.SetVisibility(1)
        tc.SetLineColor(2)

        # fixme: this name must be currently hard-coded into cowbells.cc
        pc = geo.MakeBox(p['pc_name'],self.get_med('PhotoCathode'),
                         hwidth+thick, hwidth+thick, hdepth+thick)
        pc.SetVisibility(1)
        pc.SetLineColor(1)

        pc.AddNode(tc,1)

        self_top = pc
        return pc

    pass




def fill(geo, filename = 'nsrldet.root', samplemat = 'Water'):
    '''
    Fill the given TGeo manager with geometry for the NSRL setup.

    The first tub is at the origin.
    '''
    bw = BeamWindowBuilder(geo)
    s1 = TubDetBuilder(geo, Sample=samplemat, Tub='Teflon',   Lid='Teflon')
    s2 = TubDetBuilder(geo, Sample=samplemat, Tub='Aluminum', Lid='Aluminum')
    tc = TriggerCounterBuilder(geo)

    from cowbells.prep import propmods
    for mod in propmods:
        mod.materials(geo)
        continue


    air = geo.GetMedium('Air')
    top = geo.MakeBox("Top", air, 10*meter, 10*meter, 10*meter)
    top.SetVisibility(1)

    geo.SetTopVolume(top)

    winshift = ROOT.TGeoTranslation(0, 0, -5*meter)

    # spacing between centers of neighboring hodoscope or tubdet
    spacing = 20*cm
    tc1shift = ROOT.TGeoTranslation(0, 0, -1*spacing)
    sd1shift = ROOT.TGeoTranslation(0, 0,     0.0*cm)
    tc2shift = ROOT.TGeoTranslation(0, 0, +1*spacing)
    sd2shift = ROOT.TGeoTranslation(0, 0, +2*spacing)    
    tc3shift = ROOT.TGeoTranslation(0, 0, +3*spacing)

    sdrot = ROOT.TGeoRotation()
    sdrot.RotateY(90)
    sd1tran = ROOT.TGeoCombiTrans(sd1shift, sdrot)
    sd2tran = ROOT.TGeoCombiTrans(sd2shift, sdrot)
    for t in [sdrot,sd1shift,sd2shift]: ROOT.SetOwnership(t,0)

    for shift, builder, copynum in [(winshift,bw,1),
                                    (tc1shift,tc,1),
                                    (sd1tran, s1,1),
                                    (tc2shift,tc,2),
                                    (sd2tran, s2,1),
                                    (tc3shift,tc,3)]:
        ROOT.SetOwnership(shift,0)
        obj = builder.top()
        top.AddNode(obj, copynum, shift)
        continue

    fp = ROOT.TFile.Open(filename, "update")
    geo.Write("geometry")
    fp.Close()

    properties.fill(filename)

    for tubmat, tubcolor in [('Teflon','white'), ('Aluminum','black')]:
        print 'Writing teflon color "%s"' % tubcolor
        ts = TeflonSurface('Sample'+samplemat, 'Tub'+tubmat, tubcolor)
        ts.write(filename)

    #import os
    #gdmlfile = os.path.splitext(filename)[0] + '.gdml'
    #geo.Export(gdmlfile)



if __name__ == '__main__':
    import sys
    geo = ROOT.TGeoManager('cowbells_geometry', 
                           'Geometry for COsmic WB(el)LS detector')
    sample = sys.argv[1]
    filename = 'nsrldet-%s.root' % sample.lower()
    fill(geo, filename, sample)
