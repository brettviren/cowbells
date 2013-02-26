#!/usr/bin/env python
'''
A TQ (Time/Charge) tree from a wblsdaq tree.

This module can also be run as:

  tqtree daqfile.root tqtree.root

It will also produce a JSON file containing the peaks found in each
trigger.
'''

import os
import json
import ROOT
from array import array
from collections import namedtuple
import peaks

def branch(tree, **fields):
    '''
    Branch a given tree with the given fields.  The fields are keyword
    arguments specifying (typecode, length, description) triples.

    Return a namedtuple representing the fields and holding an object
    that provides the branch memory.
    '''
    names = sorted(fields.keys())    
    values = []
    for name in names:
        typecode, length, title = fields[name]

        initval = 0 if typecode.lower() == 'i' else 0.0
        val = array(typecode.lower(),length * [initval])
        values.append(val)

        s = "" if length == 1 else "[%d]"%length
        desc = "%s%s/%s" % (name, s, typecode.upper())
        branch = tree.Branch(name,val,desc)
        branch.SetTitle(title)
    return namedtuple(tree.GetName(), names)(*values)

tq_desc = dict(
    trigt=      ('f',1,'Trigger time from run start'),

    qmin=       ('i',4,'Minimum of all FADC bins'),
    qmax=       ('i',4,'Maxium of all FADC bins'),
    tmin=       ('i',4,'Bin of highest ADC'),
    tmax=       ('i',4,'Bin of highest ADC'),

    avg=        ('f',4,'Average of ADC in trigger'),
    rms=        ('f',4,'RMS of ADC in trigger'),
    const=      ('f',4,'Constant of Gaussian fit'),
    mean=       ('f',4,'Mean of Gaussian fit'),
    sigma=      ('f',4,'Sigma of Gaussian fit'),

    n3=         ('i',4,'Number of bins with charge above 3 sigma from mean'),
    n4=         ('i',4,'Number of bins with charge above 4 sigma from mean'),
    n5=         ('i',4,'Number of bins with charge above 5 sigma from mean'),

    sum3=       ('f',4,'Sum of charge above 3 sigma from mean'),
    sum4=       ('f',4,'Sum of charge above 4 sigma from mean'),
    sum5=       ('f',4,'Sum of charge above 5 sigma from mean'),

    qpeak=      ('f',4,'Sum of bins in highest peak'),
    qwid=       ('f',4,'Number of bins in highest peak'),

    qpeaks3=    ('f',4,'Sum of bins in peaks above 3 signam from mean'),
    qpeaks4=    ('f',4,'Sum of bins in peaks above 4 signam from mean'),
    qpeaks5=    ('f',4,'Sum of bins in peaks above 5 signam from mean'),
)

class TreeSpinner(object):
    '''
    A WblsDaqTree spinner that fills the tqtree.
    '''

    def __init__(self, tqtree, debug=False):
        self.tqtree = tqtree
        self.obj = branch(tqtree, **tq_desc)
        self.debug = debug
        if self.debug:
            self.canvas = ROOT.TCanvas("tqtree","tqtree debug", 0,0, 1000, 700)
        self.saved_peaks = []
        return

    def __call__(self, daq):
        '''
        Fill an entry in our TQ tree from daq.

        This method makes this object a tree spinner.
        '''
        self.clear()
        
        self.obj.trigt[0] = daq.get("TriggerTimeFromRunStart")

        for chn in range(4):
            sig = daq.get("Channel%d"%chn)
            self.set_channel(chn, sig)

        self.fill()
        return 

    def clear(self):
        for field in self.obj:
            for i in range(len(field)):
                field[i] = 0
        return

    def fill(self):
        self.tqtree.Fill()
    
    def set_channel(self, chn, sig):
        minq = min(sig)
        self.obj.qmin[chn] = minq
        self.obj.tmin[chn] = minbin = sig.index(minq)

        maxq = max(sig)
        self.obj.qmax[chn] = maxq
        self.obj.tmax[chn] = maxinb = sig.index(maxq)


        low,high = 50,150
        minbin = sig.index(minq)
        start = max(0, minbin - low)
        stop = min(len(sig), minbin + high)
        windowed = sig[:start] + sig[stop:]
        minq = min(windowed)

        pwr = ROOT.TH1F('pwr','power', maxq-minq+2,minq-1,maxq+1)
        map(pwr.Fill, windowed)

        self.obj.avg[chn] = pwr.GetMean()
        self.obj.rms[chn] = pwr.GetRMS()

        if self.debug:
            pwr.Fit("gaus","")
            self.canvas.Modified()
            self.canvas.Update()
        else:
            pwr.Fit("gaus","Q0")
        
        fit = pwr.GetFunction("gaus")
        self.obj.const[chn] = const = fit.GetParameter(0)
        self.obj.mean[chn] = mean = fit.GetParameter(1)
        self.obj.sigma[chn] = sigma = fit.GetParameter(2)

        #if not chn:
        #    print 'Fit:',mean, sigma, const

        for c in sig[start:stop]:
            if c > mean - 3*sigma:
                continue
            self.obj.n3[chn] += 1
            self.obj.sum3[chn] += c
            if c > mean - 4*sigma:
                continue
            self.obj.n4[chn] += 1
            self.obj.sum4[chn] += c
            if c > mean - 5*sigma:
                continue
            self.obj.n5[chn] += 1
            self.obj.sum5[chn] += c
            continue

        # make positive pulse with mean pedestal at 0.
        pulse = [mean - s for s in sig] 
        ps = peaks.downhills(pulse, 0.0, 3*sigma)
        for count, (l,r) in enumerate(ps):
            peak = pulse[l:r]
            totq = sum(peak)
            if not count:
                self.obj.qpeak[chn] = totq
                self.obj.qwid[chn] = len(peak)
                self.saved_peaks.append({'t':l, 'q':peak})

            maxq = max(pulse[l:r])
            if maxq < 3*sigma:
                continue        # shouldn't happen
            self.obj.qpeaks3[chn] += totq
            if maxq < 4*sigma:
                continue
            self.obj.qpeaks4[chn] += totq
            if maxq < 5*sigma:
                continue
            self.obj.qpeaks5[chn] += totq
            
        return

def make(filename, name="tq", title=None):
    tfile = ROOT.TFile.Open(filename,"recreate")
    ttree = ROOT.TTree(name,title or "TQ tree from %s"%filename)
    return tfile,ttree

def proc(infilename, outfilename, peakfile = None):
    '''
    Produce a tqtree.
    '''

    if not peakfile:
        peakfile = os.path.splitext(outfilename)[0] + '-peaks.json'

    import tree
    daq = tree.WblsDaqTree(infilename)

    ofile, otree = make(outfilename)
    ts = TreeSpinner(otree)
    daq.spin(ts)
    otree.Write()
    ofile.Close()

    open(peakfile,'w').write(json.dumps(ts.saved_peaks, indent=2))


if __name__ == '__main__':
    import sys
    proc(sys.argv[1], sys.argv[2])
