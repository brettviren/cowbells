#!/usr/bin/env python
'''
Make plots for the reflections simulation outputs

'''
import os
import math
import ROOT
import util

canvas= ROOT.TCanvas("refl","refl",850,1100)

reflectivities = ['0.00', '0.02', '0.05', '0.10', '0.25', '0.50', '1.00']
def parameters(**kwds):
    return util.format_dict(dict(
            simname = 'gen-nsrl-reflections',
            runname = '13a',
            sample = 'water',
            data_dir = '/home/bviren/work/wbls/refactor/run/{simname}',
            tree_file = '{data_dir}/{runname}-{sample}-ref{reflectivity}.root',
            out_dir = 'images/refl',
            **kwds))

class HistCollection(object):

    def __init__(self, **kwds):
        self.opts = util.format_dict(kwds)
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
               ('hittime', (200, 18, 22)),
               ('meantime', (200, 18, 20)),
               ('rmstime', (200,0,4)),
               ('earliest_hit', (200, 16, 20)),
               ('earliest_diff', (200, -2, 2)),
               ]

    out_pattern = 'images/refl/{runname}-{sample}-ref{reflectivity}-{ud}.pdf'

    def book(self):
        self._us = {}
        self._ds = {}
        self.start_event(None)
        for variable, desc1d in self.to_plot:
            for ud, UD, hists in [('us','Upstream',self._us), ('ud','Downstream', self._ds)]:
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

    def make_plots(self):

        updown_hit_visit(self.tree,self)

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
            

def updown_hit_visit(tree, visitor):
    for t in tree:
        visitor.start_event(t)
        for hit in t.event.hc:
            if hit.hcId() != 1:
                continue
            if hit.volId()==0:  # downstream
                visitor.ds(hit)
            if hit.volId()==1:  # upstream
                visitor.us(hit)
            continue
        visitor.end_event(t)
        continue
    return visitor

def make_usds(ref='0.10'):
    if not isinstance(ref, basestring):
        ref = '%0.2f' % ref
    d = parameters(reflectivity=ref)
    usds = UsDsHistCollection(**d)
    return usds
