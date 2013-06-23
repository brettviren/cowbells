#!/usr/bin/env python

import os
from bv.rootutil.printing import PrintManager, MultiPrinter

import ROOT
canvas = ROOT.TCanvas()

def filenamer(name, **kwds):
    return name + '.pdf'

bucket = set()
def drawer(name, **kwds):
    h = ROOT.TH1F(name,name,100,-10,10)
    h.FillRandom("gaus")
    canvas.Clear()
    canvas.SetLogy()
    bucket.add(h)               # this is needed or else h will
    h.Draw()                    # disappear before printing can happen later

def test_printer():
    p = PrintManager(canvas.Print, drawer, filenamer, overwrite=True)
    fn1 = p['plot1']
    assert fn1 and 'plot1' in fn1
    fn2 = p['plot2']
    assert fn1 != fn2
    fn3 = p['plot1']    
    assert fn1 == fn3
    for fn in [fn1,fn2,fn3]:
        assert os.path.exists(fn), 'No such file: %s' % fn
        print fn

def test_multiprinter():
    with MultiPrinter(filename='mplot.pdf', printer = canvas.Print) as printer:
        pm = PrintManager(printer, drawer, filenamer, overwrite=True)
        pm('plot1')
        pm('plot2')
        pm('plot3')
    


if '__main__' == __name__:
    test_printer()
    test_multiprinter()
