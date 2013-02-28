#!/usr/bin/env python
'''
Produce a DAQ-like tree base on the hits produced by the simulation.
'''

import cowbells
import ROOT
from tree import branch
from collections import defaultdict
from array import array
import random

daq_Header = dict(
    RunNumber=          ('i',1,'Run number'),
    RunStartTime=       ('i',1,'Absolute start time in Unix epoch'),
    IsThisDataFile=     ('i',1,'Zero if MC'),
    is12or14bit=        ('i',1,'Dynamic range of FADC'),
    frequency=          ('i',1,'FADC sample rate'),
    runtype=            ('i',1,'Run type'),
    sampletype=         ('i',1,'Sample type'),
    Channel0Gain=       ('f',1,'Channel 0 gain'),
    Channel1Gain=       ('f',1,'Channel 1 gain'),
    Channel2Gain=       ('f',1,'Channel 2 gain'),
    Channel3Gain=       ('f',1,'Channel 3 gain'),
    Channel0device=     ('i',1,'Channel 0 device'),
    Channel1device=     ('i',1,'Channel 1 device'),
    Channel2device=     ('i',1,'Channel 2 device'),
    Channel3device=     ('i',1,'Channel 3 device'),
    PedestalSubstructedAtRun=('i',1,'Pedestal already subtracted'),
)
daq_Footer = dict(
    TotalEventsNumber=  ('i',1,'Number of triggers'),
    RunStopTime=        ('i',1,'Absolute stop time in Unix epoch'),
)
daq_FADCData = dict(
    Channel0=           ('H',2560,'FADC Channel 0'),
    Channel1=           ('H',2560,'FADC Channel 1'),
    Channel2=           ('H',2560,'FADC Channel 2'),
    Channel3=           ('H',2560,'FADC Channel 3'),
    EventNumber=        ('i',1,'Trigger count'),
    TriggerTimeFromRunStart=('d',1,'Time of trigger since start of run'),
)


def wash_peaks(peaks):
    ret = []
    for peak_set in peaks:
        if len(peak_set) != 1:
            continue
        peak = peak_set[0]
        t = peak['t']
        if abs(t-1525) > 25:
            continue
        ret.append(peak['q'])
    return ret

class SamplingDigitizer:
    '''
    Digitize hits using pre-sampled peak wave forms.  Peaks are of the
    form of a list of tuples holding charge samples in units of FADC
    above a pedestal and the pedestal characterized by mean/sigma.
    '''
    def __init__(self, peaks, ped):
        self.peaks = wash_peaks(peaks)
        self.ped = ped          # (mean, sigma) of pedestal
        return

    def tns_to_fadc_bin(self, tns):
        return int(tns+0.5)

    def apply(self, t, sig):
        'Select a peak and apply it to signal at time t.'
        peak = random.choice(self.peaks)
        ibin = self.tns_to_fadc_bin(t) # bin for time t
        pbin = peak.index(max(peak)) # bin in peak of peak
        start = ibin-pbin+1525       # fixme: when in FADC does the peak arive?
        for count, q in enumerate(peak):
            ind = start+count
            sig[start+count] -= int(q+0.5)
            continue
        for ind in range(2560):
            sig[ind] += int(0.5+random.gauss(*self.ped))
        return

    def __call__(self, global_time, hits):
        'Return a dictionary of signals indexed by (volid,hcid)'
        
        signals = defaultdict(lambda: array('i',[0]*2560))

        for hit in hits:
            sig = signals[(hit.volId(),hit.hcId())]
            self.apply(hit.time(), sig)
            continue
        return signals

    pass

def chidonly_mapper(volid, chid):
    return chid

class DaqTreeMaker(object):
    def __init__(self, cb_tree, digitizer, channel_map = chidonly_mapper):
        '''
        Make the tree DAQ trees in the current file using the cowbells hits tree.

        The header/footer/data trees are available from .h/.f/.d members.
        '''
        self.cb_tree = cb_tree
        self.digitizer = digitizer
        self.channel_map = channel_map

        self.h = ROOT.TTree('Header', 'DAQ header from simulation')
        self.hobj = branch(self.h, **daq_Header)
        self.f = ROOT.TTree('Footer', 'DAQ footer from simulation')
        self.fobj = branch(self.f, **daq_Footer)
        self.d = ROOT.TTree('FADCData', 'DAQ FADC data from simulation')
        self.dobj = branch(self.d, **daq_FADCData)
        
        return

    def __call__(self):

        for count, entry in enumerate(self.cb_tree):
            self.clear_d()
            self.dobj.EventNumber[0] = count

            hc = entry.event.hc
            vtx = entry.event.vtx
            self.dobj.TriggerTimeFromRunStart[0] = ttime = 0.0 if not len(vtx) else vtx[0].t

            triggers = self.digitizer(ttime, hc)
            for (volid,hcid), signal in triggers.iteritems():
                chn = self.channel_map(volid, hcid)
                for ind in range(2560):
                    self.dobj[chn][ind] = signal[ind]
                continue
            self.fill_d()
            continue
        self.write()
        return
    def write(self):
        '''
        Write trees to current file
        '''
        for t in (self.h, self.f, self.d):
            t.Write()
        
    def clear_d(self):
        for thing in self.dobj:
            for ind in range(len(thing)):
                thing[ind] = 0
        return
    def fill_d(self):
        self.d.Fill()

def proc(infilename, peaksfile, outfilename):
    infp = ROOT.TFile.Open(infilename)
    intree = infp.Get("cowbells")
    outfp = ROOT.TFile.Open(outfilename,"RECREATE")
    outfp.cd()

    import json
    peaks = json.loads(open(peaksfile).read())
    ped = (8134.5, 2.19)

    digi = SamplingDigitizer(peaks, ped)
    dtm = DaqTreeMaker(intree, digi)
    dtm()
    outfp.Close()
    return

if __name__ == '__main__':
    import sys
    proc(*sys.argv[1:4])
