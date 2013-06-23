#!/usr/bin/env python

from bv.rootutil.histo import Writer, Reader, ROOT

def test_histowrite_immediate():
    'Assure we can write immediately to a file'
    fp = ROOT.TFile.Open('test_bv_rootutil1.root','recreate')
    hwi = Writer(fp, lazy = False)
    h = ROOT.TH1F('h','h',100,0,1)
    hwi['h'] = h
    keys = [k.GetName() for k in fp.GetListOfKeys()]
    assert 'h' in keys
    fp.Close()

def test_histowrite_lazy():
    'Assure we can write lazily to a file'
    fp = ROOT.TFile.Open('test_bv_rootutil2.root','recreate')
    hwi = Writer(fp, lazy = True)
    h = ROOT.TH1F('h','h',100,0,1)
    hwi['h'] = h
    keys = [k.GetName() for k in fp.GetListOfKeys()]
    assert 'h' not in keys
    fp.Write()
    keys = [k.GetName() for k in fp.GetListOfKeys()]
    assert 'h' in keys
    fp.Close()
    
def test_historead():
    'Assure we read back what we wrote in immediate/lazy tests'
    for n in [1,2]:
        fp = ROOT.TFile.Open('test_bv_rootutil%d.root' % n)
        hr = Reader(fp)
        h = hr['h']
        assert h
        assert h.GetNbinsX() == 100

def test_both():
    'Test write/read on same file'
    filename = 'test_bv_rootutil_both.root'
    prime = ROOT.TFile.Open(filename,'recreate')
    prime.WriteTObject(ROOT.TH1F("h","h",100,0,10))
    prime.Close()

    fp = ROOT.TFile.Open(filename,'update')
    assert fp
    hw = Writer(fp)
    hr = Reader(fp)
    hw['h2'] = ROOT.TH1F('h2','h2',100,0,1)
    assert hr['h2'].GetNbinsX() == 100
    assert hr['h'].GetNbinsX() == 100
    fp.Write()                  # lazy needs this
    fp.Close()

    fp = ROOT.TFile.Open(filename)
    hr = Reader(fp)
    assert hr['h'].GetNbinsX() == 100
    assert hr['h2'].GetNbinsX() == 100


def test_postwrite():
    '''
    Assure we can fill after handing to the Writer when in lazy mode
    '''
    fp = ROOT.TFile.Open('test_bv_rootutil_postwrite.root','recreate')
    hw = Writer(fp, lazy = True)
    h = ROOT.TH1F('h','h',100,0,1)
    hw['h'] = h
    hw['h'].FillRandom("gaus")
    fp.Write()
    fp.Close()

    fp = ROOT.TFile.Open('test_bv_rootutil_postwrite.root')
    hr = Reader(fp)
    assert hr['h'].GetEntries() == 5000

    return

def test_name_mismatch():
    '''
    Write to a key name differing from the histogram name, make sure we save it by key name

    FIXME: I'd prefer to let the object keep its unique name but 
    ROOT I/O behavior makes this difficult.

    '''
    filename = 'test_bv_rootutil_namemismatch.root'

    fp = ROOT.TFile.Open(filename,'recreate')
    hw = Writer(fp, lazy = False)
    h = ROOT.TH1F('hh','hh',100,0,1)
    h.FillRandom("gaus")
    hw['h'] = h
    fp.Close()

    fp = ROOT.TFile.Open(filename)
    hr = Reader(fp)
    assert hr['h']
    assert hr['h'].GetEntries() == 5000



def test_histowrite_twofiles():
    filepat = 'test_bv_rootutil%d.root'
    fp1 = ROOT.TFile.Open(filepat%1,'recreate')
    fp2 = ROOT.TFile.Open(filepat%2,'recreate')

    hw1 = Writer(fp1, lazy=False)
    hw2 = Writer(fp2, lazy=False)

    hw1['h1'] = ROOT.TH1F('h1','h1',100,0,1)
    hw2['h2'] = ROOT.TH1F('h2','h2',100,0,1)
    
    fp1.Close()
    fp2.Close()

    for n in [1,2]:
        hw = Reader(ROOT.TFile.Open(filepat%n))
        assert set(hw.keys()) == set(['h%d'%n])


from bv.collections import ChainedDict

def test_chained():
    prime = ROOT.TFile.Open('test_bv_rootutil2.root','recreate')
    prime.WriteTObject(ROOT.TH1F("h2","h2",100,0,10))
    prime.Close()

        
    fpw  = ROOT.TFile.Open('test_bv_rootutil1.root','recreate')
    fpr = ROOT.TFile.Open('test_bv_rootutil2.root')
    assert fpr
    hw = Writer(fpw)
    hr1 = Reader(fpw)
    hr2 = Reader(fpr)
    cd = ChainedDict(source=[hr1,hr2], sink=hw)
    cd['h1'] = ROOT.TH1F('h1','h1',100,0,1)
    assert cd['h1']
    assert cd['h2']


if '__main__' == __name__:
    test_name_mismatch()
    test_histowrite_immediate()
    test_histowrite_lazy()
    test_historead()
    test_both()
    test_postwrite()
    test_histowrite_twofiles()
    test_chained()
