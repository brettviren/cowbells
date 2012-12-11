#!/usr/bin/env python
'''
Make some dE/dx plots
'''

import ROOT
import bethe, cbdedx

class PlotStuff:
    titles = {
        'edep': 'Energy Deposition Per Step for %(particle)s in %(material)s',
        'ediff': 'Energy Loss Per Step for %(particle)s in %(material)s',
        }

    def __init__(self, fname, particle=None, material=None):
        self.fname = fname
        self.canvas = ROOT.TCanvas("canvas","dedx",500,400)
        self.cprint('[')


        if particle and material:
            self.plot(particle, material)
        return
    
    def draw(self, hist, bethes):
        hist.Draw("colz,same")
        pfx = hist.ProfileX()
        pfx.Draw("same")

        color = [1,2,4,6,8]     # enough colors for anyone
        for ind,b in enumerate(bethes):
            g = b.graph()
            g.SetLineColor(color[ind%len(color)])
            g.Draw("L")
            continue
        return

    def make(self, particle, material):
        p = cbdedx.test_plotter(particle,material)
        bethes = []
        for tcut in [0.1, 1.0, 10.0]:
            bethes.append(bethe.MixDEDX(material, particle=particle, tcut=tcut))
            continue
        return (p,bethes)

    def plot(self, particle, material):
        '''
        Plot the dedxs
        '''
        self.plots, self.bethes = self.make(particle,material)
        
        for kind in self.plots.kinds():
            tit = self.titles[kind] % locals()
            self.cstart(tit)
            self.draw(self.plots.merged(kind), self.bethes)
            self.cprint()
            continue
        return

    def __del__(self):
        self.cprint(']')
        self.canvas = None
        return

    def cstart(self, title):
        self.frame = self.canvas.DrawFrame(0,0,2500,10)
        self.frame.SetTitle(title)
        self.frame.SetXTitle("Energy (MeV)")
        self.frame.SetYTitle("Energy/step (MeV/cm)")
        return

    def cprint(self, extra=""):
        if not self.canvas: return
        self.canvas.Print(self.fname + extra, 'pdf')
        return

    pass

if __name__ == '__main__':
    ps = PlotStuff('plot_dedx.pdf')
    ps.plot('proton','water')
    ps.plot('mu+','water')
    del (ps)
