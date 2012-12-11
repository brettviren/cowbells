#!/usr/bin/env python
'''
Calculate dE/dx for particles through material using truncated Bethe
formula (eqn 30.10 from 2010 PDG review)

Units: cm, MeV
'''

import math
import ROOT

mass_electron = 0.511 # MeV
mass_muon   = 105.7   # MeV

water = [("hydrogen",2*1.0),
         ("oxygen",1*18.0)]
wbls = [('Hydrogen', 0.1097),
        ('Oxygen', 0.8234),
        ('Sulfur', 0.0048),
        ('Nitrogen', 0.0001),
        ('Carbon', 0.0620)]
supported_mixtures = {
    "water":water,
    "wbls":wbls,
}

kay_over_A = 0.307075 # MeV cm2/g for A=1

def weight(mix, which):
    num = 0
    den = 0
    for count,(n,A) in enumerate(which):
        term = n*A
        if count == which:
            num = term
        den += term
        continue
    return num/den

def tmax(kine, mass):
    '''
    Maximum energy transfer possible to electrons in a single collision.

    eqn 30.5 in 2012 PDG
    '''
    bg2 = (kine/mass+1)**2 - 1  # (beta*gamma)^2
    gamma = math.sqrt(bg2+1)
    mrat = mass_electron / mass

    return (2*mass_electron * bg2) / (1 + 2*gamma*mrat + mrat*mrat)

class DEDX(object):
    '''
    Calculate dE/dx for a particle and a material.
    
    Configure with

    particle : name of particle

    material : name of material

    tcut : if restricted de/dx is calculated this sets the cut, else 0
    
    '''

    def __init__(self,**kwds):
        self.config(**kwds)
        return

    def __call__(self,kine):
        '''
        Return dE/dx for the given energy
        '''
        return self.dedx_restricted(kine)

    def config(self,**kwds):
        self.__dict__.update(kwds)
        self.update()
        return

    def update(self):
        '''
        Update internal calculations
        '''

        self.update_material()
        self.update_particle()
        return

    def update_material(self):
        '''
        Set the material properties
        '''
        # from figure 30.5 of 2012 PDG

        matprob = {
            'hydrogen': ( 1,  1, 19.2e-6),
            'carbon':   ( 6, 12, 13.0e-6),
            'nitrogen': ( 7, 14, 11.6e-6),
            'oxygen':   ( 8, 18, 11.6e-6),
            'sulfur':   (16, 32, 11.0e-6),
            }

        mp = matprob.get(self.material.lower())
        if not mp: return
        self._Z = mp[0]
        self._A = mp[1]
        self._mean_excitation_energy = mp[2]*self._Z

        return

    def update_particle(self):
        '''
        Set the particle properties
        '''
        self._charge_number = 1
        if self.particle in ['e-','e+','electron','positron']:
            self._mass = 0.511
        if self.particle[:2] == "mu":
            self._mass = 105.7
        if self.particle == "proton":
            self._mass = 938.272
        return

    def dedx_restricted(self,kine):
        '''
        dE/dx restricted by a kinetic energy cut

        eqn 30.10 in 2012 PDG
        '''

        bg2 = (kine / self._mass+1)**2 - 1  # (beta*gamma)^2
        b2 = bg2 / (bg2+1)                 # (beta)^2
        K = kay_over_A
        t_max = tmax(kine, self._mass)

        pre = K * self._charge_number**2 * self._Z / self._A / b2
        one = 2*mass_electron*bg2*self.tcut / self._mean_excitation_energy**2
        one = 0.5*math.log(one)
        two = 0.5*b2*(1+self.tcut/t_max)
        # note: density effect (delta in eqn 30.10) not included!

        # print ' '.join(['%s:%.2e'%(s,x) for s,x in [("bg^2",bg2),
        #                                             ("bg",math.sqrt(bg2)),
        #                                             ("beta^2",b2),
        #                                             ("beta",math.sqrt(b2)),
        #                                             ("K",K),
        #                                             ("pre",pre),
        #                                             ("t1",one),
        #                                             ("t2",two),
        #                                             ("tmax",t_max),
        #                                             ]])
        return pre * (one - two)

    pass

class MixDEDX(object):
    def __init__(self, matdesc, **kwds):

        print matdesc
        if isinstance(matdesc, str): 
            matdesc = supported_mixtures.get(matdesc.lower())

        self.dedx = []
        self.weight = []
        for name, massfrac in matdesc:
            self.dedx.append(DEDX(material=name,**kwds))
            self.weight.append(massfrac)
            continue
        tot = sum(self.weight)
        #print tot, self.weight
        self.weight = [w/tot for w in self.weight]
        return

    def __call__(self, kine):
        tot = 0;
        for w,f in zip(self.weight,self.dedx):
            tot += w * f(kine)
        return tot

    def graph(self, mev_range = range(10,10000)):
        '''
        Return a TGraph rep of the function
        '''
        g = ROOT.TGraph()
        ROOT.SetOwnership(g,0)
        for mev in mev_range:
            g.SetPoint(g.GetN(),mev,self(mev))
        return g


if '__main__' == __name__:
    import ROOT

    canvas = ROOT.TCanvas()
    canvas.Print("dedx_calc.pdf[","pdf")

    # values for [PDF] 
    # MUON STOPPING POWER AND RANGE TABLES 10 MeV-100 ...
    # pdg.lbl.gov/2008/AtomicNuclearProperties/adndt.pdf
    adndt = {
        'water':[(10,45.9),(100,7.29),(1000,2.210),(10000,2.132)],
        'hydrogen':[(10,101.7),(100,15.29),(1000,4.496),(10000,4.539)],
        }

    graphs = []
    for mat in ["water","wbls","hydrogen","oxygen"]:
        title = "dE/dx for mu in %s" % mat

        frame = canvas.DrawFrame(1.0,0, math.log10(10000),10)
        frame.SetTitle(title)
        frame.SetXTitle("log10(E/MeV)")
        frame.SetYTitle("dE/dx (MeV cm^{2}/g)")

        g = ROOT.TGraph()
        graphs.append(g)
        g.SetName("dedxmu_%s" % mat)
        g.SetLineWidth(2)

        print mat
        
        kwds = dict(particle="mu+",tcut=0.35)
        mix = supported_mixtures.get(mat.lower())
        if mix:
            f = MixDEDX(mix, **kwds)
        else:
            f = DEDX(material=mat, **kwds)

        for mev in range(10,10000):
            g.SetPoint(g.GetN(),math.log10(mev),f(mev))
            continue
        g.Draw("L")

        adata = adndt.get(mat)
        if adata:
            ag = ROOT.TGraph()
            for c,(x,y) in enumerate(adata):
                ag.SetPoint(c,math.log10(x),y)
            ag.SetLineColor(2)
            ag.SetMarkerColor(2)
            ag.Draw("*")

        canvas.Print("dedx_calc.pdf","pdf")
        continue
    canvas.Print("dedx_calc.pdf]","pdf")

    
