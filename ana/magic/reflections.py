#!/usr/bin/env python

# Local things
data_dir = "/home/bviren/work/wbls/refactor/run/gen-nsrl-reflections"
ref_tree_pat = data_dir + "/13a-water-ref%s.root"
output_dir = './images'
ref_strings = ['0.00', '0.02', '0.05', '0.10', '0.25', '0.50', '1.00']
#ref_strings = ['0.00', '1.00']
ref_floats = map(float,ref_strings)
ref_percent = map(lambda x: int(100*x), ref_floats)

import cowbells
ROOT = cowbells.ROOT
from cowbells.ana.util import make_file_tree, move_stats
canvas = ROOT.TCanvas()

from util import OrgCanvasPrinter
prefix_base = './images/reflections'
printer = OrgCanvasPrinter(canvas, prefix_base)

def generate_plots():
    printed = []

    for refstr, percent in zip(ref_strings, ref_percent):
        rootfile = ref_tree_pat % refstr
        file, tree = make_file_tree (rootfile)
        printer.set_prefix(prefix_base + '13a-water-ref'+refstr)
        label = '%d percent' % percent
        printed += plot_hits_per_event(tree, label, printer)
        continue
    return printed

def format_latex():
    lines = []
    # these strings must match what is used during plotting
    types = ['dsandus', 'dsvus'] 
    for refstr, percent in zip(ref_strings, ref_percent):
        printer.set_outbase(refstr)
        for which in types:
            name = printer.outname(which)
            line = r'\includegraphics[width=0.49\textwidth]{%s.pdf}%%' % name
            lines.append(line)
        lines.append('')     # gain newline to break up pairs of plots
    return '\n'.join(lines)

def format_org(which = 'dsandus'):
    lines = []
    # these strings must match what is used during plotting
    types = ['dsandus', 'dsvus'] 
    for which in types:
        for refstr, percent in zip(ref_strings, ref_percent):
            printer.set_outbase(refstr)
            name = printer.outname(which)
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

def plot_hits():
    'Plot hits per event return list of (filename,caption)'
    results = []
    for refstr, percent in zip(ref_strings, ref_percent):
        canvas.Clear()
        rootfile = ref_tree_pat % refstr
        f,t = make_file_tree (rootfile)
        outfile = './images/reflection-hits-%03d' % percent
        plot_hits(t, canvas, outfile)
        results.append((outfile,'Nhits per event with reflection at %d\\%%'%percent))
    return results

def plot_timing():
    results = []
    for refstr, percent in zip(ref_strings, ref_percent):
        canvas.Clear()
        rootfile = ref_tree_pat % refstr
        f,t = make_file_tree (rootfile)
        outfile = './images/reflection-timing-%03d' % percent
        plot_timing(t, canvas, outfile)
        results.append((outfile, "Reflection = %d\\%%" % percent))
    return results

def plot_dsus():
    results = []
    for refstr, percent in zip(ref_strings, ref_percent):
        canvas.Clear()
        rootfile = ref_tree_pat % refstr
        f,t = make_file_tree (rootfile)
        outfile = './images/reflection-dsus-%03d' % percent
        plot_dsus(t, canvas, outfile)
        results.append((outfile, "Reflection = %d\\%%" % percent))
    return results

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
