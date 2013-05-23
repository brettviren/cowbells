#!/usr/bin/env python
'''
Main program for generating the NSRL setup
'''
from cowbells import geom, default

default.all()
from cowbells.builder import nsrl



def gen(experiment, **kwds):
    b = nsrl.Builder(experiment, **kwds)
    worldlv = b.top()
    #print 'Placing world volume: %s' % worldlv.name
    geom.placements.PhysicalVolume('pvWorld',worldlv)    
    b.place()
    b.sensitive()

def write(outfile):
    fp = open(outfile, "w")
    fp.write(geom.dumps_json())
    fp.close()

def main(runname, outfile, **kwds):
    gen(runname,**kwds)
    write(outfile)

def argv2kwds(*args):
    kwds = dict()
    for arg in args:
        try:
            k,v = arg.split('=')
        except ValueError:
            print 'Not in key=val form: "%s"' % arg
            raise
        print 'Passing %s=%s' % (k,v)
        kwds[k] = v

    return kwds

if '__main__' == __name__:
    import sys

    try:
        runname = sys.argv[1]
        outfile = sys.argv[2]
        kwdargs = sys.argv[3:]
    except IndexError:
        print 'Failed to parse arguments'
        print 'gennsrl.py <run name> <outputfile.json> [key=val ...]'
        raise

    kwds = argv2kwds(*kwdargs)
    main(runname, outfile, **kwds)
