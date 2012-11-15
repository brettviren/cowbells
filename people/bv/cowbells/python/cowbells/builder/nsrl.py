#!/usr/bin/env python
'''
Builder for NSRL setup.  

  |     <>   ()  <>  ()  <>
  W     T1   S1  T2  S2  T3

This builder uses other builders or the details.  

 - beamwindow provides a single, upstream window
 - triggercounter provides multiple identical trigger counters
 - tubdets provide multiple differing tub detectors

All are assumed to be built with the Z-axis up and a rotation is made
to make the Z-axis point along the beam.  The first tubdet is placed
at the offset and second is separated by the separation.  The trigger
counters are placed with spacing that puts any interior ones half-way
between the tub centers.

'''

import base, world, tubdet, triggercounter, beamwindow
from cowbells.geom.placements import PhysicalVolume

from cowbells.units import meter, cm, degree

class Builder(base.Builder):
    default_params = {
        'sample1': 'Water',
        'sample2': 'Water',
        'tub1': 'Teflon',
        'tub2': 'Aluminum',

        'bw_offset': -5*meter,
        'td_offset': 0*meter,
        'td_separation': 40*cm,
        }

    # no explicit parts


    def make_logical_volumes(self):
        p = self.pp()[0]

        self.builders = [
            world.Builder(),
            beamwindow.Builder(),
            triggercounter.Builder(),
            tubdet.Builder(TubBottom = p.tub1, TubSide = p.tub1,
                           TubLid = p.tub1, Sample = p.sample1),            
            tubdet.Builder(TubBottom = p.tub2, TubSide = p.tub2,
                           TubLid = p.tub2, Sample = p.sample2)
            ]

        self.lvs = [b.top() for b in self.builders]
        return self.lvs[0]

    def place(self):
        p = self.pp()[0]
        tc_start = p.td_offset - 0.5*p.td_separation # starting point for first TC
        rot = { 'rotateX': 90*degree }

        to_place = [
            (self.lvs[1],p.bw_offset,0), # beam window
            (self.lvs[2],tc_start,                  1),    # TC1
            (self.lvs[2],tc_start+p.td_separation,  2),    # TC2
            (self.lvs[2],tc_start+p.td_separation*2,3),    # TC3
            (self.lvs[3],p.td_offset,0),                   # Tub1
            (self.lvs[4],p.td_offset+p.td_separation,0),   # Tub2
            ]

        world_lv = self.lvs[0]
        for lv,z,cp in to_place:
            name = lv.name.replace('lv','pv',1)
            #print 'Placing %s #%d at z=%f cm' % (name,cp,z/cm) 
            PhysicalVolume(name, lv, world_lv, rot=rot, pos=[0.0,0.0,z], copy=cp)
            continue

        for b in self.builders:
            b.place()

        return


