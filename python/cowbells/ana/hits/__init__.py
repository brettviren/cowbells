#!/usr/bin/env python
'''
Look at the hits
'''

# satisfy ana.run's interface to a study

from params import get as params

from cowbells.ana.run import BaseRun

BaseRun.file_pattern = '%(study)s-%(sample)s-%(tub)s-%(particle)s-%(energy)sMeV-%(nevents)sevts'


from cowbells.ana.run import ConfigSingleTubRun as ConfigRun
from cowbells.ana.run import SimRun
from plot import PlotRun
