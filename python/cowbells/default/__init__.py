#!/usr/bin/env python
'''
Call functions in this method to create some default cowbells.geom objects
'''

from cowbells import geom, units

gpermole = units.gram/units.mole
gpercm3 = units.gram/units.cm3

def elements():
    '''
    Create some common elements
    '''
    e = geom.elements.Element
    e('hydrogen','h',  1,   1.01*gpermole)
    e('boron',   'b',  5,  10.811*gpermole)
    e('carbon',  'c',  6,  12.01*gpermole)
    e('nitrogen','n',  7,  14.01*gpermole)
    e('oxygen',  'o',  8,  16.0*gpermole)
    e('florine' ,'F',  9 , 19.00*gpermole)
    e('sodium',  'Na',11 , 22.99*gpermole)
    e('aluminum','Al',13 , 26.98*gpermole)
    e('Silicon', 'Si',14 , 28.085*gpermole)
    e('sulfur'  ,'S', 16 , 32.07*gpermole)
    e('potassium','K',19 , 39.10*gpermole)
    e('antimony','Sb',51 ,121.76*gpermole)
    e('cesium',  'Cs',55 ,132.91*gpermole)
    e('lead',    'Pb',82 ,207.2*gpermole)
    return

def materials():
    '''
    Create some common materials
    '''
    m = geom.materials.Material

    m('Air',0.00129*gpercm3, elelist=[('n',0.7), ('o', 0.3)])
    m('Water', 1.0*gpercm3, elelist=[('h',2),        ('o', 1)])
    m('Teflon', 2.2*gpercm3, elelist=[('C',0.759814), ('F',0.240186)])
    m('Acrylic', 1.18*gpercm3, elelist=[("C",0.59984),  ("H",0.08055), ("O", 0.31961)])
    m('Aluminum', 2.7*gpercm3, elelist=[('Al',1.0)])
    m('Scintillator', 1.032*gpercm3, elelist=[('C',9), ('H',10)])
    m('Bialkali', 0.1*gpercm3, elelist=[('Na',0.375), ('K',0.1875), ('Cs',0.1875), ('Sb',0.25)])
    m('TCBialkali', 0.1*gpercm3, elelist=[('Na',0.375), ('K',0.1875), ('Cs',0.1875), ('Sb',0.25)])

    # 1% Water-based Liquid Scintillator
    m('WBLS01', 0.9945*gpercm3, elelist = [('H', 0.1097),
                                           ('O', 0.8234),
                                           ('S', 0.0048),
                                           ('N', 0.0001),
                                           ('C', 0.0620)])    

    m('SiO2', 2.20*gpercm3, elelist=[('Si',1), ('O',2)])
    m('B2O3', 2.46*gpercm3, elelist=[('B', 2), ('O',3)])
    m('Na2O', 2.27*gpercm3, elelist=[('Na',2), ('O',1)])
    m('Al2O3',4.00*gpercm3, elelist=[('Al',2), ('O',3)])

    m('Glass', 2.23*gpercm3,matlist=[('SiO2',0.806),('B2O3',0.130),
                                     ('Na2O',0.040),('Al2O3',0.024)])

    m('Lead', 11.35*gpercm3, elelist=[('Pb',1.0)])
    return

def optical():
    '''
    Create some common optical properties
    '''
    # geom.optical.MaterialProperty('Air', 'RINDEX', x=[1.329,1.425], y=[1.0,1.0])

    for matname in ['water','wbls','acrylic','bialkali','scintillator']:
        exec ('import %s' % matname)
        mod = eval (matname)
        mod.optical()



def all():
    elements()
    materials()
    optical()


if '__main__' == __name__:
    all()
    import json
    print json.dumps( { 'elements':geom.elements.pod(), 'materials':geom.materials.pod(), }, indent=2 )

        
