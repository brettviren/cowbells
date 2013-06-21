#!/usr/bin/env python
'''
Histogram store
'''

from UserDict import DictMixin

import ROOT

class Histo(DictMixin):
    '''
    Provide a dictionary interface to a TDirectory (TFile) for
    managing ROOT Histogram objects (any TNamed object, really).

    The TDirectory must be associated with a TFile opened with the
    "UPDATE" option if items are to be set on objects of this class.

    Note, that this allows items to be set using a key name that may
    differ from the histogram name.  Getting an item by histogram name
    will still work but will create a duplicate object in memory.  If
    you do not wish to save these do not do an explicit TFile::Write()
    on the file holding the TDirectory given to Histo.
    '''

    def __init__(self, tdir = None):
        '''
        A dictionary-like collection of histograms (any TObjects,
        really) tied to a file (TDirectory).  <tdir> is some ROOT
        TDirectory-like thing where the histograms are to be kept.  It
        needs to be writable in order to store histograms.
        '''
        self.tdir = tdir
        self.bag = dict()

    def __getitem__(self, name):
        hist = self.bag.get(name)
        if hist: return hist
        if self.tdir:
            hist = self.tdir.Get(name)
        if not hist: 
            raise KeyError, 'No histogram "%s"' % name
        self[name] = hist
        return hist

    def __setitem__(self, name, obj):
        obj.SetDirectory(0)
        if name != obj.GetName():
            obj.SetName(name)
        self.bag[name] = obj
        return
        
    def add(self, obj):
        self[obj.GetName()] = obj

    def keys(self):
        kl = set()
        if self.tdir:
            kl = set([k.GetName() for k in self.tdir.GetListOfKeys()])
        map(kl.add, self.bag.keys())
        return list(kl)

    def flush(self, tdir = None):
        '''
        Write all hists to directory
        '''
        tdir = tdir or self.tdir
        if not tdir:
            raise ValueError, 'No TDirectory to flush to' 
        for obj in self.bag.values():
            tdir.WriteTObject(obj)
            

def test():
    fd = ROOT.TFile.Open('test_histo.root','recreate')
    h = Histo(fd)
    h['h1key'] = ROOT.TH1F('h1name','hist1',10,-1,1)
    assert h['h1key']
    h['h1key'].FillRandom('gaus')
    entries = h['h1key'].GetEntries()
    assert entries
    print 'Original entries:', entries
    h.flush()
    fd.Close()
    del(h)

    print 'Opening file read-only'

    fd2 = ROOT.TFile.Open('test_histo.root','readonly')
    h2 = Histo(fd2)
    print 'keys',h2.keys()
    assert 'h1key' in h2.keys()
    print 'h1key',h2.get('h1key')
    assert h2.get('h1key')
    print 'h1name',h2.get('h1name')
    assert not h2.get('h1name')
    assert entries == h2['h1key'].GetEntries()

if __name__ == '__main__':
    test()
