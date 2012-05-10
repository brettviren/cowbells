#!/usr/bin/env python
'''
Function to boot up major facilites.

Failures will raise RuntimeError
'''

import ROOT
def root_load_lib(lib):
    'Use ROOT to load a library'

    if lib[:3] != 'lib':        # ROOT expects the 'lib' prefix
        lib = 'lib' + lib

    ok = ROOT.gSystem.Load(lib)
    if ok < 0:
        raise RuntimeError, 'Failed to load "%s"' % lib
    #print 'Loaded "%s"' % lib
    return

# boot ROOT and load extra libs
_root_loaded = None
def root():
    'Load extra ROOT libs'
    global _root_loaded
    if _root_loaded: return
    for lib in ["RIO","Geom"]:
        root_load_lib(lib)
        continue
    _root_loaded = True
    return

# Load Geant4 libs
# load libs from geant4-config --libs, 
_geant4_loaded = None
def geant4():
    'Load Geant4 libs, relies on the geant4-config script'
    global _geant4_loaded
    if _geant4_loaded: return

    from subprocess import check_output
    chunks = check_output(['geant4-config','--libs']).split('\n')[0].split()
    ldir = None
    libs = []
    for chunk in chunks:
        if '-L' == chunk[:2]: # if geant4-config is in our path, 
            continue          # assume LD_LIBRARY_PATH is okay too
        if '-l' == chunk[:2]:
            root_load_lib(chunk[2:])
            continue
        print 'Unknown output from geant4-config skipped: "%s"' % chunk
        continue

    _geant4_loaded = True
    return

# Load VGM libs
_various_loaded = None
def load_various():
    'Load various libraries'
    global _various_loaded
    if _various_loaded: return

    root()
    geant4()

    for lib in ['ClhepVGM','BaseVGM','Geant4GM','RootGM','XmlVGM','G3toG4','HepMC']:
        root_load_lib(lib)
        continue
    _various_loaded = True
    return

_everything_loaded = None
def everything():
    'Load all libraries including cowbells'
    global _everything_loaded
    if _everything_loaded: return
    load_various()
    root_load_lib('cowbells')
    root_load_lib("cowbellsDict")

    _everything_loaded = True
    return

if __name__ == '__main__':
    everything()

