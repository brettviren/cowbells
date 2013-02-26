#!/usr/bin/env python
'''
Interface with WBLS DAQ ROOT trees
'''

import ROOT
from array import array

class WblsDaqTree(object):

    fadc_desc = [
        ("EventNumber", 'i', 1),
        ("TriggerTimeFromRunStart", 'd', 1),
        ("Channel0", 'H', 2560),
        ("Channel1", 'H', 2560),
        ("Channel2", 'H', 2560),
        ("Channel3", 'H', 2560),
        ]

    def __init__(self, filename):
        self._tfile = ROOT.TFile.Open(filename)
        self._head_tree = self._tfile.Get("Header")
        self._fadc_tree = self._tfile.Get("FADCData")
        
        for name, type, size in self.fadc_desc:
            datum = array(type,[0]*size)
            self.__dict__['_'+name] = datum
            self._fadc_tree.SetBranchAddress(name,datum)
            continue
        return

    def get(self, name):
        try:
            datum = self.__dict__['_'+name]
        except IndexError:
            return self.__dict__[name]

        if len(datum) == 1: 
            return datum[0]
        return datum

    def __getattr__(self,name):
        return self.get(name)

    def get_entry(self, number):
        return self._fadc_tree.GetEntry(number)

    def spin(self, spinner):
        for entry in range(self._fadc_tree.GetEntries()):
            self.get_entry(entry)
            spinner(self)
            continue
        return


def simple_spinner(t):
    print t.EventNumber,t.TriggerTimeFromRunStart,t.Channel0[0]

if __name__ == '__main__':
    import sys
    t = WblsDaqTree(sys.argv[1])
    t.spin(simple_spinner)

