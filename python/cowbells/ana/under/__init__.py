#!/usr/bin/env python
'''
Extract info about things that are under Chernkov threshold
'''

import os
import ROOT

from cowbells.ana.util import StringParams

pdgid2name = {'proton':2212,
              'op':20,
              'electron':11,
              'gamma':22}

c_thresh = {'proton': 480.0,
          'muon': 53.0,
          'electron': 0.262}


def get_tree(filename, treename = 'cowbells'):
    f = ROOT.TFile.Open(filename)
    if not f: return
    return f.Get(treename)

class Plotter:
    def __init__(self, outpdf = None):
        self.set_output(outpdf)
        self.canvas = ROOT.TCanvas("canvas","canvas")
        self.canvas.SetLogy()
        ROOT.gStyle.SetOptStat(1110) # http://root.cern.ch/root/html/TPaveStats.html
        return

    def set_output(self, outpdf):
        self._outpdf = outpdf

    def printer(self, extra=""):
        if not self._outpdf: return
        self.canvas.Print(self._outpdf+extra,'pdf')

    def do_prod_energy(self, filename, treename = 'cowbells'):
        self.set_output(os.path.splitext(filename)[0] + '.pdf')

        tree = get_tree(filename)

        self.printer("[")

        p = StringParams(what = 'energy1',
                         cuts = 'stepnum==1 && pdgid==%(pdgid)d',
                         title = 'First step of %(pname)s in water',
                         xtitle = 'Kinetic Energy (MeV)')

        for p.pname, p.pdgid in pdgid2name.iteritems():

            tree.SetLineColor(4)
            count1 = tree.Draw(p.what, p.cuts)

            if not count1: 
                print 'Nothing to plot for "%s" with cuts "%s"' % (p.what,p.cuts)
                continue

            h1 = self.canvas_get_hist('h1')
            self.dress_hist(h1, **p.dict())
            h1.Draw()
            self.canvas.Modified()
            self.canvas.Update()

            ct = c_thresh.get(p.pname)
            print 'Thresh cut for %s is %f' % (p.pname, ct or -1)
            if ct:
                thresh_cut = '%s && energy1 > %f' % (p.cuts, ct)
                tree.SetLineColor(2)
                count2 = tree.Draw(p.what, thresh_cut, "same")
                frac = 100.*float(count2)/float(count1)
                label = '%s: %s %%%f above threshold' % (filename, p.pname, frac)
                print label

            self.printer()
        self.printer("]")

    def canvas_get_hist(self, newname, oldname = 'htemp'):
        h = self.canvas.GetPrimitive(oldname)
        if not h: 
            raise RuntimeError, 'Failed to get histogram "%s"' % oldname
        return h.Clone(newname)

    def dress_hist(self, hist, **params):
        p = StringParams(**params)
        hist.SetTitle(p.get('title', hist.GetTitle()))
        hist.SetXTitle(p.get('xtitle', hist.GetXaxis().GetTitle()))
        hist.SetYTitle(p.get('ytitle', hist.GetYaxis().GetTitle()))
        hist.SetLineColor(int(p.get('color',hist.GetLineColor())))
        return hist

    

if __name__ == '__main__':
    import sys
    import cowbells
    plotter = Plotter()
    for filename in sys.argv[1:]:
        plotter.do_prod_energy(filename)

