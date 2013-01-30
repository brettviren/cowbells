#!/usr/bin/python
'''
Run the study
'''
import os
import sys
import ROOT

from util import StringParams

class BaseRun(object):

    def __init__(self, in_ext, out_ext, params):
        self.p = params
        self.p.infile = self.p.outfile = ""
        if in_ext:
            self.p.infile = self.filename(in_ext)
        if out_ext:
            self.p.outfile = self.filename(out_ext)

    def dorun(self):
        'Decide if we should run'
        if self.p.force:        
            return True         
        if not self.p.outfile: 
            return True
        if not os.path.exists(self.p.outfile):
            return True
        
        ostat = os.stat(self.p.outfile)
        if not ostat.st_size:
            return True

        if self.p.infile:       # last chance, if input is newer than output
            istat = os.stat(self.p.infile)
            if istat.st_mtime > ostat.st_mtime:
                return True

        return False            # do not run



    def __call__(self):
        if self.p.infile and not os.path.exists(self.p.infile):
            raise RuntimeError, 'No input file: %s' % self.p.infile
            
        if self.dorun():        # da doo run run run, 
            self.run()          # da doo run run

        if not self.p.outfile:
            print 'No output requested'
            return

        if not os.path.exists(self.p.outfile):
            raise RuntimeError, 'Failed to produce file: %s' % self.p.outfile

        return

    def filename(self, ext = None):
        '''
        Return the filename associated with my parameters
        '''
        base = '%(study)s-%(sample)s-%(particle)s-%(energy)sMeV-%(nevents)sevts'
        base = self.p.string(base)
        if ext:
            return '.'.join([base,ext])
        return base

class ConfigSingleTubRun(BaseRun):
    '''
    Generate the config file for a single Tub
    '''
    def __init__(self, params):
        super(ConfigSingleTubRun,self).__init__(None, "json", params)
        return

    def run(self):
        from cowbells import geom, default
        from cowbells.builder import tubdet, world

        default.all()

        b = tubdet.World( tub = self.p.tub.capitalize(),
                          sample = self.p.sample.capitalize(),
                          inner_diameter = self.p.inner_diameter,
                          inner_height = self.p.inner_height,                          
                          )

        worldlv = b.top()
        geom.placements.PhysicalVolume('pvWorld',worldlv)    
        b.place()
        b.sensitive()

        print 'Writing %s' % self.p.outfile
        fp = open(self.p.outfile, 'w')
        fp.write(geom.dumps_json())
        fp.close()
        return 


class SimRun(BaseRun):
    '''
    Run cowbells.exe
    '''

    prog = "cowbells.exe"   
    args = "-k kin://beam?vertex=%(x)f,%(y)f,%(z)f&name=%(particle)s&direction=%(dx)f,%(dy)f,%(dz)f&energy=%(energy)s -p %(physics)s  -o %(outfile)s -n %(nevents)s %(infile)s"

    def __init__(self, params):
        super(SimRun,self).__init__("json", "root", params)
        return

    def run(self):
        from subprocess import Popen, PIPE, STDOUT

        args = self.p.string(self.args)
        cmd = "%s %s" % (self.prog, args)

        print 'Command: %s' % cmd
        try:
            proc = Popen(cmd.split(), stdout=PIPE, stderr=STDOUT, 
                         universal_newlines=True)
        except OSError, err:
            print 'Error running: %s' % cmd
            raise

        res = None
        while True:
            line = proc.stdout.readline()
            res = proc.poll()
            if line:
                sys.stdout.write(line)
                sys.stdout.flush()
            if res is None: 
                continue
            for line in proc.stdout.readlines():
                sys.stdout.write(line)
            sys.stdout.flush()
            break

        return


class TreePlotRun(BaseRun):
    def __init__(self, params):
        super(TreePlotRun,self).__init__("root","pdf", params)
        self.tfile = ROOT.TFile.Open(self.p.infile)
        self.tree = self.tfile.Get(self.p.tree) # set in params
        self.canvas = ROOT.TCanvas("canvas","canvas")
        return

    def cprint(self, extra=""):
        self.canvas.Print(self.p.outfile+extra,'pdf')
        return


    def canvas_get_hist(self, newname, oldname = 'htemp'):
        h = self.canvas.GetPrimitive(oldname)
        if not h: 
            raise RuntimeError, 'Failed to get histogram "%s"' % oldname
        return h.Clone(newname)

    def dress_hist(self, hist, **params):
        p = StringParams(**params)
        hist.SetTitle(p.get('title', hist.GetTitle()))
        hist.SetXTitle(p.get('xtitle', hist.GetXaxis().GetTitle()))
        hist.SetYTitle(p.get('ytitle', hist.GetYaxis().GetTitle()))
        hist.SetLineColor(int(p.get('color',hist.GetLineColor())))
        return hist

    pass



def make_args_parser(p):
    import argparse
    parser = argparse.ArgumentParser(description = 'Run a study')
    parser.add_argument('--force', default = "", action='store_const', const="force",
                        help='Run stages even if output exists')
    for k,v in p.dict().iteritems():
        parser.add_argument('--%s'%k, default=v, help='default is %s' % (v,))
    parser.add_argument('stage', nargs='+', help='Specify the stages to run')
    return parser

    

def main(study, args):
    study_mod = __import__(study)
    params = study_mod.params()
    cmdline = make_args_parser(params)
    opts = cmdline.parse_args(args)
    for k,v in opts.__dict__.iteritems():
        params.set(k,v)

    if 'help' in opts.stage:
        cmdline.print_help()
        sys.exit(1)

    print 'Configured options:'
    for k,v in sorted(opts.__dict__.iteritems()):
        print '\t%s = %s' % (k,v)

    for stage_name in opts.stage:
        stage_class_name = stage_name.capitalize() + 'Run'
        print 'Study "%s" stage "%s"' % (study, stage_name)
        stage_class = study_mod.__dict__[stage_class_name]
        s = stage_class(params.copy())
        s()

if __name__ == '__main__':
    study = sys.argv[1]
    args = sys.argv[2:]
    main(study, args)

