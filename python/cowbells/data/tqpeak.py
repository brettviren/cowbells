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
        hists = []

        max_q = 50.0
        nplots = 10
        for size in range(nplots):
            h = ROOT.TH2F('peaktmpl%02d'%size,'Peak template (%d from %d/%d)' % (size, max_q, nplots),
                          20,-10,10, 100,0,100)
            ROOT.SetOwnership(h,0)
            hists.append(h)

        time_center = 1525
        time_cut = 25
        charge_center = 80
        charge_cut = 30
        # cut:"abs(tmin[0]-1525)<25 && abs(qpeak[0]-80) <30 && qnpeaks[0] == 1"
        npeaks = 0
        for peaks in self.peaks:
            if not peaks:
                continue
            peak = peaks[0]
            q = peak['q']
            t = peak['t']

            maxq = max(q)
            sumq = sum(q)
            maxi = q.index(maxq)

            plot_num = min(int(min(maxq,max_q)/max_q*nplots),nplots)+1

            center = t + maxi
            if abs(center-time_center) > time_cut: 
                continue
            if abs(sumq - charge_center) > charge_cut:
                continue

            npeaks += 1

            h_all = hists[0]
            h_maxq = hists[plot_num]
            print maxq
            for x,y in enumerate(q):
                h_all.Fill(maxi-x, y)
                h_maxq.Fill(maxi-x, y)
                continue
            continue

        self.canvas.Clear()
        self.cprint('[')
        for h in hists:
            h.SetStats(0)
            h.Draw("colz")
            self.cprint()
        self.cprint(']')
        print 'Looked at %d peaks' % npeaks
        return
    def all(self):
        self.do_peaks()


if __name__ == '__main__':
    import sys
    try:
        pdf = sys.argv[2]
    except IndexError:
        pdf = None
    p = Plots(sys.argv[1], pdffile=pdf)
    p.all()
