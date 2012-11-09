#!/usr/bin/env python
'''
Call functions in this method to create some default cowbells.geom objects
'''

import geom

def elements():
    '''
    Create some common elements
    '''
    geom.elements.Element('hydrogen','h',  1,   1.01)
    geom.elements.Element('boron',   'b',  5,  10.811)
    geom.elements.Element('carbon',  'c',  6,  12.01)
    geom.elements.Element('nitrogen','n',  7,  14.01)
    geom.elements.Element('oxygen',  'o',  8,  16.0)
    geom.elements.Element('florine' ,'F',  9 , 19.00)
    geom.elements.Element('sodium',  'Na',11 , 22.99)
    geom.elements.Element('aluminum','Al',13 , 26.98)
    geom.elements.Element('Silicon', 'Si',14 , 28.085)
    geom.elements.Element('sulfur'  ,'S', 16 , 32.07)
    return

def materials():
    '''
    Create some common materials
    '''
    geom.materials.Material('Vacuum', 0.0)
    geom.materials.Material('Air',0.00129, elelist=[('n',0.7),      ('o', 0.3)])
    geom.materials.Material('Water',  1.0, elelist=[('h',2),        ('o', 1)])
    geom.materials.Material('Teflon', 2.2, elelist=[('C',0.759814), ('F',0.240186)])
    geom.materials.Material('Acrylic',1.18,elelist=[("C",0.59984),  ("H",0.08055), ("O", 0.31961)])
    geom.materials.Material('Aluminum',2.7,elelist=[('Al',1.0)])
    geom.materials.Material('Scintilator',1.032, elelist=[('C',9), ('H',10)])

    # 1% Water-based Liquid Scintilator
    geom.materials.Material('WBLS01', 0.9945, elelist = [('H', 0.1097),
                                                         ('O', 0.8234),
                                                         ('S', 0.0048),
                                                         ('N', 0.0001),
                                                         ('C', 0.0620)])    

    geom.materials.Material('SiO2', 2.20, elelist=[('Si',1), ('O',2)])
    geom.materials.Material('B2O3', 2.46, elelist=[('B', 2), ('O',3)])
    geom.materials.Material('Na2O', 2.27, elelist=[('Na',2), ('O',1)])
    geom.materials.Material('Al2O3',4.00, elelist=[('Al',2), ('O',3)])

    geom.materials.Material('Glass', 2.23,matlist=[('SiO2',0.806),('B2O3',0.130),
                                                   ('Na2O',0.040),('Al2O3',0.024)])

    return

def all():
    elements()
    materials()


if '__main__' == __name__:
    all()
    import json
    print json.dumps( { 'elements':geom.elements.pod(), 'materials':geom.materials.pod(), }, indent=2 )

        
