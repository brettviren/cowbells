#!/usr/bin/env python
'''
Generate the properties file
'''
import propfile

from cowbells.prep import propmods

def fill(filename):
    pf = propfile.PropertyFile(filename)
    print 'Dumping properties:'
    for mod in propmods:
        print '\t',mod.__name__
        mod.properties(pf)
        continue
    pf.close()

if __name__ == '__main__':
    import sys
    print 'Dumping materials:'
    fill(sys.argv[1])

