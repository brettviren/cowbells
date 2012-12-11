#!/usr/bin/env python
'''
Generate sample geometry for COsmic WB(el)LS test detector

It is composed of concentric, equal length cylinders of:

 - black cover
 - clear, acrylic tube
 - WBLS

At either ends of the cylinders are end-caps made from an acrylic
plate and a 2" PMT.  These modeled as a layer (thin, square box shape)
of acrylic and a glass puck with a layer of photocathode material.
'''

import cowbells
from util import make_translation, make_rotation

units = cowbells.units
inch = 2.45 * units.cm




class CowbellGeometryBuilder(object):
    
    default_dimensions = {
        'wbls_rad':1.0*inch,
        'acrylic_rad':1.25*inch,
        'cover_rad':1.25*inch+2*units.mm,
        'cyl_length':70*units.cm,
        'cap_thickness': 0.25*inch,
        'pmt_thickness': 0.25*inch,
        'pc_thickness': 1.0*units.mm,
        'puc_radius': 1.0*inch,
        }

    default_material_map = {
        'Cover': 'BlackAcrylic',
        'Acrylic': 'Acrylic',
        'WBLS': 'WBLS',
        'Glass': 'Glass',
        'PhotoCathode':'Glass',
        }

    def __init__(self, dim=None, matmap=None):
        '''
        Create the builder.  Passsing dictionaries to either dim or
        matmat will update from default_dimensions or
        default_materials respectively.
        '''
        self.dimensions = dict(CowbellGeometryBuilder.default_dimensions)
        if dim:
            self.dimensions.update(dim)
        self.matmap = dict(CowbellGeometryBuilder.default_material_map)
        if matmap:
            self.matmap.update(matmap)
        return

    def get_med(self,geo, mat_name):
        med_name = self.matmap[mat_name]
        med = geo.GetMedium(med_name)
        if not med:
            raise ValueError, 'Bogus medium name "%s"' % med_name
        return med

    def make_cyl(self, geo, name):
        'Make a cylinder by name'
        med = self.get_med(geo, name)
        rad = self.dimensions['%s_rad'%name.lower()]
        half_length = 0.5*self.dimensions['cyl_length']
        print 'Cylinder "%s" r=%f h/2=%f' %(name,rad,half_length)
        cyl = geo.MakeTube(name, med, 0.0, rad, half_length)
        cyl.SetVisibility(1)
        return cyl

    def make_puc(self, geo, name, matname):
        med = self.get_med(geo, matname)
        rad = self.dimensions['puc_radius']
        half_length = 0.5*self.dimensions['%s_thickness'%name.lower()]
        puc = geo.MakeTube(name.upper(), med, 0.0, rad, half_length)
        puc.SetVisibility(1)
        return puc

    def top(self,geo):
        '''
        Return a top volume with the cowbells geometry constructed.  
        Materials are assumed to exist and named according to the matmap.
        '''
        
        # make concentric cylinders
        colors=[1,2,4]
        cyls = [self.make_cyl(geo,name) for name in ['Cover','Acrylic','WBLS']]
        for ind,cyl in enumerate(cyls):
            cyl.SetLineColor(colors[ind])
            if not ind: continue
            cyls[ind-1].AddNode(cyl,1)
            continue

        # make end cap

        offset = 0.5*self.dimensions['cyl_length'] # start at end of
                                                   # concentric
                                                   # cylinders
        puc_ass = geo.MakeVolumeAssembly('endcap') # hold all the end
                                                   # cap parts

        nmn = [('cap','Acrylic'), ('pmt','Glass'), ('pc','PhotoCathode')]
        for ind,(name,matname) in enumerate(nmn):
            puc = self.make_puc(geo,name,matname) 
            puc.SetLineColor(colors[ind])
            width = self.dimensions['%s_thickness'%name]
            print '%d: Making %s offset = %f+%f' %(ind+1,name,offset,0.5*width)
            trans = make_translation(0,0,offset+0.5*width)
            offset += width
            puc_ass.AddNode(puc,1,trans)
            print 'Node: "%s"' % node_name(puc)
            continue        

        top = geo.MakeVolumeAssembly("cowbells")
        top.AddNode(cyls[0],1)
        top.AddNode(puc_ass,1)
        top.AddNode(puc_ass,2,make_rotation(0,180,0))
        return top

    pass

