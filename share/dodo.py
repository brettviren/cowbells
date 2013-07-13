#!/usr/bin/env doit

import sys
import ConfigParser
from doit import get_var
from doit.action import CmdAction

config = dict(
    cfgfile = get_var('cfgfile', 'cowbells.cfg'),
    geosec = get_var('geosec', 'geo'),
    simsec = get_var('simsec', 'sim'),
    geofile = get_var('geofile', 'geo.json'),
    simfile = get_var('simfile', 'sim.root'),
)


def geometry(cfgfile, section, targets):
    from cowbells import geom, default
    default.all()

    cfg = ConfigParser.SafeConfigParser()
    cfg.read(cfgfile)

    bmodname = cfg.get(section, 'builder')
    bmodoptsec = cfg.get(section, 'builder_options')
    bmodargs = dict(cfg.items(bmodoptsec))

    exec ('import ' + bmodname)
    bmod = sys.modules[bmodname]

    builder = bmod.Builder(**bmodargs)
    geom.placements.PhysicalVolume('pvWorld', builder.top())
    builder.place()
    builder.sensitive()

    with open(targets[0], 'w') as fp:
        fp.write(geom.dumps_json())

def task_gen():
    'Create the geometry description file'
    return dict(
        actions= [(geometry,
                   (config['cfgfile'], config['geosec'],))],
        file_dep= [config['cfgfile']],
        targets= [config['geofile']],
        clean= True,
    )


def create_cowbells_cmdline():
    'create cowbells.exe'
    cmdpat = '{exe} -n {nevents} -p {physics} -m {outmod} '
    cmdpat += '-k kin://{kin}?vertex={x},{y},{z}\&name={particle}\&direction={dx},{dy},{dz}\&energy={energy} '
    cmdpat += '-o {simfile} {geofile}'
    
    p = dict(x=0, y=0, z=0, dx=0, dy=0, dz=0,
             kin = 'beam', particle='proton', energy='1000',
             physics = 'op,em', outmod='hits,steps',
             nevents = 1,
             exe = 'cowbells.exe', extraargs=''
    )

    cfg = ConfigParser.SafeConfigParser()
    cfg.read(config['cfgfile'])
    p.update(config.items())
    p.update(cfg.items(config['simsec']))
    cmd = cmdpat.format(**p)
    print 'COMMAND:',cmd
    return cmd

def task_sim():
    return dict(
        actions= [CmdAction(create_cowbells_cmdline)],
        file_dep= [config['geofile'], config['cfgfile']],
        targets= [config['simfile']],
        clean= True,
    )

