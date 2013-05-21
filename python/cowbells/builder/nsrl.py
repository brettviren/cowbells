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

from collections import namedtuple
import base, world, tubdet, magic, triggercounter, beamwindow
from cowbells.geom.placements import PhysicalVolume

from cowbells.units import meter, cm, degree, mm

class Builder_12c(base.Builder):
    default_params = {
        'sample1': 'Water',
        'sample2': 'Water',
        'tub1': 'Teflon',
        'tub2': 'Aluminum',
        'tub1_color' : 'white',
        'tub2_color' : 'black',

        'bw_offset': -5*meter,
        'td_offset': 0*meter,
        'td_separation': 40*cm,

        }

    # no explicit parts

    def basename(self):
        return 'nsrl12c'

    def make_logical_volumes(self):
        p = self.pp()[0]

        self.builders = [
            world.Builder(),
            beamwindow.Builder(),
            triggercounter.Builder(),
            tubdet.Builder(base_name = 'Tub1', Bottom = p.tub1, Side = p.tub1,
                           Lid = p.tub1, Sample = p.sample1, teflon_color = p.tub1_color),
            tubdet.Builder(base_name = 'Tub2', Bottom = p.tub2, Side = p.tub2,
                           Lid = p.tub2, Sample = p.sample2, teflon_color = p.tub2_color)
            ]

        self.lvs = [b.top() for b in self.builders]
        return self.lvs[0]

    def place(self):
        p = self.pp()[0]
        tc_start = p.td_offset - 0.5*p.td_separation # starting point for first TC
        tubrot = { 'rotatex': 90*degree }

        to_place = [
            (self.lvs[1],p.bw_offset,0,None), # beam window
            (self.lvs[2],tc_start,                  1,None), # TC1
            (self.lvs[2],tc_start+p.td_separation,  2,None), # TC2
            (self.lvs[2],tc_start+p.td_separation*2,3,None), # TC3
            (self.lvs[3],p.td_offset,0,tubrot),              # Tub1
            (self.lvs[4],p.td_offset+p.td_separation,0,tubrot), # Tub2
            ]

        world_lv = self.lvs[0]
        for lv,z,cp,rot in to_place:
            name = lv.name.replace('lv','pv',1)
            print 'Placing %s #%d at z=%f cm' % (name,cp,z/cm) 
            PhysicalVolume(name, lv, world_lv, rot=rot, pos=[0.0,0.0,z], copy=cp)
            continue

        for b in self.builders:
            b.place()

        return

    def sensitive(self):
        for b in self.builders:
            b.sensitive()

class Builder_13a(base.Builder):

    default_params = {
        'sample': 'Water',

        # as read off Assembly1-2.idw using inner_z as scale
        'trigger_counter_downstream_offset': 523*mm,
        'trigger_counter_upstream_offset': 504*mm,

        # Position of magic box detector center w.r.t. nominal beam
        'magic_beam_offset': -28.89*mm,

        # Position of beam window w.r.t. the magic box center
        'beamwindow_offset': -5*meter,

        # Reflectivity of the sample/box wall surface
        'reflectivity': 0.02,
        }

    def basename(self):
        return 'nsrl13a'

    def make_logical_volumes(self):
        'Create the builders and their top logical volumes'

        p = self.pp()[0]

        Quad = namedtuple('Quad','world beamwindow triggercounter magic')
        self. builders = Quad(world.Builder(),
                              beamwindow.Builder(),
                              triggercounter.Builder(),
                              magic.Builder(reflectivity=p.reflectivity))
        self.lvs = Quad(*[b.top() for b in self.builders])
        return self.lvs[0]

    def place(self):
        'Place top logical volumes'
        p = self.pp()[0]

        
        self.lvs.world

        # Beam window
        PhysicalVolume(self.lvs.beamwindow.name.replace('lv','pv',1),
                       self.lvs.beamwindow, self.lvs.world,
                       pos = [0.0, 0.0, p.beamwindow_offset])

        # downstream trigger counter
        PhysicalVolume(self.lvs.triggercounter.name.replace('lv','pv',1),
                       self.lvs.triggercounter, self.lvs.world,
                       pos = [0.0, 0.0, p.trigger_counter_downstream_offset],
                       copy = 1)

        # upstream trigger counter
        PhysicalVolume(self.lvs.triggercounter.name.replace('lv','pv',1),
                       self.lvs.triggercounter, self.lvs.world,
                       pos = [0.0, 0.0, -p.trigger_counter_upstream_offset],
                       copy = 2)
                       
        # magic box detector
        PhysicalVolume(self.lvs.magic.name.replace('lv','pv',1),
                       self.lvs.magic, self.lvs.world,
                       pos = [0.0, p.magic_beam_offset, 0.0])

        for b in self.builders:
            b.place()

        return

    def sensitive(self):
        for b in self.builders:
            b.sensitive()
    pass


def Builder(experiment, **kwds):
    if experiment.lower() in ["12c"]:
        return Builder_12c(**kwds)
    return Builder_13a(**kwds)
