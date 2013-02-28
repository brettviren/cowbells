#!/usr/bin/env python

import os
import ROOT
import tree

class ChanDisp(object):
    def __init__(self, filename, canvas = None, pdffile = "chandisp.pdf"):
        basename = os.path.splitext(filename)[0]
        if canvas is None:
            canvas = ROOT.TCanvas("wbls_ana_plots_%s"%basename,
                                  "WBLS analysis plots", 0,0, 1000, 700)
        self.canvas = canvas
        self.wbls = tree.WblsDaqTree(filename)
        self._entnum = -1
        self.pdffile = pdffile
        self._print = True
        self.cprint("[")
        return

    def close(self):
        if self._print:
            self.cprint("]")

    def cprint(self,extra=''):
        if not self._print: return
        self.canvas.Print('%s%s'%(self.pdffile,extra), 'pdf')

    def fadc_hist(self, name="fadc", title="FADC", **params):
        '''
        Make an new FADC histogram with name and title interpolated
        with any given params + "entry" added internally.
        '''
        params.setdefault('entry',self._entnum)
        h = ROOT.TH1F(name%params, title%params, 2560,0,2560)
        return h

    def hist_qvt(self, chn):
        '''
        Charge vs. time (FACD index) histogram for current entry and
        given channel number.
        '''
        qvt = self.fadc_hist("channel%(chn)d",
                             "Channel %(chn)d, entry %(entry)d",
                             chn = chn)
        qvt.SetXTitle('FADC time bin')
        qvt.SetYTitle('FADC charge per time bin')
        signal = self.wbls.get('Channel%d'%chn)
        print 'Signal length %d, max=%d, min=%d' %\
            (len(signal),max(signal),min(signal))
        for t,q in enumerate(signal):
            qvt.Fill(t,q)
        return qvt

    def hist_qvt_nopeak(self, chn, low, high):
        '''
        Charge vs. time (FACD index) histogram for current entry and
        given channel number avoiding -low/+high around minq
        '''

        qvt = self.hist_qvt(chn)
        start, stop = self.get_nopeak_window(chn, low, high)
        signal = self.wbls.get('Channel%d'%chn)
        windowed = signal[:start] + signal[stop:]
        qvt.SetMinimum(min(windowed))
        return qvt

    def hist_pwr(self, chn):
        '''
        Power spectrum across FADC indices for current entry and given
        channel number.
        '''
        signal = self.wbls.get('Channel%d'%chn)
        minq,maxq = min(signal),max(signal)
        padding = 30            # cosmetic, room for stats box
        pwr = ROOT.TH1F("power%d"%chn, 
                        "FADC bin power in channel %d entry %d"%\
                            (chn, self._entnum),
                        maxq-minq+1+padding,minq-1,maxq+padding)
        pwr.SetXTitle('FADC charge per time bin')
        map(pwr.Fill,signal)
        pwr.Fit("gaus","","goff")
        return pwr

    def get_nopeak_window(self, chn, low, high):
        '''Return window around peak as (start,stop) tuple'''

        signal = self.wbls.get('Channel%d'%chn)
        minq = min(signal)
        minbin = signal.index(minq)
        start = max(0, minbin - low)
        stop = min(len(signal), minbin + high)
        return (start, stop)

    def hist_pwr_nopeak(self, chn, low, high):
        '''
        Like hist_pwr but to not include signal -low/+high around the
        FADC time bin holding min ADC.
        '''
        start, stop = self.get_nopeak_window(chn, low, high)
        signal = self.wbls.get('Channel%d'%chn)
        windowed = signal[:start] + signal[stop:]
        minq,maxq = min(windowed),max(windowed)

        pwr = ROOT.TH1F("power_nopeak%d"%chn, 
                        "FADC bin power outside [%d,%d] window in channel %d entry %d"%\
                            (low, high, chn, self._entnum),
                        maxq-minq+2, minq-1, maxq)

        map(pwr.Fill, windowed)
        pwr.Fit("gaus","","goff")
        return pwr

    def entry(self, number = None):
        if number is None:
            number = self._entnum + 1
        self._entnum = number
        self.wbls.get_entry(number)
        print 'Entry #%d' % number
        return

    def hist_baseline_subtracted(self, sighist, baseline):
        '''
        Return signal histogram baseline subtracted
        '''
        ret = self.fadc_hist("%(oldname)s_sub",
                             "%(oldtitle)s (baseline subtraction = %(baseline).1f)",
                             oldname = sighist.GetName(), 
                             oldtitle = sighist.GetTitle(), 
                             baseline = baseline)
        for ibin in range(sighist.GetNbinsX()):
            ret.SetBinContent(ibin, baseline - sighist.GetBinContent(ibin))
        return ret


    def hist_peak_zoom(self, h, low, high):
        'Zoom low/high around peak bin of h'
        h = h.Clone()
        mb = h.GetMaximumBin()
        x = h.GetXaxis()
        x.SetRangeUser(max(0,mb-low), min(h.GetNbinsX(), mb+high))
        return h

    def q(self, number = None, chn = 0):
        low, high = 50,150

        self.entry(number)
        qvt = self.hist_qvt(chn)

        qvt_windowed = self.hist_qvt_nopeak(chn, low, high)

        #pwr = self.hist_pwr(chn)
        pwr = self.hist_pwr_nopeak(chn, low, high)

        fit = pwr.GetFunction("gaus")
        if fit:
            mean = fit.GetParameter(1)
            sigma = fit.GetParameter(2)
        else:
            print 'Failed to fit'
            mean = 2**13
            sigma = 0
        qbl = self.hist_baseline_subtracted(qvt, mean)
        zoom = self.hist_peak_zoom(qbl, low, high)

        hists = [qvt,qvt_windowed, pwr, zoom]
        logy = [False, False, True, False]

        self.canvas.Clear()
        self.canvas.Divide(2,2)
        for n,h in enumerate(hists):
            pad = self.canvas.cd(n+1)

            h.Draw()
            pad.Modified()
            pad.Update()
            stats = h.FindObject("stats")
            if stats:
                stats.SetOptStat(1110)
                stats.SetOptFit(111)

            if logy[n]: 
                pad.SetLogy(True)
                h.SetStats(1)
            else:
                pad.SetLogy(False)
                h.SetStats(0)
            pad.Modified()
            pad.Update()

        self.canvas.Modified()
        self.canvas.Update()
        self.cprint()
        return hists

    def qvt(self, number = None):
        self.entry(number)
        self.canvas.Clear()
        self.canvas.Divide(2,2)
        hists = []

        for chn in range(4):
            hist = self.hist_qvt(chn)
            hists.append(hist)
            self.canvas.cd(chn+1)
            hist.Draw()
        self.cprint()
        return hists


    pass


        
    
