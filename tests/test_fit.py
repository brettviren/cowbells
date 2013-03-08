#!/usr/bin/env python
'''
Test root fits of gaussian
'''

import ROOT

canvas = ROOT.TCanvas("c","canvas")
gauss_nbins = 20
gauss_range = (-10,10)


def one_fit(do_fit = True):
    h = ROOT.TH1F('h','h', gauss_nbins, *gauss_range)
    #h.Clear() # <-- segfault if used without SetOwnership() line below
    #ROOT.SetOwnership(h,0)      # leak
    fill = ROOT.TF1("fill2gaus","gaus(0)",*gauss_range)
    fill.SetParameter(0, 1)
    fill.SetParameter(1, 0.25)
    fill.SetParameter(2, 3.0)
    h.FillRandom("fill2gaus")
    avg, rms = [h.GetMean(), h.GetRMS()]
    if do_fit:
        g = ROOT.TF1("fit2gaus","gaus(0)",*gauss_range)
        g.SetParameter(0, 5000)
        g.SetParameter(1, avg)
        g.SetParameter(2, rms)
        h.Fit("fit2gaus","")
        #g = h.GetFunction("gaus")
        h.Draw()
        canvas.Modified()
        canvas.Update()
        fit = [g.GetParameter(i) for i in range(3)]
    else:
        fit = [None]*3
    
    ret = fit + [avg, rms]
    del(h)
    return ret

def tgraph(name,title,color):
    g = ROOT.TGraph()
    g.SetName(name)
    g.SetTitle(title)
    g.SetLineColor(color)
    return g

def test_many_fits(ntries, do_fit = True):
    avg_mean = 0
    avg_sigma = 0

    means1d = ROOT.TH1F("means1d","Fitted means",200, *gauss_range)
    sigmas1d = ROOT.TH1F("sigmas1d","Fitted sigmas",100,0,gauss_range[1])

    means = tgraph("means","Fitted means vs try", 1)
    avgs = tgraph("avgs","Histogram average vs try", 2)

    sigmas = tgraph("sigmas","Fitted sigmas vs try", 1)
    rmses = tgraph("rmses","Histogram RMSs vs try", 2)

    for count in range(ntries):
        c,m,s,a,r = one_fit(do_fit)

        if c is not None:
            avg_mean += m
            means1d.Fill(m)
            means.SetPoint(count,count,m)
            avg_sigma += s
            sigmas1d.Fill(s)
            sigmas.SetPoint(count,count,s)

        avgs.SetPoint(count,count,a)
        rmses.SetPoint(count,count,r)
    print float(avg_mean)/ntries, float(avg_sigma)/ntries
    canvas.Clear()
    canvas.Divide(2,2)

    canvas.cd(1)
    means1d.Draw()
    canvas.cd(2)
    avgs.Draw("AL")
    if do_fit:
        means.Draw("")

    canvas.cd(3)
    sigmas1d.Draw()
    canvas.cd(4)
    rmses.Draw("AL")
    if do_fit:
        sigmas.Draw("")

    if do_fit:
        canvas.Print("test_fit.pdf")
    else:
        canvas.Print("test_hist.pdf")

if __name__ == '__main__':
    import sys
    ntries = 100
    do_fit = True
    try:
        ntries = int(sys.argv[1])
        do_fit = bool(int(sys.argv[2]))
    except IndexError:
        pass
    test_many_fits(ntries, do_fit)

    
    
