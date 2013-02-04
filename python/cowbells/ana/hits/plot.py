#!/usr/bin/env python
'''
Make some hit related plots
'''
import os
import ROOT

from cowbells.ana.util import StringParams
from cowbells.ana.run import TreePlotRun

class PlotRun(TreePlotRun):
    def run(self):
        ROOT.gStyle.SetOptStat(1110) # http://root.cern.ch/root/html/TPaveStats.html

        self.cprint("[")

        self.do_npe()
        self.do_time()

        self.cprint("]")
        return
        
    def do_npe(self):
        p = self.p.copy(what = '@hc.size()',
                        cuts = '',
                        title = 'Number of PE %(energy)s MeV %(particle)s ("%(label)s", %(nevents)s events, %(tub)s tub, %(sample)s sample)',
                        xtitle = 'Number of PE',
                        ytitle = 'Number of Events',
                        )
        self.tree.Draw(p.what,p.cuts)
        h = self.canvas_get_hist('hittime')
        self.dress_hist(h, **p.dict())
        h.Draw()
        self.cprint()
        return


    def do_time(self):
        p = self.p.copy(what = 'hc.t',
                        cuts = 'hc.hcid == 0',
                        title = 'PE times for %(energy)s MeV %(particle)s ("%(label)s", %(nevents)s events, %(tub)s tub, %(sample)s sample)',
                        xtitle = 'time (ns)',
                        ytitle = '',
                        )
        self.tree.Draw(p.what,p.cuts)
        h = self.canvas_get_hist('hittime')
        self.dress_hist(h, **p.dict())
        h.Draw()
        self.cprint()
        return

    pass