early_tmin_entries = [
42 , 57 , 63 , 80 , 104 , 106 , 126 , 139 , 168 , 246 , 251 , 271 , 305 , 311 , 393 , 541 , 597 , 633 , 660 , 720 , 734 , 744 , 817 , 844 , 874 , 891 , 908 , 910 , 921 , 922 , 936 , 938 , 974 , 978 , 998 , 1016 , 1080 , 1095 , 1179 , 1220 , 1230 , 1249 , 1255 , 1284 , 1293 , 1298 , 1458 , 1487 , 1497 , 1532 , 1603 , 1665 , 1680 , 1702 , 1717 , 1720 , 1868 , 1933 , 1935 , 1944 , 1948 , 2029 , 2051 , 2078 , 2105 , 2192 , 2279 , 2280 , 2287 , 2330 , 2359 , 2423 , 2429 , 2484 , 2515 , 2586 , 2595 , 2603 , 2604 , 2647 , 2651 , 2674 , 2717 , 2721 , 2759 , 2763 , 2830 , 2852 , 2907 , 2934 , 3003 , 3013 , 3029 , 3036 , 3039 , 3045 , 3047 , 3055 , 3084 , 3089 , 3122 , 3125 , 3133 , 3163 , 3271 , 3303 , 3326 , 3339 , 3348 , 3384 , 3408 , 3433 , 3436 , 3444 , 3493 , 3519 , 3549 , 3589 , 3621 , 3671 , 3721 , 3874 , 3888 , 3952 , 3953 , 3995 , 4002 , 4117 , 4130 , 4160 , 4171 , 4241 , 4289 , 4317 , 4320 , 4352 , 4368
]

high_rms_entries = [99,388,1037,1629,2864,2966,4300]


if __name__ == '__main__':
    import sys
    try:
        pdf = sys.argv[2]
    except IndexError:
        pdf = None
    cd = ChanDisp(sys.argv[1], pdffile=pdf)

    for ent in [2,4,9]:
        cd.q(ent)
    cd.close()
