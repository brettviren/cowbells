#!/usr/bin/env python
'''
Utility functions for histograms.
'''

from collections import namedtuple
import ROOT

def draw_stats(pad, h, fitnum =111):
    h.Draw()
    pad.Modified()
    pad.Update()
    stats = h.FindObject("stats")
    if stats:
        stats.SetOptStat(1110)
        stats.SetOptFit(fitnum)
    return


HeightMiddleWidth = namedtuple('HeightMiddleWidth','height middle width')

def mean_avg_rms(hist):
    '''Return the Mean/Average/RMS as a HeightMiddleWidth.'''
    return HeightMiddleWidth(hist.GetMaximum(), hist.GetMean(), hist.GetRMS())

def fit_gaus(hist, prime = None, limit = None, func = None):
    '''
    Fit a Gaussian to histogram, return result as a HeightMiddleWidth
    
    The fit will be primed with the <prime> triple or the
    mean_avg_rms() will be used.

    The <limit> double may be used to limit the fit.
    '''

    a = hist.GetXaxis()
    if not limit:
        limit = (a.GetXmin(), a.GetXmax())
    if not prime:
        prime = mean_avg_rms(hist)

    if func:
        ff = func
    else:
        ff = ROOT.TF1("histutil_fit_gaus","gaus(0)", *limit)
        
    for ipar, par in enumerate(prime):
        ff.SetParameter(ipar,par)

    # L:likelihood, Q:quiet, 0:do not plot, N:do not store function
    hist.Fit(ff, "LQ0F", "", *limit)

    ret = HeightMiddleWidth(*[ff.GetParameter(i) for i in range(3)])
    if not func:
        del (ff)
    return ret

def sig2hist(signal):
    '''
    Return a histogram from the array signal.  
    '''
    n = len(signal)
    h = ROOT.TH1F('sig2hist', 'sig2hist', n, 0, n)
    for ind, val in enumerate(signal):
        h.Fill(ind,val)
    return h


