#!/usr/bin/env python
'''
Main program for generating the NSRL setup
'''
from cowbells import geom, default

default.all()
from cowbells.builder import nsrl



def gen(experiment):
    b = nsrl.Builder(experiment)
    worldlv = b.top()
    #print 'Placing world volume: %s' % worldlv.name
    geom.placements.PhysicalVolume('pvWorld',worldlv)    
    b.place()
    b.sensitive()

def write(outfile):
    fp = open(outfile, "w")
    fp.write(geom.dumps_json())
    fp.close()

def main(args):
    gen(args[0])
    write(args[1])

if '__main__' == __name__:
    import sys
    if len(sys.argv[1:]) != 2:
        print 'gennsrl.py <run name> <outputfile.json>'
        sys.exit(1)

    main(sys.argv[1:])
