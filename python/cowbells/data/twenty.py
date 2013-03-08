#!/usr/bin/env python
'''
Look at twenty repeated FADC cells
'''
import ROOT
from collections import namedtuple

def book(namepat = 'sig%02d', nbins=200, center = 2**13):
    '''
    Return a list of 20 histograms booked as given
    '''
    ret = []
    for count in range(20):
        name = namepat%count
        h = ROOT.TH1I(name,name, nbins, center-nbins/2, center+nbins/2)
        ret.append(h)
    return ret

def fill(hists, signal):
    '''
    Fill "twenty" histograms with signal
    '''
    for count, sig in enumerate(signal):
        ihist = count%20
        h = hists[ihist]
        h.Fill(sig)
    return

def spin(hists, daq, start = 0, nentries = 1000, chn=0):
    '''
    From given start, spin through nentries of a daq tree filling
    twenty hists with signals from chn.
    '''
    for entry in range(start, start+nentries):
        daq.get_entry(entry)
        sig = daq.get('Channel%d'%chn)
        fill(hists, sig)
    return

def limits(hists):
    '''
    Return a list of (min,max) limits of each histogram
    '''
    ret = []
    for h in hists:
        a = h.GetXaxis()
        l = (a.GetXmin(), a.GetXmax())
        ret.append(l)
    return ret

def stats(hists):
    '''
    Return a list of twenty (maximum,average,RMS) tripples.
    '''
    ret = []
    for h in hists:
        p = (h.GetMaximum(), h.GetMean(), h.GetRMS())
        ret.append(p)
    return ret

def fits(hists):
    '''
    Return list of twenty (const,mean,sigma) tripples
    '''
    for h in hists:
        h.GetFunction("")

def fit(hists, limit = None, prime = None):
    '''
    Fit histograms with a gaussian.  

    If twenty limits are given, limit each fit to the given range.

    If twenty primes are given, use them to initialize the fit
    function, otherwise use stats().

    Return list of (const,mean,sigma) tripples
    '''
    if not limit:
        limit = limits(hists)
    if not prime:
        prime = stats(hists)

    ret = []
    for ind,(h,l,p) in enumerate(zip(hists,limit,prime)):
        f = ROOT.TF1('twentyfit','gaus(0)',*l)
        for ipar, par in enumerate(p):
            f.SetParameter(ipar,par)
        h.Fit(f,'L','',*l)
        res = [f.GetParameter(i) for i in range(3)]
        ret.append(res)
    return ret

def draw(canvas, hists, stats=False):
    '''
    Draw the twenty histograms onto the canvas.
    '''
    canvas.Clear()
    canvas.Divide(5,4)
    for count, h in enumerate(hists):
        pad = canvas.cd(count+1)
        pad.SetLogy()
        h.Draw()

    if not stats: 
        return

    canvas.Modified()
    canvas.Update()

    for count, h in enumerate(hists):
        s = h.FindObject("stats")
        s.SetOptStat(1110)
        s.SetOptFit(111)
    return

    
def tuples2arrays(tuples):
    '''
    Convert a list of N-tuples to an N-list of arrays
    '''
    return map(list, zip(*tuples))

def tuples2tgraphs(tuples):
    '''
    Convert a list of N-tuples to an N-list of tgraph
    '''
    ret = []
    for arr in tuples2arrays(tuples):
        t = ROOT.TGraph()
        for c,a in enumerate(arr):
            t.SetPoint(c,c,a)
        ret.append(t)
    return ret

def tuples2hists(tuples, names = None):
    '''
    '''
    arrs = tuples2arrays(tuples)
    if not names:
        names = ['tuple%d'%i for i in range(len(arrs))]

    ret = []
    for name, arr in zip(names, arrs):
        h = ROOT.TH1F(name,name, 100, min(arr), max(arr))
        map(h.Fill,arr)
        ret.append(h)
    return ret
        
            
def draw_graphs(canvas, tgraphs):
    '''
    Draw the list of graphs
    '''
    n = len(tgraphs)
    canvas.Clear()
    canvas.Divide(1,n)
    for c,g in enumerate(tgraphs):
        pad = canvas.cd(c+1)
        g.Draw("AL")
    return
    

SuiteData = namedtuple('Suite','daq canvas h20 fits1 fits2')
def suite(sd):
    '''
    Do a suite of things, return list of products
    '''
    if not sd.canvas:
        sd = sd._replace(canvas = ROOT.TCanvas("canvas","canvas"))

    if not sd.h20:
        sd = sd._replace(h20 = book())
        spin(sd.h20, sd.daq)

    if not sd.fits1:
        sd = sd._replace(fits1 = fit(sd.h20))

    if not sd.fits2:
        limits = [(m-2*s,m+3*s) for c,m,s in sd.fits1]
        sd = sd._replace(fits2 = fit(sd.h20, limit=limits, prime=sd.fits1))

    return sd

