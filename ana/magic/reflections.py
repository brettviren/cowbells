#!/usr/bin/env python
'''
Make plots for the reflections simulation outputs

'''


# Local things
data_dir = "/home/bviren/work/wbls/refactor/run/gen-nsrl-reflections"
ref_tree_pat = data_dir + "/13a-water-ref{reflectivity}.root"
output_dir = './images'
ref_strings = ['0.00', '0.02', '0.05', '0.10', '0.25', '0.50', '1.00']
#ref_strings = ['0.00', '1.00']
ref_floats = map(float,ref_strings)
ref_percent = map(lambda x: int(100*x), ref_floats)


import math
import cowbells
ROOT = cowbells.ROOT
from cowbells.ana.util import make_file_tree, move_stats
canvas = ROOT.TCanvas()

from util import OrgCanvasPrinter
prefix_base = 'images/reflections'
printer = OrgCanvasPrinter(canvas, prefix_base)

def generate_plots(which = 'hits', plotter = plot_hits_per_event):
    printed = []

    for refstr, percent in zip(ref_strings, ref_percent):
        rootfile = ref_tree_pat % refstr
        file, tree = make_file_tree (rootfile)
        printer.set_prefix(prefix_base + '-13a-water-ref'+refstr)
        label = '%d percent' % percent
        printed += plot_hits_per_event(tree, label, printer)
        continue
    return printed

def generate_plots_timing():
    printed = []

    for refstr, percent in zip(ref_strings, ref_percent):
        rootfile = ref_tree_pat % refstr
        file, tree = make_file_tree (rootfile)
        printer.set_prefix(prefix_base + '-timing-13a-water-ref'+refstr)
        label = '%d percent' % percent
        printed += plot_timing_per_event(tree, label, printer)
        continue
    return printed
    

def format_latex():
    lines = []
    # these strings must match what is used during plotting
    types = ['dsandus', 'dsvus'] 
    for refstr, percent in zip(ref_strings, ref_percent):
        #printer.set_outbase(refstr)
        for which in types:
            name = printer.outname(which)
            line = r'\includegraphics[width=0.49\textwidth]{%s.pdf}%%' % name
            lines.append(line)
        lines.append('')     # gain newline to break up pairs of plots
    return '\n'.join(lines)
def format_latex_timing():
    lines = []
    # these strings must match what is used during plotting
    types = ['timing'] 
    for refstr, percent in zip(ref_strings, ref_percent):
        #printer.set_outbase(refstr)
        for which in types:
            name = printer.outname(which+ '-13a-water-ref%s' % refstr.replace('.','_'))
            line = r'\includegraphics[width=0.49\textwidth]{%s.pdf}%%' % name
            lines.append(line)
        lines.append('')     # gain newline to break up pairs of plots
    return '\n'.join(lines)

def format_org(which = 'dsandus'):
    lines = []
    # these strings must match what is used during plotting
    for refstr, percent in zip(ref_strings, ref_percent):
        name = printer.outname(which + '-13a-water-ref%s' % refstr.replace('.','_'))
        line = '[[%s.svg]]' % name
        lines.append(line)
    return '\n'.join(lines)


def spit_org_figures(figcap, flavor='latex'):
    'Give a list of (filename,caption) tuples, return string flavored syntax'
    ret = []
    
    if flavor.lower() in ['latex']:
        for fig,cap in figcap:
            ret.append(r'\includegraphics[width=0.8\textwidth]{%s.pdf}' % fig)
            ret.append(r'\begin{center}%s\end{center}'% cap)
            ret.append('')

    if flavor.lower() in ['org']:
        for fig,cap in figcap:
            ret.append('[[%s.svg]]' % fig)
            ret.append(cap)
            ret.append('')

    return '\n'.join(ret)


