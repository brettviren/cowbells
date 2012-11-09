#!/usr/bin/env python
'''
Main program for generating a single tub detector.
'''
import argparse

from cowbells import geom, default, world, tubdet
    

def parse_args(argv):
    parser = argparse.ArgumentParser(
        description = 'Generate a JSON file for a stand-alone "tub" detector')
    parser.add_argument('-s','--sample',default='Water',
                        help='Specify the sample material')
    parser.add_argument('-t','--tub',default='Teflon',
                        help='Specify the tub material')
    parser.add_argument('-c','--color',default='white',
                        help='Specify the "color" of the Teflon coating (black or white)')
    parser.add_argument('file', nargs="+", default='/dev/stdout',
                        help="Specify the output file")
    return parser.parse_args()
    
def gen(args):

    default.all()

    worldb = world.Builder()
    tdb = tubdet.Builder( TubBottom = args.tub, TubSide = args.tub,
                          TubLid = args.tub, Sample = args.sample )

    worldlv = worldb.top()      # needs to come first
    td = tdb.top()
    geom.placements.PhysicalVolume('pvTub',td,worldlv)
    tdb.place()
    return

def write(outfile):
    print 'Writing to "%s"' % outfile
    fp = open(outfile, "w")
    fp.write(geom.dumps_json())
    fp.close()
    return

def main(args):
    args = parse_args(args)
    outfile = args.file[0]
    gen(args)
    write(outfile)
    return

if '__main__' == __name__:
    import sys
    main (sys.argv[1:])
