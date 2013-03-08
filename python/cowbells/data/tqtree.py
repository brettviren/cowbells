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
import peaks
from tree import branch
import histutil

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
    qnpeaks=    ('i',4,'Number of qpeaks found'),

    qpeaks3=    ('f',4,'Sum of bins in peaks above 3 signam from mean'),
    qpeaks4=    ('f',4,'Sum of bins in peaks above 4 signam from mean'),
    qpeaks5=    ('f',4,'Sum of bins in peaks above 5 signam from mean'),

    qwin=       ('f',4,'Sum of bins around highest peak'),

    dpstart=    ('i',4,'Dominant peak starting cell'),
    dploc=      ('i',4,'Dominant peak highest bin location'),
    dpbase=     ('i',4,'Dominant peak width at base'),
    dpheight=   ('f',4,'Dominant peak height above ped in ADC'),
    dpsum=      ('f',4,'Dominant peak sum of ADC'),
    dpconst=    ('f',4,'Dominant peak fit height'),
    dpmean=     ('f',4,'Dominant peak fit mean'),
    dpsigma=    ('f',4,'Dominant peak fit sigma'),

)

class TreeSpinner(object):
    '''
    A WblsDaqTree spinner that fills the tqtree.
    '''

    # Configuration items

    # Apply explicit selection for external LED triggers in channel0,
    # possibly interspersed with cosmic muons by rejecting any with
    # activity in channel 2.
    select_for_insterspersed_leds = True

    # Save all peaks found 
    save_peaks = True

    def __init__(self, tqtree, debug=False):
        self.tqtree = tqtree
        self.obj = branch(tqtree, **tq_desc)
        self.debug = debug
        if self.debug:
            self.canvas = ROOT.TCanvas("tqtree","tqtree debug", 0,0, 1000, 700)
        self.saved_peaks = []

        return

    def write(self):
        return


    def subtract_pedestal(self, sig):
        ret = array('f',len(sig)*[0])
        ped = sig[:20]
        nped = len(ped)
        norm = sum(ped)/float(nped)
        for t,q in enumerate(sig):
            ret[t] = q - (ped[t%20]-norm)
        return ret

    def __call__(self, daq):
        '''
        Fill an entry in our TQ tree from daq.

        This method makes this object a tree spinner.
        '''
        self.clear()
        
        self.obj.trigt[0] = daq.get("TriggerTimeFromRunStart")

        sigs = []
        for chn in range(4):
            sig = daq.get("Channel%d"%chn)
            sig = self.subtract_pedestal(sig)
            self.set_channel(chn, sig)
            sigs.append(sig)

        if self.fail():
            return

        self.fill()
        return 

    def clear(self):
        for field in self.obj:
            for i in range(len(field)):
                field[i] = 0
        return

    def fail(self):
        '''
        Return True if we want to fail this entry (not save it)
        '''
        if self.select_for_insterspersed_leds:
            # channel 2 has no significant charge above ped
            # activity at expected time for external trigger
            ok = self.obj.mean[2]-self.obj.qmin[2]<100 \
                and abs(self.obj.tmin[0]-1530)<30         
            return not ok
        return False

    def fill(self):
        self.tqtree.Fill()

    def set_channel(self, chn, sig):
        minq = min(sig)
        iminq = int(minq+0.5)
        self.obj.qmin[chn] = iminq
        self.obj.tmin[chn] = minbin = sig.index(minq)

        maxq = max(sig)
        imaxq = int(maxq+0.5)
        self.obj.qmax[chn] = imaxq
        self.obj.tmax[chn] = maxinb = sig.index(maxq)


        low,high = 50,150
        start = max(0, minbin - low)
        stop = min(len(sig), minbin + high)
        windowed = sig[:start] + sig[stop:]
        iminq = int(min(windowed)+0.5)

        pwr = ROOT.TH1F('pwr','power', imaxq-iminq+2,iminq-1,imaxq+1)
        map(pwr.Fill, windowed)

        self.obj.avg[chn] = pwr.GetMean()
        self.obj.rms[chn] = pwr.GetRMS()

        # guess-fit the pedestal
        fit = histutil.fit_gaus(pwr)
        const, mean, sigma = fit
        self.obj.const[chn] = const
        self.obj.mean[chn] = mean
        self.obj.sigma[chn] = sigma

        # simple, fixed window around peak
        winlow, winhigh = 10,40
        winstart = max(0, minbin - winlow)
        winstop = min(len(sig), minbin + winhigh)
        winsig = sig[winstart:winstop]
        self.obj.qwin[chn] = mean * len(winsig) - sum(winsig)

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

        # make a positive-going signal w.r.t. fit "pedestal"
        pulse = [mean - s for s in sig] 
        ps = peaks.downhills(pulse, 0.0, 3*sigma)
        
        npeaks = len(ps)
        self.obj.qnpeaks[chn] = npeaks

        if not npeaks:
            return

        # Characterize the dominant peak and transfer the returned
        # PulsedPeak values to the tree.  What a coincidence the two
        # are named so similarly....
        if npeaks == 1:
            pp = peaks.characterize(pulse, *ps[0])
            for field_name in pp._fields:
                val = self.obj.__dict__['dp%s'%field_name]
                val[chn] = pp.__dict__[field_name]

        saved_peaks = []
        for count, (l,r) in enumerate(ps):
            peak = pulse[l:r]
            totq = sum(peak)
            if not count:
                self.obj.qpeak[chn] = totq
                self.obj.qwid[chn] = len(peak)
                saved_peaks.append({'t':l, 'q':peak, 'c':chn})

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
            continue
        if self.save_peaks:
            self.saved_peaks.append(saved_peaks)

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
    ts.select_for_insterspersed_leds = False

    daq.spin(ts)
    ts.write()
    otree.Write()
    ofile.Close()

    if ts.saved_peaks:
        open(peakfile,'w').write(json.dumps(ts.saved_peaks, indent=2))


if __name__ == '__main__':
    import sys
    proc(sys.argv[1], sys.argv[2])
