#!/usr/bin/env python

# Local things
data_dir = "/home/bviren/work/wbls/refactor/run/gen-nsrl-reflections"
ref_tree_pat = data_dir + "/13a-water-ref%s.root"
output_dir = './images'
ref_strings = ['0.00', '0.02', '0.05', '0.10', '0.25', '0.50', '1.00']
#ref_strings = ['0.00', '1.00']
ref_floats = map(float,ref_strings)
ref_percent = map(lambda x: int(100*x), ref_floats)

from cowbells.ana.magic import plots as mp
canvas = mp.ROOT.TCanvas()

def spit_org_figures(figcap, flavor='latex'):
    'Give a list of (filename,caption) tuples, return string flavored syntax'
    ret = []
    
    if flavor.lower() in ['latex']:
        for fig,cap in figcap:
            ret.append(r'\includegraphics[width=0.8\textwidth]{%s.pdf}' % fig)
            ret.append(r'\begin{center}%s\end{center}'% cap)

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
        f,t = mp.make_file_tree (rootfile)
        outfile = './images/reflection-hits-%03d' % percent
        mp.plot_hits(t, canvas, outfile)
        results.append((outfile,'Nhits per event with reflection at %d%%'%percent))
    return results

def plot_timing():
    results = []
    for refstr, percent in zip(ref_strings, ref_percent):
        canvas.Clear()
        rootfile = ref_tree_pat % refstr
        f,t = mp.make_file_tree (rootfile)
        outfile = './images/reflection-timing-%03d' % percent
        mp.plot_timing(t, canvas, outfile)
        results.append((outfile, "Reflection = %d%%" % percent))
    return results

