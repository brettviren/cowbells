#!/usr/bin/env python
'''
Main program for generating the NSRL setup
'''
import argparse

from cowbells import geom, default

default.all()
from cowbells.builder import nsrl



def gen():
    b = nsrl.Builder()
    worldlv = b.top()
    #print 'Placing world volume: %s' % worldlv.name
    geom.placements.PhysicalVolume('pvWorld',worldlv)    
    b.place()

def write(outfile):
    fp = open(outfile, "w")
    fp.write(geom.dumps_json())
    fp.close()

def main(args):
    gen()
    write(args[0])

if '__main__' == __name__:
    import sys
    main(sys.argv[1:])
