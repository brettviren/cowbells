#!/usr/bin/env python

import cowbells
ROOT = cowbells.ROOT
units = cowbells.units

from UserDict import DictMixin
from collections import defaultdict

process_idname = {
    (): 'all',
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
def process_id(name):
    for k,v in process_idname.items():
        if v == name: return k
    return

def object_defaults(obj):
    return {k:v for k,v in obj.__class__.__dict__.items() \
                if not k.startswith('_') and not isinstance(v,type(lambda x:x))}    


def dedx_in_material(steps, material, trackid = 1):
    enoni = eloss = pathlen = 0
    for step in steps:
        if step.trackid != trackid: 
            continue
        if step.mat1 != material and step.mat2 != material:
            continue
        enoni +=  step.enoni
        eloss += step.energy1 - step.energy2
        pathlen += step.dist
    return (pathlen, eloss, enoni)
    return

def hits_to_channel(hits):
    ret = defaultdict(int)
    for hit in hits:
        hv = (hit.hcId(), hit.volId())
        ret[hv] += 1
    return ret

class DefaultParams(object):
    
    def initialize_params(self, **kwds):
        self._params = {k:v for k,v in self.__class__.__dict__.items() \
                            if not k.startswith('_') and not isinstance(v,type(lambda x:x))}
        self._params.update(kwds)

    def val(self, name):
        return self._params[name]
        
    def format(self, string, **kwds):
        'Format a string using parameter values and overriding keyword arguments'
        d = dict(self._params)
        d.update(kwds)
        return string.format(**d)


class BaseHistogramSource(DictMixin, DefaultParams):
    '''
    Base for a dictionary-like object which is a source of histograms.

    See individual method docstrings for subclassing protocol.

    Any class variables provide defaults which can be overridden by
    keyword arguments to the constructor and are thereafter accessible
    via the .val() method.
    '''

    # require these passed as arguments to constructor
    sample = 'unknown_sample'
    energy = 'unknown_energy'

    logy = True
    drawopt = ""

    # set if the pad should be put in to log(y) mode

    sample2material = {
        'water': 1,
        'acrylic': 8,
        'wbls01': 9,
        }

    def __init__(self, cowbells_tree, sample, energy, **kwds):
        '''
        Base normally should not override, but must call if does.
        '''
        self._tree = cowbells_tree
        self._hists = dict()
        self.initialize_params(energy=energy, sample=sample, **kwds)
        self.val('logy')

    def __getitem__(self, name):
        '''
        Subclass should provide.  Return the histogram corresponding to the name.
        '''
        return self._hists[name]

    def __setitem__(self, name, value):
        '''
        Subclass may provide in order to intercept explicit settings of histograms
        '''
        self._hists[name] = value

    def keys(self):
        '''
        Sub class should provide in order to give a sequence of names
        the subclass can make histograms for.
        '''
        return self._hists.keys()

    def get_by_kwds(self, **kwds):
        'Return a histogram by name after formatting'
        name = self.get_name(**kwds)
        hist = self[name]
        return hist

    def get_name(self, **kwds):
        'Return name of histogram based on name_pat parameter'
        return self.format(self.val('name_pat'), **kwds)

    def get_title(self, **kwds):
        'Return title of histogram based on title_pat parameter'
        return self.format(self.val('title_pat'), **kwds)

    def get_material_number(self, **kwds):
        'Return the material number for the sample'
        sample = self.val('sample').lower()
        return self.sample2material[sample]


# TC's are hcid == 0
# PMTs are hcid == 1
def pass_double_trigger(entry):
    count = defaultdict(int)
    for hit in entry.event.hc:
        count[(hit.hcId(),hit.volId())] += 1
    if count[(0,0)] > 0 and count[(0,1)] > 0:
        return True
    return False

class PerChannel(BaseHistogramSource):
    '''
    Make some timing/charge plots based on readout and physics channels.
    '''

    name_pat = '{sample}_{energy}_{quant}_{process}_{hcid_name}{volid}'
    title_pat = '{quant} for process {process}, channel {hcid_name}{volid}, in {sample}, Eproton={energy}'
    quants = ['timing', 'charge']
    procs = process_names
    hcids = range(2)
    hcid_names = ['trigger','pmt']
    volids = range(2)

    logy = True
    drawopt = ""

    def __getitem__(self, name):
        if not name.startswith(self.format('{sample}_{energy}_')):
            msg = 'PerChannel mismatch: {wantname} not related to {sample}_{energy}'
            msg = self.format(msg, wantname=name)
            #print msg
            raise KeyError, msg
        if not self._hists:
            self.fill()
        return self._hists[name]

    def configitems(self):
        params = dict(self._params)
        for quant in self.val('quants'):
            params['quant'] = quant
            for process in self.val('procs'):
                params['process'] = process
                for hcid, hcid_name in zip(self.val('hcids'),self.val('hcid_names')):
                    params['hcid'] = hcid
                    params['hcid_name'] = hcid_name
                    for volid in self.val('volids'):
                        params['volid'] = volid
                        yield params
        return

    def keys(self):
        return [self.get_name(**p) for p in self.configitems()]

    def desc(self, **kwds):
        quant = kwds['quant']
        if quant == 'timing':
            return (200,0,100)
        if quant == 'charge':
            return (100,0,100)
        return None

    def xtitle(self, **kwds):
        quant = kwds['quant']
        if quant == 'timing':
            return "Hit times (ns)"
        if quant == 'charge':
            return "Charge/event (PE)"
        return ""
        

    def get_hist(self, **kwds):
        name = self.get_name(**kwds)
        hist = self._hists.get(name)
        if hist: return hist
        hist = ROOT.TH1F(name, self.get_title(**kwds), *self.desc(**kwds))
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
            if not pass_double_trigger(entry):
                continue
            charges = defaultdict(int)
            for hit in entry.event.hc:
                hcid, volid = hit.hcId(), hit.volId()
                process = self.get_process(hit)
                d = dict(self._params, hcid=hcid, hcid_name=self.val('hcid_names')[hcid],
                         volid=volid,process=process)
                t_hist = self.get_hist(quant='timing', **d)
                t_hist.Fill(hit.time())
                q_hist = self.get_hist(quant='charge', **d)
                charges[q_hist] += 1

                d['process'] = 'all'
                t_hist = self.get_hist(quant='timing', **d)
                t_hist.Fill(hit.time())
                q_hist = self.get_hist(quant='charge', **d)
                charges[q_hist] += 1
                continue
            for qhist, q in charges.items():
                qhist.Fill(q)


class DeDxPlots(BaseHistogramSource):
    '''
    Make some plots about dE/dX
    '''

    name_pat = '{sample}_{energy}_dedx_{quant}'
    title_pat = 'PE/MeV energy loss in {quant} for {energy} protons in {sample}'
    totdedx_title_pat = 'Total dE/dX (MeV/cm) for {energy} protons in {sample}'
    totde_title_pat = 'Total dE (MeV) for {energy} protons in {sample}'
    totdx_title_pat = 'Total dX (cm) for {energy} protons in {sample}'
    
    hcids = range(2)
    hcid_names = ['trigger','pmt']
    volids = range(2)

    logy = True
    drawopt = ""

    def __getitem__(self, name):
        prefix = self.format('{sample}_{energy}_dedx_')
        if not name.startswith(prefix):
            msg = 'Mismatch in DeDxPlots with "{name}", want "{prefix}"'\
                .format(name=name,prefix=prefix)
            raise KeyError, msg
        if not self._hists:
            self.book()
            self.fill()
        return self._hists[name]

    def channels(self):
        for hcid_name in self.val('hcid_names'):
            for volid in self.val('volids'):
                yield '%s%d' % (hcid_name, volid)

    def keys(self):
        ret = [self.get_name(quant=q) for q in ['totaldedx','totalde','totaldx']]
        for quant in self.channels():
            ret.append(self.get_name(quant=quant))
        return ret

    def make_hist(self, quant, hist_desc, title = None):
        name = self.get_name(quant=quant)
        title = title or self.get_title(quant=quant)
        print 'name="%s", title="%s", desc="%s"' % ( name, title, str(hist_desc))
        h = ROOT.TH1F(name, title, *hist_desc)
        self._hists[name] = h
        return h

    def book(self):
        self.h_totdedx = self.make_hist('totaldedx', (1000, 0, 10), 
                                        title = self.format(self.totdedx_title_pat))
        self.h_totde = self.make_hist('totalde', (1000, 0, 100), 
                                        title = self.format(self.totde_title_pat))
        self.h_totdx = self.make_hist('totaldx', (1000, 0, 20), 
                                        title = self.format(self.totdx_title_pat))
        self.h_chan = {}
        for hcid,hcid_name in zip(self.val('hcids'), self.val('hcid_names')):
            for volid in self.val('volids'):
                quant = '%s%d' % (hcid_name, volid)
                h = self.make_hist(quant, (200,0,2))
                self.h_chan[(hcid,volid)] = h

    def fill(self):
        for entry in self._tree:
            pathlen, eloss, enoni = \
                dedx_in_material(entry.event.steps, self.get_material_number())

            eloss_mev = eloss/units.MeV
            pathlen_cm = pathlen/units.cm

            self.h_totde.Fill(eloss_mev)
            self.h_totdx.Fill(pathlen_cm)

            if pathlen == 0:
                continue
            dedx = eloss_mev / pathlen_cm
            self.h_totdedx.Fill(dedx)

            if eloss == 0.0:
                continue
            hits = hits_to_channel(entry.event.hc)
            for hv, pe in hits.items():
                if pe == 0: continue
                if not (hv[0] in self.val('hcids') and hv[1] in self.val('volids')):
                    print 'Unknown hv: %s with %d PE' % (str(hv), pe)
                    continue
                self.h_chan[hv].Fill(pe/eloss_mev)

            


class StepDisplay(BaseHistogramSource):
    '''
    Request a step display by name, name as in StepDisplay.name_pat.

    evtdesc is of form: "<eventnumber>" or "<firstevent>-<lastevent>"

    process can be "all" or a process name

    '''


    name_pat = '{sample}_{energy}_stepdisp_{process}_event{evtdesc}_{view}'
    title_pat = '{view} steps for process {process}, in {sample}, Eproton={energy}, event: {evtdesc}'

    logy = False
    drawopt = "COLZ"
        
    def __getitem__(self, name):
        want = self.format('{sample}_{energy}_stepdisp_')
        if not name.startswith(want):
            msg = 'StepDisplay mismatch: {wantname} not starting with {want}'
            msg = self.format(msg, wantname=name, want=want)
            #print msg
            raise KeyError, msg
        h = self._hists.get(name)
        if h: return h

        h = self.make_hist(name)
        self._hists[name] = h
        print 'StepDisplay: hit on "%s"' % h.GetName()
        return h
        
    def keys(self):
        '''
        This source is somewhat open ended in terms of what "evtdesc",
        "process" and "view" can be accepted so these keys are only
        "suggestions".
        '''
        for evtdesc in ['0','1-10','0-1000']:
            for process in  ['all']:
                for view in ['zx','zy','xy']:
                    name = self.get_name(evtdesc=evtdesc, process=process, view=view)
                    yield name


    def make_hist(self, name, hist_desc = (2000,-100,100)):
        sample, energy, stepdist, process, evtdesc, view = name.split('_')
        print 'Making hist for:', sample, energy, stepdist, process, evtdesc, view

        params = dict(self._params, sample=sample, energy=energy, 
                      process=process, evtdesc=evtdesc, view=view)
        
        event = evtdesc[len('event'):]
        if '-' in event:
            start,stop = event.split('-')
            ecut = 'Entry$ >= {start} && Entry$ <= {stop}'.format(start=start,stop=stop)
        else:
            ecut = 'Entry$ == {event}'.format(event=event)

        process = process.lower()
        ptpst = process_id(process)
        if process == 'all':
            pcut = '1'
        elif ptpst:
            pt,pst = ptpst
            pcut = 'steps.proctype == {pt} && steps.procsubtype == {pst}'.format(pt=pt,pst=pst)
        else:
            raise ValueError,'Unknown process: %s' % process

        dimcut = 'abs(steps.x1) < 100 && abs(steps.y1) < 100 && abs(steps.z1) << 100'
        cut = ' && '.join([ecut, pcut, dimcut])

        title = self.get_title(**params)
        what = ['steps.%s1'%c for c in view]
        toplot = ':'.join(what)
        toplot += ' >> ' + name
        desc = hist_desc+hist_desc
        print name, title, toplot, type(desc), desc
        h = ROOT.TH2F(name, title, *desc)
        self._tree.Draw(toplot, cut, 'colz')
        return h



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
