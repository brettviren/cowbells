#!/usr/bin/env python
'''
Test cowbells.workflow.geogen
'''
import os

from cowbells.workflow import geogen

import tempfile
import btdtwf
import ConfigParser

workdir = tempfile.mkdtemp(prefix='test_wf_geogen_')

def setup_func():
    print 'Using workdir:',workdir

def teardown_func():
    print 'Not removing',workdir
    

def make_config(callables, **kwds):
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

def test_wf_geogen():
    got = btdtwf.got.Got(workdir)

    gotwrap = btdtwf.son.nodes.got_callable(got, 'start', 'Make a configuration file', 'configured')
    cfg_node =  btdtwf.son.nodes.CallableNode(gotwrap(make_config))

    gotwrap = btdtwf.son.nodes.got_callable(got, 'configured', 'Make a geofile file', 'described')
    gen_node = btdtwf.son.nodes.CallableNode(gotwrap(geogen.geometry_generator),
                                             section='geometry test')

    graph = btdtwf.Graph()
    graph.add_edge(cfg_node, gen_node)
    btdtwf.connect(graph)

    ret = gen_node()
    print 'test_got_node returns:',ret

if __name__ == '__main__':
    setup_func()

    test_wf_geogen()

    teardown_func()
    

