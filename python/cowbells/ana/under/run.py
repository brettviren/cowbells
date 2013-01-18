#!/usr/bin/env python
from cowbells.ana.run import BaseRun

class ConfigRun(BaseRun):
    '''
    Generate the config file
    '''
    def __init__(self, params):
        super(ConfigRun,self).__init__(None, "json", params)
        return

    def run(self):
        from cowbells import geom, default
        from cowbells.builder import tubdet, world

        default.all()

        b = tubdet.World( tub = self.p.tub.capitalize(),
                          sample = self.p.sample.capitalize())
        worldlv = b.top()
        geom.placements.PhysicalVolume('pvWorld',worldlv)    
        b.place()
        b.sensitive()

        print 'Writing %s' % self.p.outfile
        fp = open(self.p.outfile, 'w')
        fp.write(geom.dumps_json())
        fp.close()
        return 