def hist_timing_per_event(tree, label = '', bindesc=(200,18,20)):
    '''
    Return double-triple of ds/us * (all hit times, mean hit times, rms hit times)
    '''

    h_ds_hit_times = ROOT.TH1D("ds_hit_times","Downstream hit times %s" % label,
                               *bindesc)
    h_us_hit_times = ROOT.TH1D("us_hit_times","Upstream hit times %s" % label,
                               *bindesc)
    h_ds_mean_times = ROOT.TH1D("ds_mean_times","Downstream mean hit times %s" % label,
                               *bindesc)
    h_us_mean_times = ROOT.TH1D("us_mean_times","Upstream mean hit times %s" % label,
                               *bindesc)
    h_ds_rms_times = ROOT.TH1D("ds_rms_times","Downstream rms hit times %s" % label,
                               200,0,2)
    h_us_rms_times = ROOT.TH1D("us_rms_times","Upstream rms hit times %s" % label,
                               200,0,2)

    for t in tree:
        us_n = ds_n = 0
        us_s = ds_s = 0
        us_s2 = ds_s2 = 0

        for hit in t.event.hc:
            if hit.hcId() != 1:
                continue
            t =  hit.time()
            if hit.volId()==0:  # upstream
                us_n += 1
                us_s += t
                us_s2 += t*t
                h_us_hit_times.Fill(t)
            if hit.volId()==1:  # downstream
                ds_n += 1
                ds_s += t
                ds_s2 += t*t
                h_ds_hit_times.Fill(t)
            continue
        if us_n:
            m = us_s / us_n
            h_us_mean_times.Fill(m)
            h_us_rms_times.Fill(math.sqrt(us_s2/us_n - m*m))
        if ds_n:
            m = ds_s / ds_n
            h_ds_mean_times.Fill(m)
            h_ds_rms_times.Fill(math.sqrt(ds_s2/ds_n - m*m))
        continue

    return ((h_ds_hit_times,h_ds_mean_times,h_ds_rms_times),
            (h_us_hit_times,h_us_mean_times,h_us_rms_times))


def hist_hits_per_event(tree, label = '', bindesc=(100,0,200)):
    '''
    Return triple of hits per event (pmt0, pmt1, pmt1 vs pmt0)
    '''
    h_dsus = ROOT.TH2D("dsus","Nhits/Event Downstream vs Upstream %s" % label,
                       *bindesc+bindesc)
    h_dsus.SetXTitle('Upstream Hits Per Event')
    h_dsus.SetYTitle('Downstream Hits Per Event')

    h_ds = ROOT.TH1D("ds","Nhits/Event Downstream %s" % label, *bindesc)
    h_ds.SetXTitle('Hits per event')

    h_us = ROOT.TH1D("us","Nhits/Event Upstream %s" % label, *bindesc)
    h_us.SetXTitle('Hits per event')

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
        h_ds.Fill(nds)
        h_us.Fill(nus)
        continue
    return (h_ds, h_us, h_dsus)

def plot_hits_per_event(tree, label, printer):
    '''
    Call hist_hits_per_event and make plots from the results.

    <label> is a short filesystem-compatible string describing the tree.

    <printer> is some callable taking one argument, <name> which is a
    short, filesystem-compatible string describing the currently drawn
    canvas.  The canvas must be available as a <.canvas> attribute.

    Return a flat list of filenames printed to file.
    '''
    printed = []

    h_ds,h_us,h_dsus = hist_hits_per_event(tree, label)
    
    h_ds.SetLineColor(2)
    h_us.SetLineColor(4)

    printer.canvas.SetLogy(True)

    h_ds.Draw()
    printed += printer('downstream')

    h_us.Draw()
    printed += printer('upstream')

    ustat = move_stats(h_us)
    ustat.SetLineColor(4)
    dstat = move_stats(h_ds, y=-0.25)
    dstat.SetLineColor(2)

    h_ds.Draw()
    h_us.Draw('same')
    ustat.Draw("same")
    dstat.Draw("same")
    printed += printer('dsandus')
    
    printer.canvas.SetLogy(False)
    h_dsus.Draw('COLZ')
    printed += printer('dsvus')

    return printed


def plot_timing_per_event(tree, label, printer):
    '''
    Call hist_timing_per_event and make plots from the results.

    <label> is a short filesystem-compatible string describing the tree.

    <printer> is some callable taking one argument, <name> which is a
    short, filesystem-compatible string describing the currently drawn
    canvas.  The canvas must be available as a <.canvas> attribute.

    Return a flat list of filenames printed to file.
    '''
    printed = []

    h_ds,h_us,h_dsus = hist_hits_per_event(tree, label)

    ((h_ds_hit, h_ds_mean, h_ds_rms),
     (h_us_hit, h_us_mean, h_us_rms)) = hist_timing_per_event(tree, label)

    
    h_ds_hit.SetLineColor(2)
    h_us_hit.SetLineColor(4)

    h_ds_mean.SetLineColor(2)
    h_us_mean.SetLineColor(4)

    h_ds_rms.SetLineColor(2)
    h_us_rms.SetLineColor(4)

    printer.canvas.SetLogy(True)

    h_us_hit.Draw()
    h_ds_hit.Draw("same")
    h_us_mean.Draw("same")
    h_ds_mean.Draw("same")
    printed += printer('hit-times')

    h_ds_rms.Draw()
    h_us_rms.Draw("same")
    printed += printer('hit-rms')

    return printed
