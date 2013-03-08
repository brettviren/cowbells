#!/usr/bin/env python

import ROOT
import cowbells.data.histutil as histutil
reload(histutil)

def fit_draw(pad, tree, what, cuts, *histspec):
    h = ROOT.TH1F('h', what, *histspec)
    f = ROOT.TF1("f","gaus(0)", *histspec[1:])

    tree.Draw('%s>>h'%what, cuts, 'goff')
    fit = histutil.fit_gaus(h, func=f)
    histutil.draw_stats(pad, h)
    f.Draw("same")
    print fit
    return h,f,fit

def main(filename, canvas = None):

    keep_alive = []

    fp = ROOT.TFile.Open(filename)
    tq = fp.Get("tq")

    if not canvas:
        canvas = ROOT.TCanvas("c","c")

    canvas.Divide(3,3)

    padn = 1
    pad = canvas.cd(padn)
    pad.SetLogy()
    tq.Draw("qnpeaks[0]")
    ROOT.TLine().DrawLine(1,10,1,100)
    ROOT.TLine().DrawLine(2,10,2,100)

    padn = 2
    pad = canvas.cd(padn)
    pad.SetLogy()
    tq.Draw("dpheight[0]","qnpeaks[0]==1")
    ROOT.TLine().DrawLine(10,0.5,10,300)

    cuts = "qnpeaks[0]==1 && dpheight[0]>10"

    padn = 3

    padn += 1
    pad = canvas.cd(padn)
    tq.Draw("dpmean[0]:dpconst[0]",cuts,"colz")

    padn += 1
    pad = canvas.cd(padn)
    tq.Draw("dpsigma[0]:dpconst[0]",cuts,"colz")

    padn += 1
    pad = canvas.cd(padn)
    tq.Draw("dpsigma[0]:dpmean[0]",cuts,"colz")

    padn += 1
    pad = canvas.cd(padn)
    x = fit_draw(pad, tq,"dpconst[0]",cuts, 50,0,50)
    keep_alive.append(x)

    padn += 1
    pad = canvas.cd(padn)
    x = fit_draw(pad, tq,"dpmean[0]",cuts, 50,0,10)
    keep_alive.append(x)

    padn += 1
    pad = canvas.cd(padn)
    x = fit_draw(pad, tq,"dpsigma[0]",cuts, 50,0,2.5)
    keep_alive.append(x)

    canvas.Print("dpsel.pdf")

    return canvas
