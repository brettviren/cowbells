#!/usr/bin/env python
'''
Plots using cowbell's "steps" output module
'''

import os
import cowbells
ROOT = cowbells.ROOT

canvas = ROOT.TCanvas("steps", "steps", 800, 600)

run_dir = "/home/bviren/work/wbls/refactor/run/nsrl-13a-wbls"
#default_file = "nsrl-13a-reflections/nsrl-13a-2gev-protons-water-10percent.hits-steps.root"
water_1k = "nsrl-13a-2gev-protons-water.hits-steps.1k.root"
water_file = os.path.join(run_dir, water_1k)

wbls01_10 = "nsrl-13a-2gev-protons-wbls01.hits-steps.root"
wbls01_1k = "nsrl-13a-2gev-protons-wbls01.hits-steps.1k.root"
wbls01_file = os.path.join(run_dir,wbls01_1k)


def make_tree(filename = water_file):
    f = ROOT.TFile.Open(filename)
    try:
        t = f.Get("cowbells")
    except ReferenceError:
        print 'Failed to get tree "cowbells" out of file %s' % filename
        raise
    return t

def entry_to_cut(entry):
    if entry is None:
        return "Entry$ < 10"
    return "Entry$ == %d" % entry

def is_pmt(hit):
    return hit.hcId() == 1

def is_upstream(hit):
    return is_pmt(hit) and hit.volId() == 1

def usds_hits(hits):
    'Return number of hits in (upstream, downstream) PMT'
    us = ds = 0
    for hit in hits:
        if not is_pmt(hit):
            continue
        if is_upstream(hit):
            us += 1
        else:
            ds += 1
    return us,ds

def track_dedx_in_material(steps, material, trackid = 1):
    '''
    Return tuple of total (pathlen, eloss, enoni) for the track ID <trackid>
    in material <material>.
    '''
    enoni = eloss = pathlen = 0
    for step in steps:
        if step.trackid != trackid: 
            continue
        if step.mat1 != material and step.mat2 != material:
            continue
        enoni +=  step.enoni
        eloss += step.energy1 - step.energy2
        pathlen += step.dist
    return (pathlen, eloss, enoni)

def hits_per_dedx(tree, entry = None, material=1, **kwds):
    entry = entry_to_cut(entry)
    
    us_hpe = ROOT.TH1D('us_hpe', 'Upstream hits per energy lost in {sample}'.format(**kwds),
                       1000,0,1)
    ds_hpe = ROOT.TH1D('ds_hpe', 'Downstream hits per energy lost in {sample}'.format(**kwds),
                       1000,0,10)

    us_hpmdedx = ROOT.TH1D('us_hpmdedx', 'Upstream hits per mean de/dx in {sample}'.format(**kwds),
                           1000,0,100)
    ds_hpmdedx = ROOT.TH1D('ds_hpmdedx', 'Downstream hits per mean de/dx in {sample}'.format(**kwds),
                           1000,0,1000)

    for entry in tree:
        nhit_us, nhit_ds = usds_hits(entry.event.hc)
        dx,de,denoni = track_dedx_in_material(entry.event.steps, material)
        
        if de:
            us_hpe.Fill(nhit_us/de)
            ds_hpe.Fill(nhit_ds/de)
            us_hpmdedx.Fill(nhit_us*dx/de)
            ds_hpmdedx.Fill(nhit_ds*dx/de)
        continue
    return (us_hpe, ds_hpe, us_hpmdedx, ds_hpmdedx)

def draw_four(hists):
    canvas.Clear()
    canvas.Divide(2,2)
    for n in range(4):
        pad = canvas.cd(n+1)
        pad.SetLogy()
        hists[n].Draw()
    return


def xray(tree, entry = None):
    entry = entry_to_cut(entry)
    cut = "abs(steps.z1)<100 && abs(steps.x1)<100 && abs(steps.y1)<100 && %s" % entry
    canvas.Clear()
    canvas.Divide(2,2)

    pad = canvas.cd(1)
    tree.Draw("steps.x1:steps.z1",cut)

    pad = canvas.cd(2)
    tree.Draw("steps.y1:steps.x1:steps.z1",cut)

    pad = canvas.cd(3)
    tree.Draw("steps.y1:steps.z1",cut)

    pad = canvas.cd(4)
    tree.Draw("steps.y1:steps.x1",cut)

