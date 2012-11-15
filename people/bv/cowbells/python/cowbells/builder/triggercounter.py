#!/usr/bin/env python
'''
Generate geometry for a trigger counter
'''
import cowbells
import base
from cowbells.geom import surfaces, sensitive
from cowbells.geom.volumes import Box, LogicalVolume
from cowbells.geom.placements import PhysicalVolume

hbarc = cowbells.units.clhep_units.hbarc
inch = cowbells.units.inch
meter = cowbells.units.meter
mm = cowbells.units.mm
cm = cowbells.units.cm

class Builder(base.Builder):
    default_params = {

        # The width transverse to the beam
        'width': 2.0*cm,
        # The depth of the scint in the direction of the beam (Z)
        'depth': 0.5*cm,
        # Thickness of "photocathode" wrapping
        'thickness': 1*mm,

        }

    # map part to material
    default_parts = {
        'TCScintillator': 'Scintillator',
        'TCPhotoCathode': 'Glass',
        }

    def make_logical_volumes(self):

        p = self.pp()[0]

        hwidth = 0.5*p.width
        hdepth = 0.5*p.depth
        thick = p.thickness

        part = 'TCPhotoCathode'
        shape = Box(self.shapename(part),
                    x=hwidth+thick, y=hwidth+thick, z=hdepth+thick)
        pc_lv = LogicalVolume(self.lvname(part),
                              matname = self.parts[part], shape = shape)

        part = 'TCScintillator'
        shape = Box(self.shapename(part),
                    x=hwidth, y=hwidth, z=hdepth)
        sc_lv = LogicalVolume(self.lvname(part),
                              matname = self.parts[part], shape = shape)

        return pc_lv

    def place(self):
        PhysicalVolume(self.pvname('TCScintillator'),
                       self.lvname('TCScintillator'),self.lvname('TCPhotoCathode'))

        self._sensors()
        return
    
    def _sensors(self):
        sd = sensitive.SensitiveDetector('SensitiveDetector', 'TC_HC', 
                                         self.lvname('TCPhotoCathode'))
        print sd.touchables()
        
