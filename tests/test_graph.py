#!/usr/bin/env python

import ROOT

def doit():
    g = ROOT.TGraph()
    g.SetPoint(0,1,8)
    g.SetPoint(1,2,9)
    g.SetPoint(2,3,10)
    g.GetXaxis().SetTitle("the x axis")
    g.GetYaxis().SetTitle("the y axis")
    g.SetName("somegraph")
    g.SetTitle("some title")

    fp = ROOT.TFile.Open("test_graph.root","update")
    g.Write()
    fp.Close()

if __name__ == '__main__':
    
    doit()
