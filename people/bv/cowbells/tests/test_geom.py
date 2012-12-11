#!/usr/bin/env python
'''
Test the cowbells.geom package
'''

from cowbells.geom import *

def test_elements():
    h = elements.Element('hydrogen','h', 1,  1.01)
    c = elements.Element('carbon',  'c', 6,  12.01)
    n = elements.Element('nitrogen','n', 7, 14.01)
    o = elements.Element('oxygen',  'o', 8, 16.0)
    assert len(elements.store), 'Failed to make any elements'

def test_materials():
    air = materials.Material('Air',0.00129, [('n',0.7), ('o', 0.3)])
    water = materials.Material('Water',1.0, [('h',2), ('o', 1)])
    assert len(materials.store), 'Failed to make any materials'
    return

def test_optical():
    optical.MaterialProperty('Air',   'RINDEX', x=[1.329,1.425], y=[     1.0,1.0    ])
    optical.MaterialProperty('Water', 'RINDEX', x=[1.329,1.425], y=[1.589e-6,6.20e-6])
    assert len(optical.store), 'Failed to make any optical material properties'
    return

def test_surfaces():
    surf = surfaces.OpticalSurface('TestSurface', model='glisur', 
                                   type='dielectric_metal', finish='polished')
    surf.add_parameter('first','pvFirstVolume')
    surf.add_parameter('second','pvSecondVolume')
    surf.add_property("REFLECTIVITY", x=[1,6], y=[1,1])
    assert len(surfaces.store), 'Failed to make any surfaces'
    return

def test_volumes():
    wb = volumes.Shape('world_box', 'box', x=10.0, y=10.0, z=10.0)
    volumes.LogicalVolume('lvWorld', 'Air', wb)

    wc = volumes.Shape('water_cyl', 'tubs', rmax=1.0, dz=1.0)
    volumes.LogicalVolume('lvCyl', 'Water', wc)
    assert len(volumes.store), 'Failed to make any logical volumes'
    return

def test_placements():
    placements.PhysicalVolume('pvWorld', 'lvWorld')
    placements.PhysicalVolume('pvWaterCylInWorld', 'lvCyl', 'lvWorld')
    assert len(placements.store), 'Failed to make any physical volumes'
    print placements.placed('lvWorld')
    for pvs,l in placements.walk('pvWorld'):
        print 'Walked:', pvs, l
    return

def test_sensitive():
    sd = sensitive.SensitiveDetector('SensitiveDetector', 'HC', 'lvCyl')
    print sd.touchables()
    return

json_test_file = 'test_geom.json'

def test_write():
    fp = open(json_test_file,"w")
    fp.write(dumps_json())
    fp.close()
    print 'Wrote %s' % json_test_file
    return

def test_j2g4():
    '''
    Run the json2g4.exe program on the file produced above
    '''
    from subprocess import Popen, PIPE
    proc = Popen(['json2g4.exe',json_test_file], stdout = PIPE)
    out,err = proc.communicate()
    print out,err
    return

if __name__ == '__main__':
    test_elements()
    test_materials()
    test_optical()
    test_surfaces()
    test_volumes()
    test_placements()
    test_sensitive()
    test_write()
    test_j2g4()
