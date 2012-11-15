#!/usr/bin/env python
'''
Trivial builder to build a beam window
'''
import cowbells
import base

from cowbells.units import mm, cm
from cowbells.geom.volumes import Tubs, LogicalVolume


class Builder(base.Builder):
    default_params = {
        'radius': 2*cm,
        'thickness': 0.381*mm,
        }

    default_parts = {
        'BeamWindow': 'Aluminum',
        }

    def make_logical_volumes(self):
        p = self.pp()[0]

        hheight = 0.5*p.thickness

        part = 'BeamWindow'
        shape = Tubs(self.shapename(part), dz = hheight, rmax = p.radius)
        lv = LogicalVolume(self.lvname(part),
                           matname = self.parts[part], shape = shape)
        
        return lv

