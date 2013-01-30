import os
import ROOT

from cowbells.ana.util import StringParams
from cowbells.ana.run import TreePlotRun

class PlotRun(TreePlotRun):
    def run(self):
        print 'Running PlotRun'
        ROOT.gStyle.SetOptStat(1110) # http://root.cern.ch/root/html/TPaveStats.html

        self.cprint("[")

        self.do_energy_boundary('begin',2,2,1) # from Teflon->water
        self.do_energy_boundary('end',1,1,2)   # from water->Teflon
        self.do_edep()

        self.cprint("]")
        return
    
    def do_energy_boundary(self, label='start', enum=2, mat1=2, mat2=1):
        p = self.p.copy(startstop=label,
                        what = 'steps.energy%d'% (enum,),
                        cuts = 'steps.mat1==%d && steps.mat2==%d && steps.parentid==0' % (mat1,mat2),
                        title = 'Gamma energy at %(startstop)s of %(sample)s',
                        xtitle = 'Kinetic energy (MeV)',
                        ytitle = '',
                        )
        self.canvas.SetLogy(True)
        self.tree.Draw(p.what,p.cuts)
        h = self.canvas_get_hist('e%s' % (label,))
        self.dress_hist(h, **p.dict())
        h.Draw()
        print 'Maximum for %s: %f' % (label, h.GetMaximum())
        self.cprint()

    def do_edep(self):

        water_number = 1        # material number
        hist_all = ROOT.TH1F('edep','Total energy Deposition in Water', 130,0,1.3)

        for entry in self.tree:
            steps = entry.event.steps

            energy_dep = 0

            for step in steps:
                if step.pdgid == 20:
                    continue
                if step.mat1 != water_number or step.mat2 != water_number:
                    continue
                energy_dep += step.edep

                continue

            hist_all.Fill(energy_dep)
            continue

        self.canvas.SetLogy(True)
        hist_all.Draw()
        self.cprint()

        self.canvas.SetLogy(False)
        axis = hist_all.GetXaxis()
        axis.SetRangeUser(0.05,1.3)
        hist_all.Draw()
        self.cprint()

        return
