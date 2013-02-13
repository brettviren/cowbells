#!/usr/bin/env python
'''
Generate geometry for a trigger counter
'''
import cowbells
import base
from cowbells.geom import surfaces, sensitive
from cowbells.geom.shapes import Box
from cowbells.geom.volumes import LogicalVolume
from cowbells.geom.placements import PhysicalVolume
from cowbells.units import inch, meter, mm, cm, hbarc

class Builder(base.Builder):
    default_params = {

        # The width transverse to the beam
        'width': 2.0*cm,
        # The depth of the scint in the direction of the beam (Z)
        'depth': 0.5*cm,
        # Thickness of "photocathode" wrapping
        'thickness': 1*mm,

        # Provide an extra name in order to make unique if needed and
        # not otherwise achieved.
        'base_name' : "TC",
        }

    # map part to material
    default_parts = {
        'Scintillator': 'Scintillator',
        'PhotoCathode': 'Glass',
        }

    def basename(self):
        return self.params['base_name']

    def make_logical_volumes(self):

        p = self.pp()[0]

        hwidth = 0.5*p.width
        hdepth = 0.5*p.depth
        thick = p.thickness

        part = 'PhotoCathode'
        shape = Box(self.shapename(part),
                    x=hwidth+thick, y=hwidth+thick, z=hdepth+thick)
        pc_lv = LogicalVolume(self.lvname(part),
                              matname = self.parts[part], shape = shape)

        part = 'Scintillator'
        shape = Box(self.shapename(part),
                    x=hwidth, y=hwidth, z=hdepth)
        sc_lv = LogicalVolume(self.lvname(part),
                              matname = self.parts[part], shape = shape)

        return pc_lv

    def place(self):
        PhysicalVolume(self.pvname('Scintillator'),
                       self.lvname('Scintillator'),self.lvname('PhotoCathode'))

        return
    
    def sensitive(self):
        what = 'PhotoCathode'
        sdname = self.sensname(what)
        hcname = self.hitcolname(what)
        lvname = self.lvname(what)
        sd = sensitive.SensitiveDetector(sdname,hcname,lvname)
        print sdname, 'touchables:', sd.touchables()

#        sd = sensitive.SensitiveDetector('SensitiveDetector', 'TC_HC', 
#                                         self.lvname('TCPhotoCathode'))
#        print 'TriggerCounter touchables:',sd.touchables()
        
