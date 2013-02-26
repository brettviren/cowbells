#!/usr/bin/env python
'''
Make some plots from the TQ tree.
'''
import ROOT

class Plots(object):
    def __init__(self, tree, canvas = None, pdffile = 'tqplot.pdf'):
        self.tree = tree
        self.pdffile = pdffile
        if not canvas:
            canvas = ROOT.TCanvas("tqtree","tqtree debug", 0,0, 1000, 700)
        self.canvas = canvas

    def cprint(self,extra=''):
        self.canvas.Print('%s%s'%(self.pdffile,extra), 'pdf')

    def do_twoXtwo(self, what, chn=0):
        self.canvas.Clear()
        self.canvas.Divide(2,2)

        for count, what in enumerate(what):
            pad = self.canvas.cd(count+1)
            pad.SetLogy(True)
            self.tree.Draw("%s[%d]"%(what,chn))
        return

    def do_minmax(self, chn=0):
        self.do_twoXtwo(['qmin','qmax','tmin','tmax'], chn)

    def do_stats(self, chn=0):
        self.do_twoXtwo(['avg','mean','rms','sigma'], chn)

    def do_sumn(self, chn=0):
        self.do_twoXtwo(['n3','n4','sum3','sum4'], chn)

    def do_34(self, chn=0, maxq=400, opt="", logy=True, fit=(25,100)):
        self.canvas.Clear()
        self.canvas.Divide(2,2)

        todraw = "n%(nsig)d[%(chn)d]*mean[%(chn)d] -sum%(nsig)d[%(chn)d]"
        for count,nsig in enumerate([3,4]):
            pad = self.canvas.cd(count+1)
            pad.SetLogy(logy)
            self.tree.Draw(todraw%locals(),"",opt)

        for count,nsig in enumerate([3,4]):
            pad = self.canvas.cd(count+3)
            pad.SetLogy(logy)
            h = ROOT.TH1F("spe%d"%nsig,'sum(ADC) >%d sigma above ped'%nsig,maxq,0,maxq)
            ROOT.SetOwnership(h,0) # leak it
            self.tree.Draw(todraw%locals()+">>spe%d"%nsig,"",opt)
            if fit:
                h.Fit("gaus","","", *fit)
                h.Draw()
                pad.Modified()
                pad.Update()
                stats = h.FindObject("stats")
                if stats:
                    stats.SetOptStat(1110)
                    stats.SetOptFit(111)
            continue
        return

    def do_34_50(self, chn=0, opt="", logy=True):
        self.do_34(chn=chn, maxq=50, opt=opt, logy=logy,fit=None)

    def do_34vEntry(self, chn=0):
        self.canvas.Clear()
        self.canvas.Divide(2,2)

        measure = "n%(nsig)d[%(chn)d]*mean[%(chn)d]-sum%(nsig)d[%(chn)d]"

        for count,nsig in enumerate([3,4]):
            pad = self.canvas.cd(count+1)
            m = measure % locals()
            m += ':Entry$'
            c = ""
            print m
            self.tree.Draw(m,c,'colz')

        for count,nsig in enumerate([3,4]):
            pad = self.canvas.cd(count+3)
            m = measure % locals()
            c = "%s > 0 && %s < 400" % (m,m)
            m += ':Entry$'
            print m
            print c
            self.tree.Draw(m,c,'colz')

        return
                               
    def do_fit(self, chn=0):
        self.canvas.Clear()
        self.canvas.Divide(2,2)
        
        toplot = "mean[%(chn)d] sigma[%(chn)d] mean[%(chn)d]:Entry$ sigma[%(chn)d]:Entry$"
        toplot = toplot % locals()
        for count,what in enumerate(toplot.split()):
            pad = self.canvas.cd(count+1)
            opt = ""
            if 'Entry$' in what:
                opt = "COLZ"
            self.tree.Draw(what,"",opt)
            continue
        return

    def all(self, chn = 0):
        self.cprint('[')
        for what in [
            'minmax','stats','fit','sumn',
            '34','34_50', '34vEntry',
            ]:
            meth = getattr(self, 'do_%s' % what)
            meth(chn)
            self.cprint()
        self.cprint(']')

if __name__ == '__main__':
    import sys
    fp = ROOT.TFile.Open(sys.argv[1])
    tree = fp.Get("tq")
    try:
        pdf = sys.argv[2]
    except IndexError:
        pdf = None
    p = Plots(tree, pdffile=pdf)
    p.all()
