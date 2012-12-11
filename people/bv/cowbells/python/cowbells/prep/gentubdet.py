#!/usr/bin/env python
'''
Generate a ROOT TGeometry file for the NSRL "tub" detector
'''

import cowbells
import properties

hbarc = cowbells.units.clhep_units.hbarc
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
        
        # tub = bucket with lid
        t_radius = 0.5*(p['inner_diameter']+2.0*p['tub_thickness'])
        t_hheight = 0.5*(p['inner_height']+p['tub_thickness']+p['lid_thickness'])
        tubname = 'Tub' + self.params['Tub']
        tub = self.geo.MakeTube(tubname, self.get_med('Tub'), 
                           0.0, t_radius, t_hheight)
        tub.SetVisibility(1)
        tub.SetLineColor(2)
        print 'Geo: tub: r=%f hh=%f' % (t_radius, t_hheight)

        # sample volume
        s_radius = 0.5*(p['inner_diameter'])
        s_hheight = 0.5*(p['inner_height'])
        samname = 'Sample' + self.params['Sample']
        sam = self.geo.MakeTube(samname, self.get_med('Sample'),
                           0.0, s_radius, s_hheight)
        sam.SetVisibility(1)
        sam.SetLineColor(1)
        print 'Geo: sample: r=%f hh=%f' % (s_radius, s_hheight)
        
        # Window, from bottom to top
        win = ROOT.TGeoPcon('Window', 0, 360, 4)
        ROOT.SetOwnership(win,0)
        # win = self.geo.MakePcon('Window', self.get_med('Window'), 0, 360, 4)
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
        # FIXME: this name must be hard-coded into cowbells.cc
        pc = self.geo.MakeTube('TUB_PC', self.get_med('PhotoCathode'),
                          0.0, 0.5*p['photocathode_diameter'],
                          0.5*p['photocathode_thickness'])
        pc.SetVisibility(1)
        pc.SetLineColor(7)

        # put sample in tub
        s_shift = t_hheight - s_hheight - p['tub_thickness']
        samintub = ROOT.TGeoTranslation(0, 0, -1*s_shift)
        ROOT.SetOwnership(samintub,0)
        tub.AddNode(sam, 1, samintub)

        # put PC in window
        pc_shift = p['lid_thickness'] - 0.5*p['photocathode_thickness']
        pcinwin = ROOT.TGeoTranslation(0, 0, pc_shift)
        ROOT.SetOwnership(pcinwin,0)
        win.AddNode(pc, 1, pcinwin)

        # put window in lid
        w_shift = t_hheight - p['lid_thickness']
        wininlid = ROOT.TGeoTranslation(0, 0, w_shift)
        ROOT.SetOwnership(wininlid,0)
        tub.AddNode(win, 1, wininlid)

        self._top = tub
        return tub
    pass

#    known_parameters = ['type', 'model', 'finish', 'first', 'second',
#                        'polish', 'sigmaalpha']

def mkgraph(name):
    g = ROOT.TGraph()    
    ROOT.SetOwnership(g,0)
    g.SetName(name)
    return g

from surfaces import GenericSurface
class TeflonSurface(GenericSurface):
    '''
    Describe the optical surface between the sample and the teflon
    walls.  You must giver the names of the first (sample) and second
    (tub) physical volumes.
    '''
    def __init__(self, first, second, color = 'white'):
        name = color.capitalize() + 'TeflonSurface'
        super(TeflonSurface,self).__init__(name, type='dielectric_metal', 
                                           model="glisur", finish="polished", 
                                           first=first, second=second)

        if color.lower() in ['white']: self.set_white_teflon()
        if color.lower() in ['black']: self.set_black_teflon()

        return


    def set_white_teflon(self):
        data = [(250,0.90),
                (400,0.96),
                (500,0.95),
                (600,0.94),
                (800,0.87)]

        return self.set_surface(data)

    def set_black_teflon(self):
        # these numbers are pulled out of the proverbial butt
        data = [(250,0.02),
                (400,0.02),
                (500,0.02),
                (600,0.02),
                (800,0.02)]
        return self.set_surface(data)


    def set_surface(self, reflectivity_vs_nm):
        '''
        PTFE 8 layers from Fig 10 of,  
        Reflectivity Spectra for Commonly Used Reflectors
        Martin Janecek
        IEEE TRANSACTIONS ON NUCLEAR SCIENCE, VOL. 59, NO. 3, JUNE 2012
        '''
        r = mkgraph("REFLECTIVITY")
        t = mkgraph("TRANSMITTANCE")
        # nm, reflectivity
        data = list(reflectivity_vs_nm) # copy
        data.reverse()
        for nm,ref in data:
            r.SetPoint(r.GetN(), hbarc/nm, ref)
            t.SetPoint(t.GetN(), hbarc/nm, 1-ref)
            continue
        self.add_property_tgraph(r)
        # can turn on transmittance if dielectric_dielectric is used
        #self.add_property_tgraph(t)
        return

    pass

def fill(geo, filename = 'tubdet.root', samplemat='Water', tubmat = 'Teflon'):
    '''
    Fill the given TGeo manager with geometry for the "tub" detector
    given the parameters of the detector.
    '''

    tdb = TubDetBuilder(geo, Sample=samplemat, Tub=tubmat, Lid=tubmat)

    from cowbells.prep import propmods
    for mod in propmods:
        print 'Setting up materials module "%s"' % mod.__name__
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

    tubcolor = {'Teflon':'white', 'Aluminum':'black'}[tubmat]
    print 'Writing teflon color "%s"' % tubcolor
    ts = TeflonSurface('Sample'+samplemat, 'Tub'+tubmat, tubcolor)
    ts.write(filename)

    import os
    gdmlfile = os.path.splitext(filename)[0] + '.gdml'
    geo.Export(gdmlfile)

    return

if __name__ == '__main__':
    import sys
    geo = ROOT.TGeoManager('cowbells_geometry', 
                           'Geometry for COsmic WB(el)LS detector')
    args = tuple([x.lower() for x in sys.argv[1:3]])
    filename = 'tubdet-%s-%s.root' % args
    fill(geo, filename, sys.argv[1], sys.argv[2])
