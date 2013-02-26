#!/usr/bin/env python
'''
Display peaks saved by tqtree
'''

import ROOT
import json

class Plots(object):
    '''
    Plot the peaks saved to the JSON file by running tqtree.
    '''
    def __init__(self, peakfile, canvas = None, pdffile = 'tqpeak.pdf'):
        self.peaks = json.loads(open(peakfile).read())

        self.pdffile = pdffile
        if not canvas:
            canvas = ROOT.TCanvas("tqpeak","tqpeak debug", 0,0, 1000, 700)
        self.canvas = canvas

    def cprint(self,extra=''):
        self.canvas.Print('%s%s'%(self.pdffile,extra), 'pdf')
        
    def do_peaks(self):
        self.canvas.Clear()
        
        h = ROOT.TH2F('peaktmpl','Peak template',
                      20,-10,10, 100,0,100)
        ROOT.SetOwnership(h,0)

        time_center = 1525
        time_cut = 25
        charge_center = 80
        charge_cut = 30
        # cut:"abs(tmin[0]-1525)<25 && abs(qpeak[0]-80) <30 && qnpeaks[0] == 1"
        for peaks in self.peaks:
            if len(peaks) != 1:
                continue
            peak = peaks[0]
            q = peak['q']
            t = peak['t']
            maxq = max(q)
            sumq = sum(q)
            maxi = q.index(maxq)
            center = t + maxi
            if abs(center-time_center) > time_cut: 
                continue
            if abs(sumq - charge_center) > charge_cut:
                continue
            for x,y in enumerate(q):
                h.Fill(maxi-x, y)
        h.Draw("colz")

    def all(self):
        self.cprint('[')
        for what in [
            'peaks',
            ]:
            meth = getattr(self, 'do_%s' % what)
            meth()
            self.cprint()
        self.cprint(']')

if __name__ == '__main__':
    import sys
    try:
        pdf = sys.argv[2]
    except IndexError:
        pdf = None
    p = Plots(sys.argv[1], pdffile=pdf)
    p.all()
