#!/usr/bin/env python
'''

'''
#from btdtwf.son.nodes import wrap_connection_keywords, wrap_format_keywords


import os
import sys
import ConfigParser
import btdtwf
import ConfigParser

def make_simple_config(ignored, **kwds):
    cfgfilename = kwds.get('cfgfile','test.cfg')
    print 'make_config("%s")' % cfgfilename

    cfg = ConfigParser.SafeConfigParser()
    cfg.optionxform = str       # why would we want to lose case?
    section = 'defaults'
    cfg.add_section(section)
    cfg.set(section, 'section', 'geometry test')

    section = 'geometry test'
    cfg.add_section(section)
    cfg.set(section, 'builder', 'cowbells.builder.nsrl')
    cfg.set(section, 'builder_options', 'nsrl water')
    cfg.set(section, 'geofile', 'nsrl-{sample}.json')
    section = 'nsrl water'
    cfg.add_section(section)
    cfg.set(section, 'sample', 'Water')

    with open(cfgfilename,'w') as fp:
        cfg.write(fp)
        print 'writing %s in %s' % (cfgfilename, os.getcwd())
    return cfgfilename


def parse_config(filename, **params):
    '''Parse configuration file to provide direction for generating
    geometry description.

    Args:
        filename (string): name of a Python configuration file

    The configuration specifies things like:
    
        [defaults]
        section = mydet
        
        [mydet]
        builder = cowbells.builder.nsrl
        builder_options = nsrl water
        
        [nsrl water]
        Sample = Water
    '''
    
    cfg = ConfigParser.SafeConfigParser()
    cfg.optionxform = str       # why would we want to lose case?
    cfg.read(filename)

    section = params.get('section')
    if not section:
        section = cfg.get('defaults', 'section')
    if not section:
        raise ValueError, 'No section to start from with: %s' % filename

    params.update(cfg.items(section))
    bo = params.get('builder_options')
    if bo:
        bo = dict(cfg.items(bo))
    if bo:
        params['builder_options'] = bo
        params.update(bo)
    else:
        params['builder_options'] = dict()

    params = btdtwf.util.format_flat_dict(params)

    print 'Params = %s' % str(params)
    return params

def get_builder(name, **kwds):
    exec ('import ' + name)
    bmod = sys.modules[name]
    return bmod.Builder(**kwds)

def generate_geometry_file(geofile=None, **params):
    from cowbells import geom, default
    default.all()               # fixme, move this into geom

    builder = get_builder(params['builder'], **params['builder_options'])
    geom.placements.PhysicalVolume('pvWorld', builder.top())
    builder.place()
    builder.sensitive()

    with open(geofile,'w') as fp:
        fp.write(geom.dumps_json())

    return geofile


def geometry_generator(connections, **defaults):
    '''A btdtwf.son.node callable producing a geometry description file

    Args:
        connections (OrderedDict): node input edge connection objects and keywords

        defaults (dict): creation-time keywords

    Returns:
        string naming the generated file

    A single connection is expected and should provide the name of a configuration file.

    Any ``defaults`` keyword arguments will be overridden by any that
    are attached to the edge connection and finally by any in the
    configuration file.

    See ``parse_config`` for explanation of configuration file.  Any
    keys described there may be given as ``defaults`` or as edge
    keywords.  In addition a ``section`` keyword may be given to
    select the starting section from the configuration file.

    '''
    params = defaults
    if len(connections):
        node, kwds = connections.items()[0]
        params.update(kwds)
        cfg_filename = node()
        params = parse_config(cfg_filename, **params)
    return generate_geometry_file(**params)



