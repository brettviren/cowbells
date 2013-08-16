#!/usr/bin/env python
'''
Some btdtwf nodes to run parts of a cowbells workflow
'''


import sys
from btdtwf.son.nodes import CallableNode, ProgramCallable
from subprocess import Popen, PIPE, STDOUT


def generate_geometry(connections, builder, geofile, sample = 'Water', **kwds):
    from cowbells import geom, default
    default.all()               # fixme, move this into geom

    exec ('import ' + builder)
    bmod = sys.modules[builder]
    bobj = bmod.Builder(sample=sample)

    geom.placements.PhysicalVolume('pvWorld', bobj.top())
    bobj.place()
    bobj.sensitive()

    with open(geofile,'w') as fp:
        fp.write(geom.dumps_json())

    return geofile

def geometry(**kwds):
    '''
    Return a node that will generate a geometry file
    '''
    return CallableNode(generate_geometry, **kwds)


def run_detsim(connections, cmdpat, logfile='/dev/stdout', **params):
    geofile = connections.keys()[0]()
    params.setdefault('geofile',geofile)

    cmdstr = cmdpat.format(**params)
    print 'Running %s' % cmdstr
    proc = Popen(cmdstr.split(), shell=False, stdout=PIPE, stderr=STDOUT)
    ret = proc.wait()
    with open(logfile,'w') as fp:
        fp.write(proc.stdout.read())
    if ret:
        raise RuntimeError, cmdstr
    return params.get('simfile')
    

def detsim(**kwds):
    '''
    Return a node that will track particles through the detector
    defined in a geometry file and produce some output.
    '''
    defaults = dict(cowbellsexe = 'cowbells.exe',
                    nevents = '100', 
                    physics = 'op,em,had',
                    outmod = 'hits')
    params = dict(defaults, **kwds)
    cmdpat = '{cowbellsexe} -n {nevents} -p {physics} -m {outmod} -k {kinematics_url} '
    cmdpat += '-o {simfile} {geofile}'
    return CallableNode(run_detsim, cmdpat = cmdpat, **params)
