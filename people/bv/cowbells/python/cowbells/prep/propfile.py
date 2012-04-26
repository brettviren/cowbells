#!/usr/bin/env python
'''
Cowbells takes a ROOT file that defines material properties.  

The file is organized like:

  material_name/property_name/tgraph

The material name must already be defined in the geometry.  The
property name is passed unchanged to the MC so should have property
capitalization.  The tgraph holds the property values.  If the
property is a scalar the tgraph has but one entry
'''

import ROOT

def cast(x):
    '''
    Return x as in int, float or string
    '''
    if '.' in x:
        try:
            return float(x)
        except ValueError:
            return x
        pass

    base = 10
    if '0x' == x[:2]: 
        base = 16
    try:
        return int(x,base)
    except ValueError:
        pass

    return x
    

class FileParser:
    '''
    A class that takes a filename of a text file with columns of
    number and then iterates over its rows returning a tuple
    '''
    def __init__(self,filename):
        self.fp = open(filename)
        return
    def __iter__(self): return self
    def next(self):
        line = self.fp.readline()
        if not line:
            raise StopIteration
        line = line.strip()
        print line
        if (not line) or (line[0] == '#'):
            return self.next()
        return [cast(x) for x in line.split()]

class PropertyFile:

    # The sub-directory holding properties.
    propdirname = "properties"

    def __init__(self,filename):
        self.fp = ROOT.TFile.Open(filename,'update')
        pd = self.fp.Get(PropertyFile.propdirname)
        if not pd:
            pd = self.fp.mkdir(PropertyFile.propdirname)
        self.propdir = pd
        return

    def set_axis(self, graph, xaxis, yaxis):
        print 'Setting axis to "%s"/"%s"' % (xaxis, yaxis)
        if xaxis:
            graph.GetXaxis().SetTitle(xaxis)
        if yaxis:
            graph.GetYaxis().SetTitle(yaxis)
        return

    def add(self, matname, propname, data, axis = None):
        'Add from list-of-tuple-like data'
        mdir = self.propdir.Get(matname)
        if not mdir:
            mdir = self.propdir.mkdir(matname)
        mdir.cd()
        g = ROOT.TGraph()
        g.SetName(propname)
        if axis: self.set_axis(g,*axis)
        for i,(x,y) in enumerate(data):
            g.SetPoint(i,x,y)
            continue
        nbytes = g.Write()
        return nbytes
        
    def add_file(self,matname, propname, filename, axis = None):
        'Add material based on text file with two columns of numbers'
        return self.add(matname, propname, FileParser(filename), axis)

    def close(self):
        self.fp.Close()
        return
    pass

if __name__ == '__main__':
    import sys

    outfile = sys.argv[1]
    mat = sys.argv[2]
    prop = sys.argv[3]
    infile = sys.argv[4]

    pf = PropertyFile(outfile)
    pf.add_file(mat,prop,infile)
    pf.close()

