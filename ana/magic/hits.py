#!/usr/bin/env python

import os
import cowbells
ROOT = cowbells.ROOT
import histo
from collections import defaultdict

canvas = ROOT.TCanvas('hits_canvas', 'hits_canvas', 1100, 800)

proc_types = {
    (2, 21): 'Cerenkov',
    (2, 22): 'Scintillation',
    (3, 34): 'OpWLS',
    (2, 12): 'phot',
    (2, 13): 'compt',
    (2, 2): 'eIoni',
    (2, 2): 'hIoni',
    (2, 3): 'eBrem',
}
proc_names = sorted(proc_types.values())
def proc_index(typ, subtyp):
    return proc_names.index(proc_types[(typ,subtyp)])

def print_filename(sample, what, pmtid, extra = ""):
    return 'images/hits/{sample}_{what}_pmt{pmtid}{extra}.pdf'.format(**locals())



class Sample(object):
    run_dir = "/home/bviren/work/wbls/refactor/run/nsrl-13a-wbls"
    file_pat = "nsrl-13a-2gev-protons-{sample}.hits-steps.1k.root"
    
    q_hist_name = 'h_pe_{pn}_pmt{pmtid}'
    t_hist_name = 'h_time_{pn}_pmt{pmtid}'

    def __init__(self, name):
        self.name = name
        filename = os.path.join(self.run_dir, self.file_pat.format(sample=name))
        fp = ROOT.TFile.Open(filename)
        if not fp:
            raise ValueError, 'Failed to open for reading: %s' % filename
        self.file = fp
        self.tree = fp.Get('cowbells')

    def make_q_plots(self, hcid = 1):
        sample = self.name
        title = 'PE in PMT{pmtid} for process {pn} in {sample}'
        name = self.q_hist_name
        hdesc = (200,0,20)

        all_hists = [{},{}]

        for ptpst, pn in proc_types.items():
            for pmtid in range(2):
                fname = name.format(pn=pn.lower(), pmtid=pmtid)
                ftitle = title.format(**locals())
                hist = ROOT.TH1D(fname, ftitle, *hdesc)
                all_hists[pmtid][ptpst] = hist

        for entry in self.tree:
            pe = [defaultdict(int), defaultdict(int)]
            for hit in entry.event.hc:
                if hit.hcId() != hcid:
                    continue            
                ptpst = (hit.pType(),hit.pSubType())
                if ptpst == (0,0): # transport?
                    continue
                pe[hit.volId()][ptpst] += 1

            for ptpst, pn in proc_types.items():
                for pmtid in range(2):
                    n = pe[pmtid][ptpst]
                    if n:       # zero suppress
                        all_hists[pmtid][ptpst].Fill(n)

        return all_hists

    def make_t_plots(self, hcid = 1):
        sample = self.name
        what = 'hc.t'
        cut = 'hc.ptype == {pt} && hc.psubtype == {pst} && hc.volid == {pmtid} && hc.hcid == {hcid}'
        title = 'Hit times in PMT{pmtid} for process {pn} in {sample}'
        name = self.t_hist_name
        hdesc = (200, 0, 100)

        all_hists = [{},{}]

        for ptpst, pn in proc_types.items():
            (pt,pst) = ptpst 
            for pmtid in range(2):
                fname = name.format(pn=pn.lower(), pmtid=pmtid)
                ftitle = title.format(**locals())
                fcut = cut.format(**locals())
                hist = ROOT.TH1D(fname, ftitle, *hdesc)
                all_hists[pmtid][ptpst] = hist
                self.tree.Draw("%s >> %s"%(what,fname), fcut, 'colz')
        return all_hists

    def print_t(self):
        for pmtid, perpmt in enumerate(self.make_t_plots()):
            self.draw_hists(perpmt.values())
            f = print_filename(self.name, 'timing', pmtid)
            canvas.Print(f, 'pdf')
            for h in perpmt.values():
                canvas.Clear()
                canvas.SetLogy()
                h.Draw()
                f = print_filename(self.name, 'timing', pmtid, extra='_'+h.GetName())
                canvas.Print(f, 'pdf')


    def print_q(self):
        for pmtid, perpmt in enumerate(self.make_q_plots()):
            self.draw_hists(perpmt.values())
            f = print_filename(self.name, 'charge', pmtid)
            canvas.Print(f, 'pdf')
            for h in perpmt.values():
                canvas.Clear()
                canvas.SetLogy()
                h.Draw()
                f = print_filename(self.name, 'charge', pmtid, extra='_'+h.GetName())
                canvas.Print(f, 'pdf')


    def save(self, tfile): 
        sd = tfile.Get(self.name)
        if not sd:
            sd = tfile.mkdir(self.name)
        for h in self.hists.values():
            sd.WriteTObject(h)

    def draw_hits(self, pmtid):
        hists = self.ht.plot_hits_by_proc(pmtid)
        return self.draw_hists(hists)

    def draw_pe(self, pmtid):
        hists = self.ht.plot_pe_by_proc(pmtid)
        return self.draw_hists(hists)

    def draw_hists(self, hists):
        canvas.Clear()
        canvas.Divide(3,3)
        for count, hist in enumerate(hists):
            pad = canvas.cd(count+1)
            pad.SetLogy()
            hist.Draw()

        canvas.Modified()
        canvas.Update()
        return 
    
def test():
    water = Sample('water')
    wbls01 = Sample('wbls01')
    water.print_t()
    wbls01.print_t()
    water.print_q()
    wbls01.print_q()

if __name__ == '__main__':
    test()
# see hits.org for some higher level entries
