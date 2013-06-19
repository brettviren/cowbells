#!/usr/bin/env python
'''
Plots using cowbell's "steps" output module
'''

import os
import ROOT

canvas = ROOT.TCanvas("steps", "steps", 800, 600)

data_dir = "/home/bviren/work/wbls/refactor/run/nsrl-13a-reflections"
default_file = "nsrl-13a-2gev-protons-water-10percent.hits-steps.root"

def make_tree(filename = data_dir + '/' + default_file):
    f = ROOT.TFile.Open(filename)
    return f.Get("cowbells")

def xray(tree, entry = None):
    if entry is None:
        entry = "Entry$ < 10"
    else:
        entry = "Entry$ == %d" % entry
    cut = "abs(steps.z1)<100 && abs(steps.x1)<100 && abs(steps.y1)<100 && %s" % entry
    canvas.Clear()
    canvas.Divide(2,2)

    pad = canvas.cd(1)
    tree.Draw("steps.x1:steps.z1",cut)

    pad = canvas.cd(2)
    tree.Draw("steps.y1:steps.x1:steps.z1",cut)

    pad = canvas.cd(3)
    tree.Draw("steps.y1:steps.z1",cut)

    pad = canvas.cd(4)
    tree.Draw("steps.y1:steps.x1",cut)


def plot_xray(tree, entries, basename = "images/steps/xray-"):
    printed = []
    for entry in entries:
        if entry is None: 
            entry_str = "all"
        else:
            entry_str = str(entry)
        fn = basename + "-%s.png"
        fn = fn % entry_str
        printed.append(fn)
        if os.path.exists(fn):
            print 'File exists, remove to force regen: %s' % fn
            continue
        xray(tree, entry)
        canvas.Print(fn, 'png')
    return printed

