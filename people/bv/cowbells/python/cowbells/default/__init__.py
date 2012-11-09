#!/usr/bin/env python
'''
Call functions in this method to create some default cowbells.geom objects
'''

from cowbells import geom

def elements():
    '''
    Create some common elements
    '''
    e = geom.elements.Element
    e('hydrogen','h',  1,   1.01)
    e('boron',   'b',  5,  10.811)
    e('carbon',  'c',  6,  12.01)
    e('nitrogen','n',  7,  14.01)
    e('oxygen',  'o',  8,  16.0)
    e('florine' ,'F',  9 , 19.00)
    e('sodium',  'Na',11 , 22.99)
    e('aluminum','Al',13 , 26.98)
    e('Silicon', 'Si',14 , 28.085)
    e('sulfur'  ,'S', 16 , 32.07)
    return

def materials():
    '''
    Create some common materials
    '''
    m = geom.materials.Material

    m('Air',0.00129, elelist=[('n',0.7),      ('o', 0.3)])
    m('Water',  1.0, elelist=[('h',2),        ('o', 1)])
    m('Teflon', 2.2, elelist=[('C',0.759814), ('F',0.240186)])
    m('Acrylic',1.18,elelist=[("C",0.59984),  ("H",0.08055), ("O", 0.31961)])
    m('Aluminum',2.7,elelist=[('Al',1.0)])
    m('Scintilator',1.032, elelist=[('C',9), ('H',10)])

    # 1% Water-based Liquid Scintilator
    m('WBLS01', 0.9945, elelist = [('H', 0.1097),
                                   ('O', 0.8234),
                                   ('S', 0.0048),
                                   ('N', 0.0001),
                                   ('C', 0.0620)])    

    m('SiO2', 2.20, elelist=[('Si',1), ('O',2)])
    m('B2O3', 2.46, elelist=[('B', 2), ('O',3)])
    m('Na2O', 2.27, elelist=[('Na',2), ('O',1)])
    m('Al2O3',4.00, elelist=[('Al',2), ('O',3)])

    m('Glass', 2.23,matlist=[('SiO2',0.806),('B2O3',0.130),
                             ('Na2O',0.040),('Al2O3',0.024)])

    return

def optical():
    '''
    Create some common optical properties
    '''
    geom.optical.MaterialProperty('Air', 'RINDEX', x=[1.329,1.425], y=[1.0,1.0])
    import water
    water.optical()
    import wbls
    wbls.optical()



def all():
    elements()
    materials()
    optical()


if '__main__' == __name__:
    all()
    import json
    print json.dumps( { 'elements':geom.elements.pod(), 'materials':geom.materials.pod(), }, indent=2 )

        