class xray_plots(object):
    def __init__(self, label = "", dirname='images/steps', nbins = 400, force=False):
        self.label, self.dirname, self.force = label, dirname, force
        self.filename = 'xray-%s.png' % self.label
        self.xy = ROOT.TH2F('xray_xy', 'Steps in XY for %s'%label,
                            nbins,-100,100, nbins,-100,100,)
        self.zy = ROOT.TH2F('xray_zy', 'Steps in ZY for %s'%label,
                            nbins,-100,100, nbins,-100,100,)
        self.zx = ROOT.TH2F('xray_zx', 'Steps in ZX for %s'%label,
                            nbins,-100,100, nbins,-100,100,)
    def exists(self):
        if self.force: return None
        fn = print_filename(self.filename, self.dirname)
        if os.path.exists(fn):
            return fn
        return None

    def fill(self, steps):
        for step in steps:
            self.xy.Fill(step.x1,step.y1)
            self.zy.Fill(step.z1,step.y1)
            self.zx.Fill(step.z1,step.x1)
        return None
    def plot(self, opt="colz"):
        fn = self.exists()
        if fn: return fn
        canvas.Clear()
        canvas.Divide(2,2)
        canvas.cd(1) 
        self.zx.Draw(opt)
        canvas.cd(3)
        self.zy.Draw(opt)
        canvas.cd(4)
        self.xy.Draw(opt)
        return cprint(self.filename, self.dirname)


def xrayplot(sample, **kwds):
    tree = make_tree(root_file(sample))
    fns = []
    nsingle = 5
    ntotal = 10
    for entry in range(nsingle):
        xrp = xray_plots('%s-%d' % (sample, entry), **kwds)
        fn = xrp.exists()
        if not fn:
            tree.GetEntry(entry)
            xrp.fill(tree.event.steps)
            fn = xrp.plot()
        fns.append(fn)
    xrp = xray_plots('%s-many' % (sample,), **kwds)
    fn = xrp.exists()
    if not fn:
        for entry in range(nsingle,ntotal+nsingle):
            tree.GetEntry(entry)
            xrp.fill(tree.event.steps)
        fn = xrp.plot()
    fns.append(fn)
    return fns

def print_filename(filename, dirname="images/steps"):
    fn = os.path.join(dirname, filename)
    name,ext = os.path.splitext(fn)
    return name.replace('.','_') + ext # latex hates .'s in the filename
    
def cprint(filename, dirname="images/steps"):
    fn = print_filename(filename, dirname)
    ext = os.path.splitext(fn)[1][1:]
    print 'Printing %s as %s' % (fn,ext)
    canvas.Print(fn,ext)
    return fn


def root_file(sample):
    if sample.lower() == 'water':
        return water_file
    elif sample.lower() == 'wbls01':
        return  wbls01_file
    raise ValueError,  'Unknown sample: "%s"' % sample
def material_number(sample):
    if sample.lower() == 'water':
        return 1
    elif sample.lower() == 'wbls01':
        return  9
    raise ValueError,  'Unknown sample: "%s"' % sample



def dedxplot(sample, **kwds):
    fname = '%s-hits-per-mev.pdf' % sample.lower()
    path = print_filename(fname)
    if not kwds.get('force') and os.path.exists(path):
        print 'Plot already exists: %s' % path
        return path

    t = make_tree(root_file(sample))
    h = hits_per_dedx(t, material=material_number(sample), sample=sample)
    draw_four(h)
    path = cprint(fname)
    return path
  
def dedxplot_twosample(sample1, sample2, **kwds):
    fname = '%s-%s-hits-per-mev.pdf' % (sample1.lower(), sample2.lower())
    path = print_filename(fname)
    if not kwds.get('force') and os.path.exists(path):
        print 'Plot already exists: %s' % path
        return path

    t1 = make_tree(root_file(sample1))
    h1 = hits_per_dedx(t1, material=material_number(sample1), sample=sample1)
    t2 = make_tree(root_file(sample2))
    h2 = hits_per_dedx(t2, material=material_number(sample2), sample=sample2)
    draw_four(h1[:2] + h2[:2])
    path = cprint(fname)
    return path
    
