#!/usr/bin/env python

from glob import glob
import cowbells
from util import OrgCanvasPrinter
ROOT = cowbells.ROOT
canvas = ROOT.TCanvas()

data_dir = "/home/bviren/work/wbls/refactor/run/gen-nsrl-histat"
input_glob_pat = data_dir+'/13a-{sample}-ref0.02-energy{energy}-seq*.root'


printer = OrgCanvasPrinter(canvas, './images/histat')


def make_chain(sample, energy, limit = None):
    '''
    Make a tree chain from all files matching sample and energy.
    
    If limit given, limit to using the first <limit> files.
    '''
    pat = input_glob_pat.format(**locals())
    tree = ROOT.TChain('cowbells')
    files = sorted(glob(pat))
    if limit:
        files = files[:int(limit)]
    print 'Chaining %d files matching %s' % (len(files), pat)
    for fname in files:
        tree.AddFile(fname)
    return tree


def generate_plots():
    printed = []
    printer.canvas.SetLogy(False)
    for sample in ['Water','WBLS01']:
        for energy in [475, 2000]:
            tree = make_tree(sample, energy)
            h = make_hits_per_event(tree)
            h.Draw("colz")
            printed += printer(h,'%s-%dMeV' % (sample, energy))
            continue
        continue
    return printed

def make_hits_per_event(tree):
    hist = ROOT.TH2D("dsus","Nhits/Event Downstream vs Upstream",
                     100,0,200, 100, 0, 200)
    for t in tree:
        nds = nus = 0
        for hit in t.event.hc:
            if hit.hcId() != 1:
                continue
            if hit.volId()==0:                
                nds += 1
            if hit.volId()==1:
                nus += 1
            continue
        hist.Fill(nds,nus)
        continue
    return hist
