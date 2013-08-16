#!/usr/bin/env python

import btdtwf

def callme(connections, **node_kwds):
    print 'Called with %d connections and %d node keywords' % (len(connections), len(node_kwds))
    print 'connections:'
    for c,d in connections.items():
        print '\t',c
        print '\t edge keywords:'
        for k,v in d.items():
            print '\t\t',k,v
    print 'node keywords:'
    for k,v in node_kwds.items():
        print '\t',k,v

def file_copy(**kwds):
    src = kwds['src']
    dst = kwds['dst']
    def copy_src_dst(connections, **node_kwds):
        srcs = src.split(',')
        with open(dst,'w') as out:
            for one in srcs:
                with open(one) as inp:
                    out.write(inp.read())
                    out.write('\n')
        return dst
    return btdtwf.son.nodes.CallableNode(copy_src_dst, **kwds)
    
