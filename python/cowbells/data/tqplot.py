#!/usr/bin/env python
'''
Make some plots from the TQ tree.
'''
import ROOT
import math

def liberate(tobj): 
    ROOT.SetOwnership(tobj,0)
    return tobj

def draw_stats(pad, h, fitnum =111):
    h.Draw()
    pad.Modified()
    pad.Update()
    stats = h.FindObject("stats")
    if stats:
        stats.SetOptStat(1110)
        stats.SetOptFit(fitnum)
    return

class Plots(object):
    expected_led_time = "abs(tmin[%(chn)d]-1530) < 30"

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
            h = liberate(ROOT.TH1F("spe%d"%nsig,'sum(ADC) >%d sigma above ped'%nsig,maxq,0,maxq))
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

    def _fit_pe(self, chn=0, cuts=None,
                  spe=(60,110), dpe=(115,220), tpe=(225,350), qmeas = 'qpeak'):
        '''
        Fit single/double PE peak of qpeak.
        '''
        if cuts is None:
            cuts = self.expected_led_time
        nbins, minq, maxq = 500, 0, 500

        cuts = cuts%locals()
        what = "%(qmeas)s[%(chn)d]"%locals()
        h = liberate(ROOT.TH1F('hqpeak', "%s {%s}" % (qmeas, cuts,), nbins, minq, maxq))
        self.tree.Draw('%s >> hqpeak'%what, cuts)

        pe1 = liberate(h.Clone())
        pe1.Fit("gaus","L","",*spe)
        fit1 = pe1.GetFunction("gaus")
        fit1.SetRange(minq,maxq)
        fit1.SetLineColor(2)
        
        pe2 = liberate(h.Clone())
        pe2.Add(fit1, -1)
        pe2.Fit("gaus","L","",*dpe)
        fit2 =  pe2.GetFunction("gaus")
        fit2.SetRange(spe[0],maxq)
        fit2.SetLineColor(4)

        pe3 = liberate(h.Clone())
        pe3.Add(fit2, -1)
        pe3.Fit("gaus","L","",*tpe)
        fit3 = pe3.GetFunction("gaus")
        #fit3.SetRange(dpe[0],maxq)
        fit3.SetLineColor(6)

        pe123 = liberate(h.Clone())
        dfit = liberate(ROOT.TF1("dfit","gaus(0)+gaus(3)+gaus(6)",10,tpe[1]))

        for ind in range(3):
            dfit.SetParameter(ind,  fit1.GetParameter(ind))
            dfit.SetParameter(ind+3,fit2.GetParameter(ind))
            dfit.SetParameter(ind+6,fit3.GetParameter(ind))
        pe123.Fit(dfit,"L","",10,maxq)
        dfit = pe123.GetFunction("dfit")
        dfit.SetRange(10,maxq)
        dfit.SetLineColor(7)

        self.canvas.Clear()
        self.canvas.Divide(2,2)

        pad = self.canvas.cd(1)
        draw_stats(pad, pe1)

        pad = self.canvas.cd(2)
        draw_stats(pad, pe2)

        pad = self.canvas.cd(3)
        draw_stats(pad, pe3)

        pad = self.canvas.cd(4)
        draw_stats(pad, pe123, 111111111)

        a1 = fit1.Integral(minq,maxq)
        if not a1:
            print 'No fit 1'
            return
        a2 = fit2.Integral(minq,maxq)
        c1 = fit1.GetParameter(0)
        c2 = fit2.GetParameter(0)
        mu1 = fit1.GetParameter(1)
        mu2 = fit2.GetParameter(1)
        mupe = 2.0*a2/a1
        print 'Mean <PE> of source = 2*%.1f/%.1f = %.3f' %(a2,a1,mupe)
        mu2mu1_frac = 0
        if mu1: mu2mu1_frac = mu2/mu1
        print 'Ratio of PE2/PE1: %.1f/%.1f = %.3f (~2?)' % (mu2,mu1,mu2/mu1)
        if mupe > 0:
            print 'Prob 0PE: %.3f' % (math.exp(-1*mupe),)
        return

    def do_pe_fits(self, chn=0, cuts = None):
        for qmeas in ['qpeak','qpeaks3','qpeaks4','qpeaks5','qwin']:
            self._fit_pe(chn=chn,qmeas=qmeas,cuts=cuts)
            self.cprint()
            continue
        return

    def do_interspersed_led_cuts(self):
        # Cuts to select LEDs interspersed with cosmic muon triggers
        self.canvas.Clear()
        self.canvas.Divide(2,2)
        
        pad = self.canvas.cd(1)
        pad.SetLogy(True)
        self.tree.Draw("mean[2]-qmin[2]","mean[2]-qmin[2]<1000")
        
        pad = self.canvas.cd(2)
        pad.SetLogy(True)
        self.tree.Draw("qnpeaks[0]","mean[2]-qmin[2]<100")

        pad = self.canvas.cd(3)
        pad.SetLogy(True)
        self.tree.Draw("tmin[0]","mean[2]-qmin[2]<100 && qnpeaks[0] == 1")

        pad = self.canvas.cd(4)
        pad.SetLogy(False)
        self.tree.Draw("qpeak[0]")


    def all(self, chn = 0):
        self.cprint('[')
        for what in [
            'minmax','stats','fit','sumn',
            '34','34_50', '34vEntry',
            ]:
            meth = getattr(self, 'do_%s' % what)
            meth(chn)
            self.cprint()

        self.do_interspersed_led_cuts()
        self.cprint()

        self.do_pe_fits(chn)

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
