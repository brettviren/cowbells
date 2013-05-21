#!/usr/bin/env python
'''
Make some plots.

This is meant to be used from an org file
'''
import cowbells
ROOT = cowbells.ROOT

def move_stats(h, x=0, y=0):
    '''
    Move stats box of histogram "h" by relative x/y amount.  Histogram
    must already be Draw()n.
    '''
    h.Draw()                    # need to do this to make the initial stats box
    ROOT.gPad.Update()
    s = h.GetListOfFunctions().FindObject("stats").Clone('%s_stats'%h.GetName())
    s.SetX1NDC(s.GetX1NDC() + x)
    s.SetY1NDC(s.GetY1NDC() + y)
    s.SetX2NDC(s.GetX2NDC() + x)
    s.SetY2NDC(s.GetY2NDC() + y)
    ROOT.gPad.Modified()
    return s
    
def plot_hits(tree, canvas, outbase):
    '''
    Plot number of hits per event in each PMT
    '''
    canvas.SetLogy()

    ucolor,dcolor = 2,4

    minn, maxn = 0.0, 1000.0
    nbins = int(0.5*(maxn-minn))

    ush = ROOT.TH1F("ushits","Hits per event in up-stream PMT",
                    nbins, minn, maxn)
    dsh = ROOT.TH1F("dshits","Hits per event down-stream PMT",
                    nbins, minn, maxn)

    # volid: d.s. is 0, u.s. is 1
    # hcid: trigger/hodoscope PMTs are 0, sample PMTs are 1

    tree.Draw("event.@hc.size()>>ushits","hc.volid==1 && hc.hcid == 1")
    ush.SetLineColor(ucolor)
    ustat = move_stats(ush)
    ustat.SetLineColor(ucolor)

    tree.Draw("event.@hc.size()>>dshits","hc.volid==0 && hc.hcid == 1")
    dsh.SetLineColor(dcolor)
    dstat = move_stats(dsh, y=-0.25)
    dstat.SetLineColor(dcolor)

    dsh.SetTitle("Hits/Event in PMTs (blue=downstream, red=upstream)")
    dsh.SetXTitle("Hits per event")

    dsh.Draw()
    ush.Draw("same")
    dstat.Draw("same")
    ustat.Draw("same")
    
    for ext in ['svg','png','pdf']:
        canvas.Print('%s.%s' % (outbase, ext))

    return None

def plot_timing(tree, canvas, outbase):
    '''
    Make hit timing plots
    '''
    canvas.SetLogy()

    ucolor,dcolor = 2,4

    mint, maxt = 17.0, 25.0
    nbins = int((maxt-mint)*100)

    ust = ROOT.TH1F("ustiming","Hit timing in up-stream PMT",
                    nbins, mint, maxt)
    dst = ROOT.TH1F("dstiming","Hit timing in down-stream PMT",
                    nbins, mint, maxt)

    # volid: d.s. is 0, u.s. is 1
    # hcid: trigger/hodoscope PMTs are 0, sample PMTs are 1

    tree.Draw("hc.t>>ustiming","hc.volid==1 && hc.hcid == 1")
    ust.SetLineColor(ucolor)
    ustat = move_stats(ust)
    ustat.SetLineColor(ucolor)

    tree.Draw("hc.t>>dstiming","hc.volid==0 && hc.hcid == 1")
    dst.SetLineColor(dcolor)
    dstat = move_stats(dst, y=-0.25)
    dstat.SetLineColor(dcolor)

    dst.SetTitle("Timing in PMTs (blue=downstream, red=upstream)")
    dst.SetXTitle("Hit times (ns)")

    dst.Draw()
    ust.Draw("same")
    dstat.Draw("same")
    ustat.Draw("same")
    
    for ext in ['svg','png','pdf']:
        canvas.Print('%s.%s' % (outbase, ext))

    return None


def make_file_tree(filename, treename = 'cowbells'):
    tfile = ROOT.TFile.Open(filename)
    tree = tfile.Get(treename)
    return tfile, tree
