#!/usr/bin/env python
'''
Eyeball pedestals
'''
import os
import ROOT
    

class Pedestal(object):
    def __init__(self, cell, nominal, chn=0):
        nbins = 2*10*nominal[1]
        xmin = nominal[0] - nbins/2
        xmax = nominal[0] + nbins/2
        self.func = ROOT.TF1('pedfit_ch%d_cell%04d'%(chn,cell), "gaus(0)", xmin, xmax)
        self.hist = ROOT.TH1I('ped_ch%d_cell%04d'%(chn,cell), 'Pedestal for ch#%d cell #%d'%(chn,cell), nbins, xmin, xmax)
        self.hist.SetDirectory(0)
        self.dirty = True

        
    def collect(self, adc):
        self.hist.Fill(adc)
        self.dirty = True

    def fit(self):
        cms = [self.hist.GetMaximum(), self.hist.GetMean(), self.hist.GetRMS()]
        for ind,val in enumerate(cms):
            self.func.SetParameter(ind, val) 
        sig = min(cms[2],2)
        cent = cms[1]+sig
        self.hist.Fit(self.func.GetName(),"L","",cent-3*sig, cent+3*sig)
        self.dirty = False
        return self.func

    def correct(self, adc):
        if self.dirty:
            self.fit()
        return float(adc) - self.func.GetParameter(1)

class Pedestals(object):
    def __init__(self, chn, nominal, npeds = 2560):
        print 'Making pedestals object for channel %d'%chn
        self.peds = []
        for cell in range(npeds):
            self.peds.append(Pedestal(cell, nominal, chn=chn))

    def collect(self, sig):
        for ind, ped in enumerate(self.peds):
            ped.collect(sig[ind])

    def correct(self, sig):
        ret = array('f',sig)
        for ind, ped in enumerate(self.peds):
            ret[ind] = ped.correct(sig[ind])
        return ret

    def write(self):
        for ped in self.peds:
            ped.hist.Write()

class Display(object):
    def __init__(self, pdffile, canvas = None):
        self.pdffile = pdffile
        if not canvas:
            canvas = ROOT.TCanvas("canvas","canvas")
        self.canvas = canvas
        self.cprint("[")

    def finish(self):
        self.cprint("]")

    def cprint(self,extra=""):
        self.canvas.Print(self.pdffile+extra,'pdf')
    
    def pedset(self, pedset):
        gs = map (lambda x: ROOT.TGraph(), range(3))
        for g,n in zip(gs, ['const','mean','sigma']):
            g.SetName(n)
            g.SetTitle(n.capitalize())

        g_const = ROOT.TGraph()
        g_mean = ROOT.TGraph()
        g_sigma = ROOT.TGraph()
        for ind, ped in enumerate(pedset.peds):
            cycle = ind%20
            if cycle == 0:
                self.canvas.Clear()
                self.canvas.Divide(5,4)
            pad = self.canvas.cd(1 + cycle)
            pad.SetLogy()

            h = ped.hist
            f = ped.fit()
            h.Draw()
            #pad.Modified()
            #pad.Update()
            stats = h.FindObject("stats")
            if stats:
                stats.SetOptStat(1110)
                stats.SetOptFit(111)

            for ig, g in enumerate(gs):
                g.SetPoint(ind, ind, f.GetParameter(ig))
            if cycle == 19:
                self.cprint()
            continue

        self.canvas.Clear()
        self.canvas.Divide(1,3)
        for n, g in enumerate(gs):
            pad = self.canvas.cd(n+1)
            g.Draw("AL")
        self.cprint()
        return gs
        return

class TreeSpinner(object):
    nominal_pedestals = (8134, 5)
    def __init__(self):
        self.pedset = [Pedestals(0, nominal = self.nominal_pedestals, npeds=2560)]
        self.ped_graphs = []

    def __call__(self, daq):
        for chn in range(1):
            sig = daq.get("Channel%d"%chn)
            self.pedset[chn].collect(sig)

    def plots(self, filepat):
        for chn in range(1):
            disp = Display(filepat % chn)
            gs = disp.pedset(self.pedset[chn])
            self.ped_graphs += gs
            disp.finish()

    def write(self):
        for peds in self.pedset:
            peds.write()
        for g in self.ped_graphs:
            g.Write()
        
    

def proc(infilename):
    import tree

    pdffile = os.path.splitext(infilename)[0] + '-peds-ch%d.pdf'
    outfile = os.path.splitext(infilename)[0] + '-peds-chN.root'

    daq = tree.WblsDaqTree(infilename)
    ts = TreeSpinner()
    daq.spin(ts, 1000)

    ts.plots(pdffile)
    
    out = ROOT.TFile.Open(outfile,'RECREATE')
    out.cd()
    ts.write()
    out.Close()


if __name__ == '__main__':
    import sys
    proc(sys.argv[1])
