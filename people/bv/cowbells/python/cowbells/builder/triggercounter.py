#!/usr/bin/env python
'''
Generate geometry for a trigger counter
'''
import cowbells
import base
from geom import materials, surfaces, sensitive
from geom.volumes import Tubs, Polycone, LogicalVolume
from geom.placements import PhysicalVolume

hbarc = cowbells.units.clhep_units.hbarc
inch = cowbells.units.inch
meter = cowbells.units.meter
mm = cowbells.units.mm
cm = cowbells.units.cm

class Builder(base.Builder):
    default_parameters = {

        # The width transverse to the beam
        'width': 2.0*cm,
        # The depth of the scint in the direction of the beam (Z)
        'depth': 0.5*cm,
        # Thickness of "photocathode" wrapping
        'thickness': 1*mm,

        }

    # map part to material
    default_parts = {
        'Scintillator': 'Scintillator',
        'PhotoCathode': 'Glass',
        }

    def make_logical_volumes(self):

        parms,parts = self.pp()

        hwidth = 0.5*parms.width
        hdepth = 0.5*parms.depth
        thick = p.thickness

        shape = Box(self.shapename('PhotoCathode'),
                    x=hwidth+thick, y=hwidth+thick, z=hdepth+thick)
        pc_lv = LogicalVolume(self.lvname('PhotoCathode'),
                              matname = parts.PhotoCathode, shape = shape)

        
        shape = Box(self.shapename('Scintillator'),
                    x=hwidth, y=hwidth, z=hdepth)
        sc_lv = LogicalVolume(self.lvname('Scintillator'),
                              matname = parts.Scintillator, shape = shape)

        return pc_lv

    def place(self):
        PhysicalVolume(self.pvname('Scintillator'),
                       self.lvname('Scintillator'),self.lvname('PhotoCathode'))

        return
