#!/usr/bin/env python
'''
Interface to PSTAR data

http://physics.nist.gov/PhysRefData/Star/Text/PSTAR.html
'''

import os

samples = ['air', 'aluminum', 'liquidwater', 'plasticscint', 'teflon', 'toluene']

def parse_file(filename):
    '''
    Return list of tuples (kinE,estop,nstop,totstop,csda,proj)

*PSTAR: Stopping Powers and Range Tables for Protons
*
*AIR (dry, near sea level)                                               
*
*Kinetic   Electron. Nuclear   Total     CSDA      Projected 
*Energy    Stp. Pow. Stp. Pow. Stp. Pow. Range     Range     
*MeV       MeV cm2/g MeV cm2/g MeV cm2/g g/cm2     g/cm2     
*
1.000E-03 1.197E+02 2.163E+01 1.414E+02 9.857E-06 3.257E-06 
....
    '''
    fp = open(filename)
    ret = []
    for line in fp.readlines():
        if line[0] == '*': continue
        ret.append(map(float,line.strip().split()))
    return ret


def data_by_name(name):
    filename = os.path.join(os.path.dirname(__file__),name+'.txt')
    if not os.path.exists(filename): return None
    return parse_file(filename)

def data_to_tgraph(data):
    import ROOT
    names = ['Estop','Nstop','Tstop','CSDArange','PROJrange']
    def mkgraph(name): 
        g = ROOT.TGraph()
        g.SetName(name)
        return g
    graphs = map(mkgraph,names)
    for line in data:
        mev = line[0]
        for val,graph in zip(line[1:],graphs):
            graph.SetPoint(graph.GetN(), mev, val)
            continue
        continue
    return graphs

def plot(name):
    graphs = data_to_tgraph(data_by_name(name))
    
    import ROOT
    canvas = ROOT.TCanvas()

    pdffile = 'pstar_%s.pdf' % name
    canvas.Print(pdffile+"[","pdf")
    canvas.SetLogy(True)
    for g in graphs:
        g.SetTitle(name)
        g.Draw("AL")
        g.GetXaxis().SetTitle(g.GetName())
        g.GetYaxis().SetTitle('Energy (MeV)')
        canvas.Print(pdffile,"pdf")
        continue
    canvas.Print(pdffile+"]","pdf")
    return

if __name__ == '__main__':
    for sam in samples:
        print 'Sample: "%s"' % sam
        plot(sam)

