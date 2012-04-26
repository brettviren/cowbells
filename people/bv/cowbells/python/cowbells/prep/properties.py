#!/usr/bin/env python
'''
Generate the properties file
'''

import sys
import propfile
import water, wbls, acrylic

pfname = sys.argv[1]
pf = propfile.PropertyFile(pfname)

print 'Dumping materials:'
for mod in [water, wbls, acrylic]:
    print '\t',mod.__name__
    mod.properties(pf)
    continue

pf.close()

