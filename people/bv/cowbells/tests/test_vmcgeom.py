#!/usr/bin/env python
'''
Test starting up VMC with a pre-prepared ROOT geometry
'''

import ROOT
def load_libs(extra = None):
    libs = ["RIO","Geom", "VMC", "Physics"]

    from subprocess import check_output
    chunks = check_output(['geant4-config','--libs']).split('\n')[0].split()
    for chunk in chunks:
        if chunk[:2] == '-l':
            libs.append(chunk[2:])
        continue
    libs += ['ClhepVGM','BaseVGM','Geant4GM','RootGM','XmlVGM',
             'G3toG4','g4root','geant4vmc']
    if extra:
        libs += extra

    for lib in libs:
        if lib[:3] != 'lib':        # ROOT expects the 'lib' prefix
            lib = 'lib' + lib
        ok = ROOT.gSystem.Load(lib)
        if ok < 0:
            raise RuntimeError, 'Failed to load "%s"' % lib
        print 'Loaded',lib
        continue
    return

def make_geom():
    geomfile = 'geo.root'
    geo = ROOT.TGeoManager.Import(geomfile)

def make_app(which = 'cowbells'):
    if which == 'cowbells':
        app = ROOT.CowMCapp('cowbells','COsmic WB(el)LS simulation')
    elif which == 'example06':
        app = ROOT.Ex06MCApplication('Ex06','G4VMC Example 06')
    else:
        app = None

    ROOT.SetOwnership(app,0)
    return app

def make_mc(geom_style):
    print 'Setting runconfig to geometry style "%s"' % geom_style
    run_config = ROOT.TG4RunConfiguration(geom_style, "emStandard+optical")

    print 'Making the TGeant4'
    geant4 = ROOT.TGeant4("TGeant4", "The Geant4 Monte Carlo", run_config)
    ROOT.SetOwnership(geant4,0)
    return geant4

if __name__ == '__main__':
    import sys

    which = sys.argv[1]

    load_libs([which])

    geo = make_geom()

    # geomRoot or geomRootToGeant4
    app = make_app(which)

    mc = make_mc(sys.argv[2])

    print 'Init MC'
    g4config = 'g4Config.C'
    fp = open(g4config,"w")
    fp.write('void Config() { }\n')
    fp.close()
    app.InitMC(g4config)

    print 'Run MC'
    app.RunMC(10)


    
