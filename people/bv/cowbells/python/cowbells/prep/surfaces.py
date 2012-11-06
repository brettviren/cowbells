#!/usr/bin/env python 
'''
Define optical surfaces
'''

import ROOT

class GenericSurface(object):

    # location for all surfaces
    subdir = "surfaces"

    # Known parameters
    known_parameters = ['type', 'model', 'finish', 'first', 'second',
                        'polish', 'sigmaalpha']

    # Known properties
    known_properties = ['RINDEX','REALRINDEX','IMAGINARYRINDEX',
                        'REFLECTIVITY','EFFICIENCY','TRANSMITTANCE',
                        'SPECULARLOBECONSTANT','SPECULARSPIKECONSTANT',
                        'BACKSCATTERCONSTANT']


    def __init__(self, name, **parameters):

        self.name = name
        self.parameters = {}
        self.properties = []
        for k,v in parameters.iteritems():
            self.add_parameter(k,v)
            continue
        return

    def add_parameter(self, key, value):
        assert key in self.known_parameters, \
            'Unknown parameter given to surface %s: "%s"' % (self.name, key)
        self.parameters[key] = value
        return

    def add_property_tgraph(self, tgraph):
        name = tgraph.GetName() 
        assert name in self.known_properties, \
            'Unknown property given to surface %s: "%s"' % (self.name, name)
        self.properties.append(tgraph)
        return

    def add_property_lists(self, name, xvals, yvals):
        g = ROOT.TGraph()
        g.SetName(name)
        for x,y in zip(xvals,yvals):
            g.SetPoint(g.GetN,x,y)
        self.add_tgraph(tgraph)
        return

    def write(self, tfile):

        i_opened = False
        if isinstance(tfile,str):
            tfile = ROOT.TFile.Open(tfile,'update')
            ROOT.SetOwnership(tfile,0)
            i_opened = True
            print 'Opened TFile: "%s"' % tfile.GetName()

        sdir = tfile.Get(self.subdir)
        if not sdir:
            print 'Making subdir "%s"' % self.subdir
            sdir = tfile.mkdir(self.subdir)
            assert sdir,"Failed to make dir %s in %s" %\
                (self.subdir, tfile.GetName())
            pass
        print 'Making subdir "%s"' % self.name
        mydir = sdir.mkdir(self.name)
        assert mydir, "Failed to make dir %s in %s" % \
            (self.name, sdir.GetName())

        print 'Writing %s/%s to %s' % (self.subdir, self.name, tfile.GetName())

        mydir.cd()
        pardir = mydir.mkdir('parameters')
        pardir.cd()
        for k,v in sorted(self.parameters.iteritems()):
            n = ROOT.TNamed(k,v)
            n.Write()
            print '\t%s parameters:%s/%s' % (self.name,k,v)
            continue

        mydir.cd()
        prodir = mydir.mkdir('properties')
        prodir.cd()
        for p in self.properties:
            p.Write()
            print '\t%s properties:%s' % (self.name, p.GetName())
            continue

        if i_opened:
            tfile.Close()
            del tfile
        return

    pass


if '__main__' == __name__:
    import sys
    filename = sys.argv[1]

    gs = GenericSurface('TestSurface', model='glisur', type='dielectric_metal', finish='polished')
    gs.add_parameter('first','pvFirstVolume')
    gs.add_parameter('second','pvSecondVolume')
    ref = ROOT.TGraph()
    ref.SetName("REFLECTIVITY")
    ref.SetPoint(0,1,1);
    ref.SetPoint(1,6,1);
    gs.add_property_tgraph(ref);
    gs.write(filename)

