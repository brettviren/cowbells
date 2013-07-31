#!/usr/bin/env python
'''
Test cowbells.workflow.geogen
'''
from cowbells.workflow import geogen

import tempfile
import btdtwf

workdir = tempfile.mkdtemp(prefix='test_wf_geogen_')

def setup_func():
    print 'Using workdir:',workdir

def teardown_func():
    print 'Not removing',workdir
    

def test_wf_geogen():
    got = btdtwf.got.Got(workdir)

    gotwrap = btdtwf.son.nodes.got_callable(got, 'start', 'Make a configuration file', 'configured')
    cfg_node =  btdtwf.son.nodes.CallableNode(gotwrap(geogen.make_simple_config))

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
    

