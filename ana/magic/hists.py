#!/usr/bin/env python

import cowbells
ROOT = cowbells.ROOT
from UserDict import DictMixin
from collections import defaultdict

process_idname = {
    ( 0, 0): 'primary',
    ( 2, 2): 'eioni',
    ( 2, 2): 'hioni',
    ( 2, 3): 'ebrem',
    ( 2, 12): 'phot',
    ( 2, 13): 'compt',
    ( 2, 21): 'cerenkov',
    ( 2, 22): 'scintillation',
    ( 3, 34): 'opwls',
    ( 4, 111): 'hadelastic',
}
process_names = sorted(process_idname.values())

class PerChannel(DictMixin):

    # eg: timing_scintillation_1_0
    name_pat = '{sample}_{energy}_{quant}_{process}_{hcid_name}{volid}'
    title_pat = '{quant} for process {process}, channel {hcid_name}{volid}, in {sample}, Eproton={energy}'
    quants = ['timing', 'charge']
    procs = process_names
    hcids = range(2)
    hcid_names = ['trigger','pmt']
    volids = range(2)
    sample = 'unknown_sample'
    energy = 'unknown_energy'

    def __init__(self, cowbells_tree, **kwds):
        self._tree = cowbells_tree
        self._hists = dict()
        self._params = {k:v for k,v in self.__class__.__dict__.items() \
                        if not k.startswith('_') and not isinstance(v,type(lambda x:x))}
        self._params.update(kwds)

    def format(self, string, **kwds):
        d = dict(self._params)
        d.update(kwds)
        return string.format(**d)

    def __getitem__(self, name):
        if not name.startswith(self.format('{sample}_{energy}_')):
            msg = 'PerChannel mismatch: {wantname} not related to {sample}_{energy}'
            print self.format(msg, wantname=name)
            raise KeyError, name
        if not self._hists:
            self.fill()
        return self._hists[name]

    def configitems(self):
        params = dict(self._params)
        for quant in self._params['quants']:
            params['quant'] = quant
            for process in self._params['procs']:
                params['process'] = process
                for hcid, hcid_name in zip(self._params['hcids'],self._params['hcid_names']):
                    params['hcid'] = hcid
                    params['hcid_name'] = hcid_name
                    for volid in self._params['volids']:
                        params['volid'] = volid
                        yield params
        return

    def keys(self):
        if not self._hists:
            self.fill()
        return self._hists.keys()

    def desc(self, **kwds):
        quant = kwds['quant']
        if quant == 'timing':
            return (200,0,100)
        if quant == 'charge':
            return (200,0,100)
        return None

    def xtitle(self, **kwds):
        quant = kwds['quant']
        if quant == 'timing':
            return "Hit times (ns)"
        if quant == 'charge':
            return "Charge/event (PE)"
        return ""
        

    def get_hist(self, **kwds):
        name = self._params['name_pat'].format(**kwds)
        hist = self._hists.get(name)
        if hist: return hist
        hist = ROOT.TH1F(name, self._params['title_pat'].format(**kwds), *self.desc(**kwds))
        hist.SetXTitle(self.xtitle(**kwds))
        self._hists[name] = hist
        return hist

    def get_process(self, hit):
        ptpst = hit.pType(), hit.pSubType()
        process = process_idname.get(ptpst)
        if process: return process
        return 'unknown_%d_%d' % ptpst

    def fill(self):
        for entry in self._tree:
            charges = defaultdict(int)
            for hit in entry.event.hc:
                hcid, volid = hit.hcId(), hit.volId()
                process = self.get_process(hit)
                d = dict(self._params, hcid=hcid, hcid_name=self._params['hcid_names'][hcid],
                         volid=volid,process=process)
                t_hist = self.get_hist(quant='timing', **d)
                t_hist.Fill(hit.time())
                q_hist = self.get_hist(quant='charge', **d)
                charges[q_hist] += 1
                continue
            for qhist, q in charges.items():
                qhist.Fill(q)


def print_file(rootfile, printfile, sample='water', energy='2gev'):
    import os
    ext = os.path.splitext(printfile)[1][1:]

    ROOT.gStyle.SetOptStat(111111)

    tfile = ROOT.TFile.Open(rootfile)
    ttree = tfile.Get('cowbells')
    pc = PerChannel(ttree, sample=sample, energy=energy)
    canvas = ROOT.TCanvas()
    canvas.Print(printfile + '[', ext)
    for name, hist in sorted(pc.items()):
        canvas.Clear()
        canvas.SetLogy()
        hist.Draw()
        print hist.GetName(), hist.GetEntries()
        canvas.Print(printfile, ext)
    canvas.Print(printfile + ']', ext)

if '__main__' == __name__:
    import sys
    sample, energy, rootfile, printfile = sys.argv[1:]
    print_file(rootfile, printfile, sample, energy)
