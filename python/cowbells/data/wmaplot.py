#!/usr/bin/env python 

import ROOT


class Plotter(object):
    
    def __init__(self, tfile, canvas = None, pdffile = 'tqplot.pdf'):
        self.tfile = tfile
        self.pdffile = pdffile
        if not canvas:
            canvas = ROOT.TCanvas("wmaplot","Wave Form plots", 0,0, 1000, 700)
        self.canvas = canvas

    def cprint(self,extra=''):
        self.canvas.Print('%s%s'%(self.pdffile,extra), 'pdf')

    def do_avg(self, limits = (0,2560), name="avgw"):
        self.canvas.Clear()
        self.canvas.Divide(2,2)

        for n in range(4):
            pad = self.canvas.cd(n+1)
            hname = '%s%d'% (name,n)
            h = self.tfile.Get(hname)
            if not h:
                raise ValueError, 'Failed to get Histogram "%s" from the canvas.' %hname
            h.SetStats(0)
            h.GetXaxis().SetRangeUser(*limits)
            h.Draw()
        self.cprint()

    def __call__(self):
        self.cprint("[")
        self.do_avg()

        early = (0,100)
        mystery = (200,300)
        muon = (1300,1500)
        led = (1500,1600)
        everything = (0,2560)
        plots = ["avg600s20", "avg600a200", "avg20a20", "avg20","avg20s20","avgw"]

        def do_some(names, limits = everything):
            for name in names:
                self.do_avg(limits, name=name)

        for limits in [everything, early, mystery, muon, led]:
            do_some(plots,limits)

        self.cprint("]")
        return

    pass

if __name__ == '__main__':
    import sys
    p = Plotter(ROOT.TFile.Open(sys.argv[1]), pdffile=sys.argv[2])
    p()

        

    
