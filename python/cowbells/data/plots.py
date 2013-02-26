#!/usr/bin/env python

import os
import ROOT

import tree

class Summary(object):
    '''
    Make some summary plots of a raw FADC file
    '''
    def __init__(self, filename, pdffile = None, canvas = None):
        if canvas is None:
            canvas = ROOT.TCanvas("wbls_ana_plots","WBLS analysis plots", 500, 400)
        self.canvas = canvas

        self.wbls = tree.WblsDaqTree(filename)
        
        if pdffile is None:
            pdffile = os.path.splitext(filename)[0] + '.pdf'
        self.pdffile = pdffile
        self.cprint("[")
        return
    
    def __del__(self): self.close()
    def close(self):
        if not self.canvas: return
        self.cprint("]")
        self.canvas = None
        return

    def clear(self):
        self.canvas.Clear()
        return

    def cprint(self, extra=""):
        self.canvas.Print(self.pdffile+extra,'pdf')
        return

    def plot_waveform(self):
        '''
        Make plots of histogram of each channels waveforms
        '''
        hists = []
        for chn in range(4):
            hist = ROOT.TH2F("channel%d"%chn, "Channel %d"%chn,
                             256,0,2560, 1000,0,10000) # insane?
            hists.append(hist)
            continue
        def fill(t):
            #print type(t),t
            for chn in range(4):
                hist = hists[chn]
                name = 'Channel%d'%chn
                signal = t.get(name)
                
                for tim,ph in enumerate(signal):
                    hist.Fill(tim,ph)
                    continue

                continue
            return
        self.wbls.spin(fill)
        
        self.clear()
        self.canvas.Divide(2,2)
        for chn in range(4):
            self.canvas.cd(chn+1)
            hists[chn].Draw("colz")
            continue
        self.cprint()

        f = ROOT.TFile.Open("dump.root","recreate")
        f.cd()
        for h in hists: h.Write()
        f.Close()
        return

    pass

if __name__ == '__main__':
    import sys
    s = Summary(sys.argv[1],sys.argv[2])
    s.plot_waveform()
    s.close()


