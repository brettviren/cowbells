#!/usr/bin/env python


import os
import math
import cowbells
ROOT = cowbells.ROOT
units = cowbells.units
from collections import namedtuple, defaultdict


def dedx_in_material(steps, material, trackid, zranges):

    results = [dict(enoni=0,eloss=0,pathlen=0) for x in zranges]

    for step in steps:
        if step.trackid != trackid:
            continue
        if step.mat1 != material and step.mat2 != material:
            continue

        for res, zrange in zip(results, zranges):
            if step.z1 > zrange[0] and step.z1 < zrange[1]:
                res['enoni'] += step.enoni
                res['eloss'] += step.energy1 - step.energy2
                res['pathlen'] += step.dist

    return [namedtuple('DedxRes','enoni eloss pathlen')(**res) for res in results]


# hcid=0:triggers, hcid=1:tub1, hcid=2:tub2
def passcut(entry):
    count = defaultdict(int)
    for hit in entry.event.hc:
        count[(hit.hcId(),hit.volId())] += 1
    if count[(0,0)] > 0 and count[(0,1)] > 0:
        return True
    return False
    

def plot_dedx(tree):
    dedxhists = namedtuple('dedxhists', 'totdedx totde totdx')
    zranges = [(-70,70),(330,470)]
    titles = ['first tub','second tub']
    hists = []
    
    for tit in titles:
        h = dedxhists(
            totdedx = ROOT.TH1F('totaldedx', 'dE/dX, ' + tit, 1000, 0, 20),
            totde = ROOT.TH1F('totalde', 'Total dE, ' + tit, 2000, 0, 200),
            totdx = ROOT.TH1F('totaldx', 'Total dX, ' + tit, 1000, 0, 20))
        hists.append(h)
        h.totdedx.SetXTitle('dE/dX (MeV/cm)')
        h.totde.SetXTitle('total dE (MeV)')
        h.totdx.SetXTitle('total dX (cm)')

    for entry in tree:
        
        if not passcut(entry):
            continue

        for hist, res in zip(hists, dedx_in_material(entry.event.steps, 1, 1, zranges)):

            eloss_mev = res.eloss/units.MeV
            pathlen_cm = res.pathlen/units.cm
    
            hist.totde.Fill(eloss_mev)
            hist.totdx.Fill(pathlen_cm)
            if pathlen_cm == 0:
                continue
            dedx = eloss_mev / pathlen_cm
            hist.totdedx.Fill(dedx)

    return hists

def mean_error(h):
    h.Fit('gaus')
    avg = h.GetMean()
    rms = h.GetRMS()
    f = h.GetFunction('gaus')
    return (avg,rms),(f.GetParameter(1), f.GetParError(1))

def summary(filename):
    run,sample,energy,particle,totevts = os.path.splitext(filename)[0].split('-')
    fp = ROOT.TFile.Open(filename)
    ret = []
    for cycle in range(1,3):
        ret.append((
            fp.Get('totaldedx;%d'%cycle).GetEntries(),
            mean_error(fp.Get('totalde;%d'%cycle)),
            mean_error(fp.Get('totaldx;%d'%cycle)),
            mean_error(fp.Get('totaldedx;%d'%cycle))
            ))
    return ret

def format_arms(ar,ms):
    names = ['avg','fit']
    return ' '.join(['%s: %6.2f(%7.4f)'%(n,x,y) for n,(x,y) in zip(names,(ar,ms))])
    
def format_tub_summary(tubn, nevents, *rest):
    names = ['   dE','   dX','dE/dX']
    res = ['Tub#%d, %d events:' % (tubn, nevents)]
    for name, (ar,ms) in zip(names, rest):
        res.append('\t%s: %s' % (name, format_arms(ar,ms)))
    return '\n'.join(res)
    
def main_summary(*args):
    res = {}
    for histfile in args:
        r = summary(histfile)
        res[histfile] = r
    for k,v in res.items():
        print 'Summary for', k
        for itub, tub in enumerate(v):
            print format_tub_summary(itub+1, *tub)

def main_run(*args):
    rootfile = args[0]
    outname = args[1]
    #datadir='/home/bviren/work/wbls/refactor/run/tubs'
    tfile = ROOT.TFile.Open(rootfile)
    tree = tfile.Get("cowbells")
    hists = plot_dedx(tree)

    pdffile = outname + '.pdf'
    outroot = outname + '.root'

    canvas = ROOT.TCanvas()
    canvas.SetLogy()
    canvas.Print(pdffile+'[','pdf')
    
    outfile = ROOT.TFile.Open(outroot,'recreate')
    outfile.cd()

    for hc in hists:
        for h in hc:
            h.Draw()
            canvas.Print(pdffile,'pdf')
            h.Write()

    canvas.Print(pdffile+']','pdf')
    
if '__main__' == __name__:
    import sys
    cmd = sys.argv[1]
    meth = eval('main_%s' % cmd)
    meth(*sys.argv[2:])

