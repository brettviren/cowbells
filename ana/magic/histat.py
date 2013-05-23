#!/usr/bin/env python

import cowbells
ROOT = cowbells.ROOT
from cowbells.ana.util import make_file_chain, move_stats
canvas = ROOT.TCanvas()

data_dir = "/home/bviren/work/wbls/refactor/run/gen-nsrl-histat"
input_glob = data_dir+'/13a-water-ref0.02-seq*.root'

def generate_plots():
    tree = make_file_chain(input_glob)

    h_dsus = ROOT.TH2D("dsus","Nhits/Event Downstream vs Upstream",
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
        h_dsus.Fill(nds,nus)
        continue
    canvas.SetLogy(False)
    h_dsus.Draw("colz")
    printed = []
    for ext in ['pdf','png','svg']:
        outname = './images/histat-dsvus.' + ext
        canvas.Print(outname, ext)
        printed.append(outname)
    return printed
    
