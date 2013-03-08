#!/usr/bin/env python
'''
Show a channel map of all wave forms
'''

import ROOT
from array import array

class TreeSpinner(object):
    def __init__(self, nmaxtrig = 1000):
        self.trigger_number = 0

        self.nmaxtrig = nmaxtrig

        self.wft=[]
        self.avg=[]
        self.avg20=[]
        self.avg20s20=[]
        self.avg20a20=[]
        self.avg600a200=[]
        self.avg600s20=[]
        self.nfills = 0

        for chn in range(4):
            wft = ROOT.TH2I('wft%d'%chn, 'Wave Form vs. Time for channel %d'%chn,
                            nmaxtrig, 0, nmaxtrig, 2560,0,2560)
            self.wft.append(wft)

            avgw = ROOT.TH1F('avgw%d'%chn, 'Average of Wave Forms for Ch #%d'%chn,
                             2560,0,2560)
            self.avg.append(avgw)

            avg20 = ROOT.TH1F('avg20%d'%chn, 'Average of Wave Forms for Ch #%d 20-subtracted'%chn,
                             2560,0,2560)
            self.avg20.append(avg20)
            avg20s20 = ROOT.TH1F('avg20s20%d'%chn, 'Average of Wave Forms for Ch #%d 20skip20-subtracted'%chn,
                                 2560,0,2560)
            self.avg20s20.append(avg20s20)
            avg20a20 = ROOT.TH1F('avg20a20%d'%chn, 'Average of Wave Forms for Ch #%d 20avg20-subtracted'%chn,
                                 2560,0,2560)
            self.avg20a20.append(avg20a20)

            avg600a200 = ROOT.TH1F('avg600a200%d'%chn, 'Average of Wave Forms for Ch #%d 600avg200-subtracted'%chn,
                                 2560,0,2560)
            self.avg600a200.append(avg600a200)

            avg600s20 = ROOT.TH1F('avg600s20%d'%chn, 'Average of Wave Forms for Ch #%d 600skip20-subtracted'%chn,
                                 2560,0,2560)
            self.avg600s20.append(avg600s20)


        return

    def write(self):
        for h in self.wft:
            h.Write()
        for h in self.avg + self.avg20 + self.avg20s20 + self.avg20a20 + self.avg600a200 + self.avg600s20:
            h.Scale(1.0/self.trigger_number)
            h.Write()

    def fill_sig(self, chn, sig):
        wft = self.wft[chn]
        avg = self.avg[chn]
        avg20 = self.avg20[chn]
        avg20s20 = self.avg20s20[chn]
        avg20a20 = self.avg20a20[chn]
        avg600a200 = self.avg600a200[chn]
        avg600s20 = self.avg600s20[chn]

        ped20 = sig[:20]
        ped20s20 = sig[20:40]
        ped600s20 = sig[600:620]

        ped600a200 = array('f',[0]*20)
        for ind, q in enumerate(sig[600:800]):
            ped600a200[ind%20] += q
        for ind in range(20):
            ped600a200[ind] = ped600a200[ind]/10.0

        for t,q in enumerate(sig):
            if self.trigger_number < self.nmaxtrig:
                wft.Fill(self.trigger_number, t, q)
            avg.Fill(t+0.5, q)

            iped = t%20
            avg20.Fill(t+0.5,    q - ped20[iped])
            avg20s20.Fill(t+0.5, q - ped20s20[iped])
            avg20a20.Fill(t+0.5, q - 0.5*(ped20[iped]+ped20s20[iped]))
            avg600s20.Fill(t+0.5, q - ped600s20[iped])
            avg600a200.Fill(t+0.5, q - ped600a200[iped])

    def __call__(self, daq):
        for chn in range(4):
            sig = daq.get("Channel%d"%chn)
            self.fill_sig(chn, sig)
            continue
        self.trigger_number += 1
        return
    pass

def proc(infilename, outfilename):
    import tree
    daq = tree.WblsDaqTree(infilename)

    outfp = ROOT.TFile.Open(outfilename, 'RECREATE')

    ts = TreeSpinner()
    daq.spin(ts, 1000)
    
    outfp.cd()
    ts.write()
    outfp.Close()
    return

if __name__ == '__main__':
    import sys
    proc(sys.argv[1], sys.argv[2])



