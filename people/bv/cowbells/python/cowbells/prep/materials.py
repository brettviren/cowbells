#!/usr/bin/env python
'''
Generate and dump out materials
'''

import cowbells
import water, wbls, acrylic

geo = cowbells.geo()

for mod in water, wbls, acrylic:
    mod.materials(geo)

if __name__ == '__main__':
    import sys
    out = sys.argv[1]
    geo.Export(out,"g")

    
