#!/usr/bin/env python
'''
Plot Edep and dE/dx from cowbells tree
'''

import ROOT

import matlist
def treefiles(filenames, treename = "cowbells"):
    'return a list of tuples (ttree,tfile) for each filename'
    ret = []
    for filename in filenames:
        f = ROOT.TFile.Open(filename)
        ROOT.SetOwnership(f,0)
        t = f.Get(treename)
        ret.append((t,f))
        continue
    return ret

class Plotter:

    min_energy = 0.0            # MeV
    max_energy = 2500.0         # MeV
    energy_per_bin = 10         # MeV

    min_delta_energy = 0.0      # MeV/cm
    max_delta_energy = 10.0     # MeV/cm
    delta_energy_per_bin = .1   # MeV/cm

    def __init__(self, particle, material, data):
        '''
        Make a plotter for particle and material.  Data should be a
        list of corresponding tuples of (energy_in_mev, tree).
        '''
        matnum = matlist.index(material)
        self.particle = particle
        self.particle_varname = particle.replace('+','p').replace('-','m').lower()
        self.material = material
        self.material_varname = material.lower()
        self.data = data
        self.energies = []
        self.trees = []
        for e,t in data:
            self.energies.append(e)
            self.trees.append(t)

        self._hists = {}
        common_cut = 'mat1==%d&&mat2==%d&&parentid==0'%(matnum,matnum)
        self._hist_def = {
            'edep':('10*edep/dist:energy1',common_cut),
            'ediff':('10*(energy1-energy2)/dist:energy1',common_cut),
            }

        print 'Plotter for %s/%s(#%d) with %d files and %d plots: %s' % \
            (particle, material, matnum, 
             len(data), len(self._hist_def), str(self.kinds()))
        return

    def kinds(self):
        return self._hist_def.keys()

    def histdesc(self,axis = 'energy'):
        '''
        Return tuple of (nbins,min,max) histogram description
        '''
        if axis.lower() == "energy":
            maxe = Plotter.max_energy
            mine = Plotter.min_energy
            enum = int((maxe-mine) / Plotter.energy_per_bin)
            return (enum, mine, maxe)
        if axis.lower() == "dedx":
            maxe = Plotter.max_delta_energy
            mine = Plotter.min_delta_energy
            enum = int((maxe-mine) / Plotter.delta_energy_per_bin)
            return (enum, mine, maxe)

        return None

    def makehist2d(self, name, title):
        args = self.histdesc('energy') + self.histdesc('dedx')
        h = ROOT.TH2F(name,title,*args)
        ROOT.SetOwnership(h,0)
        return h

    def hist2d_one(self, kind, energy, tree, formula, cuts):
        '''
        Fill and return a TH2F from the given tree
        '''
        name = '%s_%s_%s_%d' % \
            (kind, self.particle_varname, self.material_varname, energy)
        title = '%s: %s in %s at %s' % \
            (kind, self.particle, self.material, energy)
        h = self.makehist2d(name,title)
        tree.Draw('%s>>%s'%(formula,h.GetName()),cuts,'goff')
        return h

    def hists(self, kind):
        '''
        Generate individual histograms
        '''
        hist_list = self._hists.get(kind)
        if hist_list: return hist_list
        hist_list = []

        fc = self._hist_def.get(kind)
        if not fc:
            msg = 'No histograms of the kind %s',kind
            raise ValueError, msg

        for e,t in self.data:
            hist_list.append(self.hist2d_one(kind, e, t, *fc))
            continue
        self._hists[kind] = hist_list
        return hist_list

    def merged(self, kind):
        name = '%s_%s_%s' % \
            (kind, self.particle_varname, self.material_varname)
        title = '%s: %s in %s' % \
            (kind, self.particle, self.material)
        m = self.makehist2d(name,title)
        m.SetXTitle("Energy (MeV)")
        m.SetYTitle("Energy/step (MeV/cm)")
        m.SetStats(0)

        for h in self.hists(kind):
            m.Add(h)
        return m
        

    pass

file_pattern='%(particle)ss-????-%(material)s-%(number)s-%(stepsize)s.root'
def test_plotter(particle, material, number='10000', stepsize='1.0'):
    '''
    return a plotter
    '''
    import glob
    fglob = glob.glob(file_pattern % locals())
    print 'Using files:', fglob
    data = []
    for tree,tfile in treefiles(fglob):
        parts = tfile.GetName().split('-')
        energy = int(parts[1])
        data.append((energy,tree))
        continue
    data.sort()
    return Plotter(particle, material, data)

