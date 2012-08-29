#!/usr/bin/env python
'''
Generate a ROOT TGeometry file for the NSRL "tub" detector
'''

import cowbells
import properties


inch = cowbells.units.inch
meter = cowbells.units.meter

import ROOT


class TubDetBuilder(object):
    '''
    Build a "tub" detector geometry like used at the NSRL beam test.

    This detector is a simple cylinder, open at the top with a wall
    and bottom thickness.  An inner diameter and height specify the
    sample volume.  On top is a lid of a given thickness with an
    embeded and stepped window.  The PMT photocathode is modeled by
    embedding a disk of PC material just inside the top surface of the
    window.

    '''

    # numbers taken from Harry's Rev02 drawing sent to the mailing
    # list
    # https://lists.bnl.gov/mailman/private/wbls-l/attachments/20120524/a0af1b4f/attachment-0003.bin
    default_params = {
        # Inner diameter of sample volume
        'inner_diameter': (6.0-2*0.25)*inch,
        # Inner height of sample volume
        'inner_height': 6.0*inch,
        # Thickness of wall and bottom
        'tub_thickness': 0.25*inch,
        # Thickness of lid
        'lid_thickness': 0.75*inch,
        # Fraction of distance from outside of window to step relative
        # to lid/window thickness.
        'step_fraction': 2.0/3.0,
        # Diameters of window before and after step
        'window_full_diameter': 2.750*inch,
        'window_step_diameter': 2.125*inch,
        # Photocathode diameter and thickness
        'photocathode_diameter': 2.0*inch,
        'photocathode_thickness': 0.01*inch,

        # materials:
        'Tub' : 'Teflon',
        'Lid' : 'Teflon',
        'Window' : 'Acrylic',
        'PhotoCathode': 'Acrylic',
        #'Sample': 'Water',
        'Sample': 'WBLS',
        'World': 'Air',
        }


    def __init__(self, geo, **params):
        self.geo = geo
        self.params = {}
        self._top = None
        self.params.update(TubDetBuilder.default_params)
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
        
        # bucket with lid
        b_radius = 0.5*(p['inner_diameter']+2.0*p['tub_thickness'])
        b_hheight = 0.5*(p['inner_height']+p['tub_thickness']+p['lid_thickness'])
        tub = geo.MakeTube('Bucket', self.get_med('Tub'), 
                           0.0, b_radius, b_hheight)
        tub.SetVisibility(1)
        tub.SetLineColor(2)
        print 'Geo: bucket: r=%f hh=%f' % (b_radius, b_hheight)

        # sample volume
        s_radius = 0.5*(p['inner_diameter'])
        s_hheight = 0.5*(p['inner_height'])
        sam = geo.MakeTube('Sample', self.get_med('Sample'),
                           0.0, s_radius, s_hheight)
        sam.SetVisibility(1)
        sam.SetLineColor(1)
        print 'Geo: sample: r=%f hh=%f' % (s_radius, s_hheight)
        
        # Window, from bottom to top
        win = ROOT.TGeoPcon('Window', 0, 360, 4)
        ROOT.SetOwnership(win,0)
        # win = geo.MakePcon('Window', self.get_med('Window'), 0, 360, 4)
        r_full = 0.5*p['window_full_diameter']
        r_step = 0.5*p['window_step_diameter']
        z_full = p['lid_thickness']
        z_step = z_full * (1-p['step_fraction'])
        win.DefineSection(0,    0.0, 0.0, r_step)
        win.DefineSection(1, z_step, 0.0, r_step)
        win.DefineSection(2, z_step, 0.0, r_full)
        win.DefineSection(3, z_full, 0.0, r_full)
        win = ROOT.TGeoVolume('Window', win, self.get_med('Window'))
        ROOT.SetOwnership(win,0)
        win.SetVisibility(1)
        win.SetLineColor(4)
        
        # wafer of PC
        pc = geo.MakeTube('PC', self.get_med('PhotoCathode'),
                          0.0, 0.5*p['photocathode_diameter'],
                          0.5*p['photocathode_thickness'])
        pc.SetVisibility(1)
        pc.SetLineColor(7)

        # put sample in bucket
        s_shift = b_hheight - s_hheight - p['tub_thickness']
        samintub = ROOT.TGeoTranslation(0, 0, -1*s_shift)
        ROOT.SetOwnership(samintub,0)
        tub.AddNode(sam, 1, samintub)

        # put PC in window
        pc_shift = p['lid_thickness'] - 0.5*p['photocathode_thickness']
        pcinwin = ROOT.TGeoTranslation(0, 0, pc_shift)
        ROOT.SetOwnership(pcinwin,0)
        win.AddNode(pc, 1, pcinwin)

        # put window in lid
        w_shift = b_hheight - p['lid_thickness']
        wininlid = ROOT.TGeoTranslation(0, 0, w_shift)
        ROOT.SetOwnership(wininlid,0)
        tub.AddNode(win, 1, wininlid)

        self._top = tub
        return tub
    pass

def fill(geo, filename = 'tubdet.root'):
    '''
    Fill the given TGeo manager with geometry for the "tub" detector
    given the parameters of the detector.
    '''

    tdb = TubDetBuilder(geo)

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

    return

if __name__ == '__main__':
    import sys
    geo = ROOT.TGeoManager('cowbells_geometry', 
                           'Geometry for COsmic WB(el)LS detector')
    fill(geo, sys.argv[1])
