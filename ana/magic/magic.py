#!/usr/bin/env python
                
import os

import cowbells 
ROOT = cowbells.ROOT

from hists import PerChannel
from bv.rootutil.histo import Writer, Reader
from bv.rootutil.printing import MultiPrinter, PrintManager
from bv.collections import ChainedDict

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


def_dir = "/home/bviren/work/wbls/refactor/run/nsrl-13a-wbls"
#def_pat = def_dir + "/nsrl-13a-2gev-protons-{sample}.hits-steps.1k.root"
def_pat = def_dir + "/nsrl-13a-{energy}-protons-{sample}.hits.steps.had.1k.root"
def plots(tree_file_pattern = def_pat, out_file = 'magic.root', opt = 'update', 
          samples = None, energies = None):

    samples = samples or ['water','wbls01']
    energies = energies or ['475mev',  '2gev']

    sources = []

    # Check the root file first for histograms from prior runs
    hist_file = ROOT.TFile.Open(out_file, opt)
    hr = Reader(hist_file)
    sources.append(hr)

    # Fall back to generating the histograms from the tree
    pcs = []
    for energy in energies:
        params = dict(energy=energy)
        for sample in samples:
            params['sample'] = sample
            rootfile = tree_file_pattern.format(**params)
            infile = ROOT.TFile.Open(rootfile)
            ttree = infile.Get('cowbells')
            pc = PerChannel(ttree, **params)
            pcs.append(pc)
            sources.append(pc)
        
    # Write any histograms that may be produced.
    sinks = []
    hw = Writer(hist_file)
    sinks.append(hw)

    # Put it all together
    hists = ChainedDict(source=sources, sink=sinks)


    # This draws the named histogram into the canvas
    def drawer(name, **kwds):
        canvas.Clear()
        try:
            h = hists[name]
        except KeyError, msg:
            print msg
            canvas.SetLogy(False)
            canvas.Range(-1,-1,1,1)
            tt = ROOT.TText()
            tt.DrawText(-1.0,.00,str(msg))
        else:
            canvas.SetLogy()
            h.Draw()

    # Make a single file, multi page PDF and a directory of single file pdfs and pngs
    out_dir = 'magic_plots'
    printers = [
        PrintManager(drawer=drawer, printer=canvas.Print, filenamer = SingleShot(out_dir,ext))
        for ext in ['pdf','png']]
    with MultiPrinter(filename=out_dir+'.pdf', printer = canvas.Print) as printer:
        mp = PrintManager(drawer=drawer, printer = printer, filenamer = shunt)
        printers.append(mp)
        for pc in pcs:
            for params in pc.configitems():
                name = pc.name_pat.format(**params)
                print 'Printing histogram "%s"' % name
                for printer in printers:
                    printer(name)

    # Catch any new histograms that were added
    # fixme: do this with a context manager?
    hist_file.Write()



    
