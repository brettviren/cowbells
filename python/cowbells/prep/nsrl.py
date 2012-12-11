#!/usr/bin/env python
'''
Make NSRL test detector geometry
'''

    def make_cyl(self, geo, name):
        'Make a cylinder by name'
        med = self.get_med(geo, name)
        rad = self.dimensions['%s_rad'%name.lower()]
        half_length = 0.5*self.dimensions['cyl_length']
        print 'Cylinder "%s" r=%f h/2=%f' %(name,rad,half_length)
        cyl = geo.MakeTube(name, med, 0.0, rad, half_length)
        cyl.SetVisibility(1)
        return cyl

def make_sample_tub(inner_diameter, inner_height, thickness):
    ........
    return 

