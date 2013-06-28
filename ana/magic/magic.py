#!/usr/bin/env python
                
import os

import cowbells 
ROOT = cowbells.ROOT

from hists import DefaultParams, PerChannel, StepDisplay, DeDxPlots
from bv.rootutil.histo import Writer, Reader
from bv.rootutil.printing import MultiPrinter, PrintManager
from bv.collections import ChainedDict
from collections import defaultdict

canvas = ROOT.TCanvas()

def shunt(name, **kwds):
    return name

class SingleShot(object):
    def __init__(self, outdir, ext = 'pdf'):
        self.outdir = outdir
        self.ext = ext

    def __call__(self, name, **kwds):
        if not os.path.exists(self.outdir):
            os.makedirs(self.outdir)
        return os.path.join(self.outdir, name + '.' + self.ext)

def quiet_print(filename, ext=''):
    ext = ext or os.path.splitext(filename)[1][1:]
    old = ROOT.gErrorIgnoreLevel
    ROOT.gErrorIgnoreLevel = 1999
    canvas.Print(filename, ext)
    ROOT.gErrorIgnoreLevel = old

def maybe_print(filename, ext=''):
    if os.path.exists(filename):
        print 'Not reprinting %s' % filename
    quiet_print(filename, ext)


class Plots(DefaultParams):
    
    # fixme: extract the object_defaults()/val() pattern used here and
    # in the histogram sources into a mixin


    data_dir = "/home/bviren/work/wbls/refactor/run/nsrl-13a-wbls"
    tree_file_pattern = data_dir + "/nsrl-13a-{energy}-protons-{sample}.hits.steps.had.1k.root"
    samples = ['water','wbls01']
    energies = ['475mev',  '2gev']
    opt = 'update'
    out_dir = 'magic_plots'
    out_root = 'magic_plots.root'

    hist_sources = [StepDisplay, PerChannel, DeDxPlots]

    def __init__(self, **kwds):
        self.initialize_params(**kwds)

        hist_sources = self.val('hist_sources')
        if not hist_sources: 
            msg = 'Given no histogram sources!  Why do you bother creating me?'
            raise ValueError, msg

        self.logy = True
        sources = []
        self.hist_source_objects = defaultdict(list)

        # Check the root file first for histograms from prior runs
        self.hist_file = ROOT.TFile.Open(self.val('out_root'), self.val('opt'))
        assert self.hist_file, self.format('Unable to open {out_root} with option "{opt}"')
        hr = Reader(self.hist_file)
        sources.append(hr)

        for energy in self.val('energies'):
            params = dict(energy=energy)
            for sample in self.val('samples'):
                params['sample'] = sample
                rootfile = self.val('tree_file_pattern').format(**params)
                infile = ROOT.TFile.Open(rootfile)
                if not infile:
                    raise ValueError, 'No such file: %s' % rootfile
                ttree = infile.Get('cowbells')

                for HistSource in hist_sources:
                    hs_obj = HistSource(ttree, **params)
                    self.hist_source_objects[HistSource.__name__].append(hs_obj)
                    sources.append(hs_obj)

        # Write any histograms that may be produced.
        sinks = []
        hw = Writer(self.hist_file)
        sinks.append(hw)

        # Put it all together
        self.hists = ChainedDict(source=sources, sink=sinks)
        return


    def drawer(self, name, **kwds):
        'draws the named histogram into the canvas'
        canvas.Clear()
        try:
            h = self.hists[name]
        except KeyError, msg:
            canvas.SetLogy(False)
            canvas.Range(-1,-1,1,1)
            tt = ROOT.TText()
            tt.DrawText(-1.0,.00,str(msg))
        else:
            canvas.SetLogy(self.logy)
            h.Draw()
        return

    def plots(self):

        print_mgrs = []         # filled below
        
        def print_sources(sources):
            if not sources: 
                return
            for src in sources:
                self.logy = src.val('logy')
                for name in src.keys():
                    for pm in print_mgrs:
                        pm(name)
            return


        out_dir = self.val('out_dir')
        for ext in ['pdf','png']:
            pm = PrintManager(drawer=self.drawer, printer=maybe_print, 
                              filenamer = SingleShot(out_dir,ext))
            print_mgrs.append(pm)

        with MultiPrinter(filename=out_dir+'.pdf', printer = quiet_print) as printer:
            mp = PrintManager(drawer=self.drawer, printer = printer, filenamer = shunt)
            print_mgrs.append(mp)

            for hs_name, hs_objects in self.hist_source_objects.items():
                print_sources(hs_objects)

        # Catch any new histograms that were added
        # fixme: do this with a context manager?
        self.hist_file.Write()




