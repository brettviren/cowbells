#!/usr/bin/env python
'''
Plot material properties
'''

import os
import ROOT
from rootutil import walk

class PlotProperty:
    def __init__(self,filename):
        self.canvas = ROOT.TCanvas()
        self.pdffile = os.path.splitext(filename)[0] + '.pdf'
        self.scalars = []
        self.spin(filename)
        return

    def cprint(self,extra=""):
        self.canvas.Print(self.pdffile + extra,'pdf')
        return

    def spin(self,filename):
        self.cprint("[")

        fp = ROOT.TFile.Open(filename)
        top = fp.Get('properties')
        for dirpath, subdirs, objs in walk(top):
            for sd in subdirs:
                self.plot_material(sd)
                continue
            continue
        self.cprint("]")

        for m,p,v in sorted(self.scalars):
            print '\t%s:%s = %f' % (m,p,v)
        return

    def plot_material(self, matdir):
        for dirpath, subdirs, objs in walk(matdir):
            for prop in objs:
                print '%s:%s [%d]' % (matdir.GetName(), prop.GetName(), prop.GetN())

                if prop.GetN() == 1:
                    print 'Got scalar: %s' % prop.GetName()
                    self.scalars.append((matdir.GetName(), prop.GetName(), prop.GetY()[0]))
                    continue
                self.plot_property(matdir.GetName(), prop)
                self.cprint()
                continue
            continue
        return


    def plot_property(self, matname, prop):
        prop.SetTitle("Property %s:%s" % (matname, prop.GetName()))
        prop.Draw("AL")
        return
    pass

if __name__ == '__main__':
    import sys
    pp = PlotProperty(sys.argv[1])
    
