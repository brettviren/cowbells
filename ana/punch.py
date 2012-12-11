#!/usr/bin/env python
'''
Make plots showing the particle's ability to punch through a volume
'''

import ROOT


enter_exit_desc = {
    'enter': (1,'trackid==1 && mat1==11 && mat2==10'),
    'exit': (2,'trackid==1 && mat1==10 && mat2==11'),
    }


class PunchPlots(object):
    def __init__(self, rootfile, pdffile):
        self.tfile = ROOT.TFile.Open(rootfile)
        self.tree = self.tfile.Get("cowbells")
        self.pdffile = pdffile
        self.outroot = ROOT.TFile.Open(pdffile+'.root','recreate')
        self.canvas = ROOT.TCanvas("canvas","Punch Plots", 500, 400)
        self.cprint("[")
        return

    def clear(self): self.canvas.Clear()
    def cprint(self, extra = ""):
        if not self.canvas: return
        self.canvas.Modified()
        self.canvas.Update()
        self.canvas.Print(self.pdffile + extra, 'pdf')
        self.outroot.cd()
        self.canvas.Write()
        return

    def __del__(self): self.close()
    def close(self):
        if not self.canvas: return
        self.cprint(']')
        self.outroot.Close()
        self.canvas = None
        return


    def plot_energy(self, passing = 'enter'):
        '''
        Plot the energy spectrum of the primary particle entering or
        exiting the tub
        '''
        self.clear()
        self.canvas.SetLogy()

        desc = enter_exit_desc[passing]
        self.tree.Draw('energy%d'%desc[0],desc[1])

        f = self.canvas.FindObject("htemp")
        f.SetTitle("Primary particle kinetic energy at %s point" % passing)
        f.SetXTitle('T (MeV)')

        self.cprint()
        self.canvas.SetLogy(False)
        return

    def plot_zy(self, passing = 'enter'):
        '''
        Plot Z vs Y for primary particle entering or exiting the tub
        '''
        self.clear()
        self.canvas.SetLogz()

        h = ROOT.TH2D('zy_'+passing,'Z vs Y for %s' % passing,
                      200, -10,10, 200, -10,10)
        h.SetXTitle('Y (mm)')
        h.SetYTitle('Z (mm)')

        desc = enter_exit_desc[passing]
        self.tree.Draw('z%d:y%d>>%s'%(desc[0],desc[0],h.GetName()),
                       desc[1], 'goff')
        h.Draw('colz')

        self.move_stats(h, style='colz')

        self.cprint()
        self.canvas.SetLogz(False)
        return
                                      


    def move_stats(self, hist= 'htemp', **kwds):
        '''
        Move stats box.
        '''
        self.canvas.Modified()
        self.canvas.Update()

        if isinstance(hist, str):
            hist = self.canvas.FindObject(hname)
            if not hist:
                print 'Failed to find histogram "%s" in canvas "%s" not moving stats' % \
                    (hname, self.canvas.GetName())
                return
            pass
        stats = hist.FindObject("stats")
        if not stats:
            print 'Failed to find stats in histogram "%s" not moving them' % hname
            return

        dx = dy = 0.0
        x1 = stats.GetX1NDC()
        x2 = stats.GetX2NDC()
        y1 = stats.GetY1NDC()
        y2 = stats.GetY2NDC()
        move = 'relative'

        style = kwds.get('style')
        if style:
            opts = style.split()
            if 'colz' in opts: 
                dx = -0.1
                pass
            pass
        
        if move == 'relative':
            x1 += dx
            y1 += dy
            x2 += dx
            y2 += dy

        stats.SetX1NDC(x1)
        stats.SetY1NDC(y1)
        stats.SetX2NDC(x2)
        stats.SetY2NDC(y2)
        return


    def plot_r2(self, passing= 'enter'):
        '''
        Plot r^2 for primary particle entering or exiting the tub
        '''

        self.clear()
        self.canvas.SetLogy()

        desc = enter_exit_desc[passing]
        what = 'z%(n)d**2 + y%(n)d**2' % {'n':desc[0] }
        self.tree.Draw(what, desc[1])

        f = self.canvas.FindObject("htemp")
        f.SetTitle("r^2 at %s point" % passing)
        f.SetXTitle('r^2 (mm^2)')

        self.cprint()
        self.canvas.SetLogy(False)
        return

if __name__ == '__main__':
    import sys
    pp = PunchPlots(sys.argv[1],sys.argv[2])

    pp.plot_energy('enter')
    pp.plot_energy('exit')

    pp.plot_zy('enter')
    pp.plot_zy('exit')

    pp.plot_r2('enter')
    pp.plot_r2('exit')

    pp.close()
