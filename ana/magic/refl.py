#!/usr/bin/env python
'''
Make plots for the reflections simulation outputs

'''
import os
import math
import ROOT
import util

from collections import defaultdict

canvas= ROOT.TCanvas("refl","refl",850,1100)

reflectivities = ['0.00', '0.02', '0.05', '0.10', '0.25', '0.50', '1.00']
reflectivities10k = ['0.00', '0.02', '0.05', '0.10']
#reflectivities10k2 = ['0.01', '0.03', '0.06', '0.11']
reflectivities10k2 = ['0.00', '0.001', '0.01', '0.02']
def parameters(**kwds):
    return util.format_dict(dict(
            simname = 'gen-nsrl-reflections',
            runname = '13a',
            sample = 'water',
            data_dir = '/home/bviren/work/wbls/refactor/run/{simname}/repeat',
            tree_file = '{data_dir}/{runname}-{sample}-ref{reflectivity}.root',
            out_dir = 'images/refl',
            **kwds))
# TC's are hcid == 0
# PMTs are hcid == 1
def pass_double_trigger(entry):
    count = defaultdict(int)
    for hit in entry.event.hc:
        count[(hit.hcId(),hit.volId())] += 1
    if count[(0,0)] > 0 and count[(0,1)] > 0:
        return True
    return False


class HistCollection(object):

    def __init__(self, **kwds):
        print kwds
        self.opts = util.format_dict(kwds)
        print self.opts
        from cowbells.ana.util import make_file_tree

        tree_file = self.opts.get('tree_file')
        ft = make_file_tree(tree_file)
        if not ft:
            raise ValueError, "Failed to open %s" % tree_file
        self.file , self.tree = ft

        self.hists = {}
        self.book()

    def book(self):
        return

    def format(self, string, **kwds):
        d = dict(self.opts)
        if self.opts.get('invert'):
            d['invert'] = 'reversed'
        else:
            d['invert'] = 'normal'
        d.update(kwds)
        d = util.format_dict(d)
        try:
            return string.format(**d)
        except KeyError, err:
            print err
            print string
            print kwds
            raise

    def book_one(self, name, title, desc1d, desc2d = None, **kwds):
        name = self.format(name, **kwds)
        title = self.format(title, **kwds)
        if not desc2d:
            h =  ROOT.TH1D(name, title, *desc1d)
        else:
            h =  ROOT.TH2D(name, title, *desc1d+desc2d)
        self.hists[name] = h
        return h
    pass

class UsDsHistCollection(HistCollection):
    '''
    Make various plots for a given tree for both upstream and downstream PMTs
    '''
    to_plot = [('nhits',(200,0,200)),
               ('hittime', (300, 16, 22)),
               ('meantime', (200, 18, 20)),
               ('rmstime', (200,0,4)),
               ('earliest_hit', (200, 16, 20)),
               ('earliest_diff', (200, -2, 2)),
               ]

    out_pattern = 'images/refl/{runname}-{sample}-ref{reflectivity}-{ud}-{invert}.pdf'

    def book(self):
        self._us = {}
        self._ds = {}
        self.start_event(None)
        for variable, desc1d in self.to_plot:
            for ud, UD, hists in [('us','Upstream',self._us), ('ds','Downstream', self._ds)]:
                h = self.book_one('h_{ud}_{variable}', '{UD} {variable} ref={reflectivity}',
                                  desc1d=desc1d, ud=ud, UD=UD, variable=variable)
                hists[variable] = h
                #print 'Booked "%s"' % variable
                continue
            continue
        return

    def start_event(self, event):
        for nam in ('nper','meant','meant2','earliest'):
            self._us[nam] = 0
            self._ds[nam] = 0
    def us(self, hit):
        self.record_hit(self._us, hit)
    def ds(self, hit):
        self.record_hit(self._ds, hit)
    def end_event(self, event): 
        self.tally_event(self._us)
        self.tally_event(self._ds)

        diff  = self._us['earliest'] - self._ds['earliest']
        self._us['earliest_diff'].Fill(diff)
        self._ds['earliest_diff'].Fill(-diff)
        

    def record_hit(self, _ud, hit):
        t = hit.time()
        _ud['nper'] += 1
        _ud['meant'] += t
        _ud['meant2'] += t*t
        _ud['hittime'].Fill(t)
        if _ud['earliest'] == 0:
            _ud['earliest'] = t
        else:
            _ud['earliest'] = min(_ud['earliest'], t)

    def tally_event(self, _ud):
        n = _ud['nper']
        if not n: return
        _ud['nhits'].Fill(n)

        m = _ud['meant'] / n
        m2 = _ud['meant2'] / n
        _ud['meantime'].Fill(m)
        _ud['rmstime'].Fill(math.sqrt(m2 - m*m))
        _ud['earliest_hit'].Fill(_ud['earliest'])

    def get_output(self, **kwds):
        fn = self.format(self.out_pattern, **kwds)
        base,ext = os.path.splitext(fn)
        base = base.replace('.','_')
        return base+ext

    def get_plots(self):
        ret = []
        for ud in ('us','ds'):
            fn = self.get_output(ud=ud)
            if not os.path.exists(fn):
                print 'No file: %s' % fn
                return None
            ret.append(fn)
        return ret


    def plots(self):
        files = self.get_plots()
        if files: return files
        return self.make_plots()

    def fill(self):
        for t in self.tree:
            self.start_event(t)
            if self.opts.get('invert') == pass_double_trigger(t):
                continue
            for hit in t.event.hc:
                if hit.hcId() != 1:
                    continue
                if hit.volId()==0:  # downstream
                    self.ds(hit)
                if hit.volId()==1:  # upstream
                    self.us(hit)
                continue
            self.end_event(t)
            continue
        return 

    def make_plots(self):

        self.fill()

        ret = []
        for ud, _ud in [('us',self._us), ('ds',self._ds)]:
            fname = self.get_output(ud=ud)
            ret.append(fname)

            canvas.Clear()
            canvas.Divide(2,3)
            for ind, nam in enumerate(['nhits','hittime','rmstime','meantime',
                                       'earliest_hit','earliest_diff']):
                pad = canvas.cd(ind+1)
                pad.SetLogy()
                _ud[nam].Draw()
            canvas.Print(fname, 'pdf')
        parts = '\n'.join(['\\includegraphics[width=\\textwidth]{%s}\n'%fn for fn in ret])
        return parts
            


def make_usds(ref='0.10', invert=False, **extra):
    if not isinstance(ref, basestring):
        ref = '%0.2f' % ref
    d = parameters(reflectivity=ref)
    d.update(extra)
    usds = UsDsHistCollection(invert=invert, **d)
    return usds
    
def stats(hc):
    for name, hist in hc.hists.items():
        print '\t%s n=%8d avg=%8.2f rms=%8.4f' % (name, hist.GetEntries(), hist.GetMean(), hist.GetRMS())

def do_all():
    outfp = ROOT.TFile.Open('refl_hists.root','recreate')
    for ref in reflectivities10k2:
        for invert in [True, False]:
            usds = make_usds(ref, invert=invert)
            files = usds.make_plots()
            print 'Refl %s (inverted=%s):' % (ref, invert)
            stats(usds)
            print files
            subdir_name = usds.format('{invert}_{reflectivity}')
            subdir = outfp.mkdir(subdir_name)
            subdir.cd()
            for h in usds.hists.values():
                h.Write()
