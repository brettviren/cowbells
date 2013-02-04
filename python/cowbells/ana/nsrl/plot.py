import os
import ROOT
from collections import defaultdict

from cowbells.ana.util import StringParams
from cowbells.ana.run import TreePlotRun


        

class PlotRun(TreePlotRun):
    def run(self):
        print 'Running PlotRun'
        ROOT.gStyle.SetOptStat(111110) # http://root.cern.ch/root/html/TPaveStats.html

        self.cprint("[")

        self.do_plot()

        self.cprint("]")
        return

    def do_plot(self):
        ht1 = ROOT.TH1F("ht1","Proton energy in T1",
                       2100,0,2100)
        ht2 = ROOT.TH1F("ht2","Proton energy in T2",
                        2100,0,2100)
        for entry in self.tree:
            steps = entry.event.steps
            
            for step in steps:
                if step.parentid > 0:
                    continue
                if step.mat1==2 and step.mat2==1: # teflon->water
                    ht1.Fill(step.energy2)
                elif step.mat1==4 and step.mat2==1: # aluminum->water
                    ht2.Fill(step.energy2)
                continue
            continue
        
        self.canvas.SetLogy(True)

        ht1.Draw()
        self.cprint()

        ht2.Draw()
        self.cprint()

        return

    pass

