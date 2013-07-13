#!/usr/bin/env python
'''
A btdtwf process for running cowbells
'''

from cowbells import units
from btdtwf.parameter import ParameterSet, FileName, String, Number
from btdtwf import bein


input_parameters = ParameterSet(
    x             = Number(0, desc='Vertex X position'),
    y             = Number(0, desc='Vertex Y position'),
    z             = Number(0, desc='Vertex Z position'),
    dx            = Number(0, desc='Vertex dX position'),
    dy            = Number(0, desc='Vertex dY position'),
    dz            = Number(1, desc='Vertex dZ position'),
    kin           = String('beam', desc='Kinematics type'),
    energy        = Number(1*units.GeV, desc='Initial Particle Energy'),
    particle      = String('proton', desc='Particle name'),
    nevents       = Number(1, desc='Number of events'),
    physics       = String('op,em', desc='Physics list modules'),
    outmod        = String('hits', desc='Output data modules'),
    cb_extra_args = String('', desc='Any extra command line arguments to cowbells'),
    geofile       = FileName('', desc='Geometry configuration file', hint='input'),
    cowbellsexe   = String('cowbells.exe', desc='Name of cowbells executable'),
    simfile       = FileName('cowbells_{particle}_{energy}_{nevents}_{geofile}.root', 
                             desc = 'ROOT file holding cowbells tree.', hint='output'),
)

output_parameters = ParameterSet(
    )

@bein.program
def cbexe(**kwds):
    cmdstr = '{cowbellsexe} -n {nevents} -p {physics} -m {outmod} -k '
    cmdstr += '-o {simfile} {cb_extra_args} {geofile}'
    cmdstr += " \'kin://{kin}?vertex={x},{y},{z}&particle={particle}&direction={dx},{dy},{dz}&energy={energy}\' "
    cmdstr = cmdstr.format(**kwds)
    cmd = cmdstr.split()
    print cmd
    return {'arguments': cmd, 'return_value': kwds['simfile']}


def callable(ex, **kwds):
    cbexe(ex, **kwds)
    return dict()
