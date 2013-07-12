#!/usr/bin/env python
'''
A btdtwf process for generating a geometry configuration file.
'''

import sys
from cowbells import units
from btdtwf.parameter import ParameterSet, FileName, String

from cowbells import geom, default
default.all()

input_parameters = ParameterSet(
    geogen_builder = String('', desc='Detector builder name'),
    geogen_params = String('', desc='Comma-separated key=value list of overriding params'),
    geofile = FileName('{geogen_builder}.json',hint='output', 
                       desc='Detector configuration file')
)

output_parameters = ParameterSet(
    )

def parse_args(**kwds):
    builder = kwds['geogen_builder']
    if not builder:
        raise ValueError, 'geogen: no builder given'

    params = kwds['geogen_params'] or dict()
    if params:
        print 'GEOGEN PARAMS: "%s"' % params
        params = {k:v for k,v in [kv.split('=') for kv in params.split(',')]}

    fname = kwds['geofile']
    if not fname:
        raise ValueError, 'geogen: no output file given'

    print builder,params,fname
    return builder,params,fname


def callable(ex, **kwds):
    builder,params,fname = parse_args(**kwds)

    modname = 'cowbells.builder.%s' % builder 
    exec ('import %s' % modname)
    mod = sys.modules[modname]
    b = mod.Builder(**params)
    worldv = b.top()
    pvTop = 'pvWorld'
    geom.placements.PhysicalVolume(pvTop,worldv)
    b.place()
    b.sensitive()
    
    with open(fname, 'w') as fp:
        fp.write(geom.dumps_json())

    return dict()

