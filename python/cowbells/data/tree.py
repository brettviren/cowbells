#!/usr/bin/env python
'''
Interface with WBLS DAQ ROOT trees
'''

import ROOT
from array import array
from collections import namedtuple

def py2root_typecode(py_typecode):
    '''Translate Python array typecodes ROOT typecodes.  Not all
    typecodes are supported.  May raise KeyError'''

    return {
        'b':'B',                # 1-byte   signed char
        'B':'b',                # 1-byte unsigned char
        'h':'S',                # 2-byte   signed short
        'H':'s',                # 2-byte unsigned short
        'i':'I',                # 4-byte   signed int
        'I':'i',                # 4-byte unsigned int
        'l':'L',                # 8-byte   signed int
        'L':'l',                # 8-byte unsigned int
        'f':'F',                # 4-byte float
        'd':'D',                # 8-byte float
        }[py_typecode]


def branch(tree, **fields):
    '''
    Branch a given tree with the given fields.  The fields are keyword
    arguments specifying (typecode, length, description) triples.

    Return a namedtuple representing the fields and holding an object
    that provides the branch memory.
    '''


    names = sorted(fields.keys())    
    values = []
    for name in names:
        py_typecode, length, title = fields[name]
        root_typecode = py2root_typecode(py_typecode)

        initval = 0.0 if py_typecode in ['f','d'] else 0
        val = array(py_typecode, length * [initval])
        values.append(val)

        s = "" if length == 1 else "[%d]"%length
        desc = "%s%s/%s" % (name, s, root_typecode)
        branch = tree.Branch(name,val,desc)
        branch.SetTitle(title)
    return namedtuple(tree.GetName(), names)(*values)

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

