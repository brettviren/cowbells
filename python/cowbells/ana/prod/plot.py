import os
import ROOT
from collections import defaultdict

from cowbells.ana.util import StringParams
from cowbells.ana.run import TreePlotRun

pdgids = { 11: 'electron',
           13: 'muon',
           22 : 'gamma',
           2212: 'proton',
           9999: 'all',
           }

class PlotRun(TreePlotRun):
    def run(self):
        print 'Running PlotRun'
        ROOT.gStyle.SetOptStat(111110) # http://root.cern.ch/root/html/TPaveStats.html

        self.cprint("[")

        self.do_op_prod()

        self.cprint("]")
        return

    def do_op_prod(self):

        hists = {}
        for pdg,pname in sorted(pdgids.items()):
            hname = 'op_%s' % pname
            htit = self.p.string('Optical Photons from %s'%pname + ' (%(sample)s sample, %(tub)s tub, %(particle)s parent at %(energy)s MeV, %(nevents)s events)')
            h = ROOT.TH1F(hname, htit, 1000,0,10000)
            h.SetXTitle('number of optical photons')
            hists[pdg] = h
            print h.GetTitle()
            continue

        for entry in self.tree:

            counts = defaultdict(int)
            for stack in entry.event.stacks:
                counts[stack.pdgid] += stack.nceren
                continue
            
            for pdgid, count in sorted(counts.items()):
                h = hists.get(pdgid)
                if h:
                    h.Fill(count)
                continue
            hists.get(9999).Fill(sum(counts.values()))
            continue

        for pdg,h in sorted(hists.items()):
            self.canvas.SetLogy(True)
            h.Draw()
            self.cprint()

            self.canvas.SetLogy(False)
            h.Draw()
            self.cprint()
            continue
        return
