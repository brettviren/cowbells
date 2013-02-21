#!/usr/bin/env python
'''
Check some things with the NSRL setup
'''

from params import get as params

from cowbells.ana.run import BaseRun
BaseRun.file_pattern = '%(study)s-%(label)s-%(layout)s-%(box)s-%(sample)s-%(particle)s-%(energy)sMeV-%(nevents)sevts'

class ConfigRun(BaseRun):
    '''
    Generate the config file for a single Box detector
    '''
    def __init__(self, params):
        super(self.__class__, self).__init__(None, "json", params)
        return

    def run(self):
        from cowbells import geom, default
        from cowbells.builder import boxdet, world

        default.all()

        b = boxdet.World( box = self.p.box.capitalize(),
                          sample = self.p.sample.capitalize(),
                          layout = self.p.layout,
                          period = float(self.p.period), ndets = int(self.p.ndets),
                          absorber_material = self.p.absorber_material,
                          absorber_thickness = float(self.p.absorber_thickness),
                          )
        
        worldlv = b.top()
        geom.placements.PhysicalVolume('pvWorld',worldlv)    
        b.place()
        b.sensitive()

        print 'Writing %s' % self.p.outfile
        fp = open(self.p.outfile, 'w')
        fp.write(geom.dumps_json())
        fp.close()
        return 


from cowbells.ana.run import SimRun
#from plot import PlotRun
