#!/usr/bin/env python
'''
Main program for generating a single trigger counter
'''
import argparse

from cowbells import geom, default
from cowbells.builder import world, triggercounter
    
def gen():

    default.all()

    worldb = world.Builder()
    tcb = triggercounter.Builder( )

    worldlv = worldb.top()      # needs to come first
    tc = tcb.top()

    geom.placements.PhysicalVolume('pvWorld',worldlv)
    geom.placements.PhysicalVolume('pvTC',tc,worldlv)

    tcb.place()
    return

def write(outfile):
    print 'Writing to "%s"' % outfile
    fp = open(outfile, "w")
    fp.write(geom.dumps_json())
    fp.close()
    return

def main(args):
    gen()
    write(args[0])
    return

if '__main__' == __name__:
    import sys
    main (sys.argv[1:])
