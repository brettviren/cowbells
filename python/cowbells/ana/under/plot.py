#!/usr/bin/env python
'''
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


from cowbells.ana.run import TreePlotRun
class PlotRun(TreePlotRun):
    
    def run(self):
        print 'Running PlotRun'
        ROOT.gStyle.SetOptStat(1110) # http://root.cern.ch/root/html/TPaveStats.html

        self.cprint("[")
        self.do_prod_energy()
        self.do_nceren()
        self.cprint("]")
        return


    def do_nceren(self):
        '''
        Requires the "stacks" DataRecorder branch was written
        '''
        p = self.p.copy(what = 'stacks.nceren:stacks.energy',
                        cuts = 'stacks.pdgid==%(cut_pdgid)d && stacks.mat==%(material_number)d',
                        title = 'Cerenkov multiplicity for %(cut_particle)s from %(energy)s MeV protons in %(sample)s',
                        xtitle = 'Kinetic energy (MeV)',
                        ytitle = 'Number of optical photons',
                        )

        print p

        self.canvas.SetLogy(False)
        for p.cut_particle, p.cut_pdgid in pdgid2name.iteritems():
            if p.cut_particle in ['op']: 
                continue

            count = self.tree.Draw(p.what, p.cuts, 'colz')
            print '%d "%s" cuts=%s' % (count,p.cut_particle,p.cuts)
            if not count:       # null draw leaves prior plot on canvas
                h = ROOT.TH2F("h","hist",1,0,1,1,0,1)
            else:
                h = self.canvas_get_hist('h')
            self.dress_hist(h, **p.dict())
            h.Draw("colz")
            #self.canvas.Modified()
            #self.canvas.Update()
            self.cprint()
            continue
        return

    def do_prod_energy(self):
        '''
        Requires the "steps" DataRecorder branch was written.
        '''
        p = self.p.copy(what = 'steps.energy1',
                        cuts = 'steps.stepnum==1 && steps.pdgid==%(cut_pdgid)d',
                        title = 'First step of %(cut_particle)s in %(sample)s',
                        xtitle = 'Kinetic Energy (MeV)')

        self.canvas.SetLogy(True)
        for p.cut_particle, p.cut_pdgid in pdgid2name.iteritems():

            self.tree.SetLineColor(4)
            count1 = self.tree.Draw(p.what, p.cuts)

            if not count1: 
                print 'Nothing to plot for "%s" with cuts "%s"' % (p.what,p.cuts)
                continue

            h1 = self.canvas_get_hist('h1')
            self.dress_hist(h1, **p.dict())
            h1.Draw()
            self.canvas.Modified()
            self.canvas.Update()

            ct = c_thresh.get(p.cut_particle)
            print 'Thresh cut for %s is %f' % (p.cut_particle, ct or -1)
            if ct:
                thresh_cut = '%s && energy1 > %f' % (p.cuts, ct)
                self.tree.SetLineColor(2)
                count2 = self.tree.Draw(p.what, thresh_cut, "same")
                frac = 100.*float(count2)/float(count1)
                label = '%s: %s %%%f above threshold' % (self.filename(), p.cut_particle, frac)
                print label

            self.cprint()
            continue
        return
