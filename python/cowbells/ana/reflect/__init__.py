#!/usr/bin/env python
'''
Use cosmics to look at effect of different reflectivity.
'''
# satisfy ana.run's interface to a study

from params import get as params
from cowbells.ana.run import BaseRun
BaseRun.file_pattern = '%(study)s-%(reflectivity)s-%(tub)s-%(sample)s-%(particle)s-%(energy)sMeV-%(nevents)sevts'

from cowbells.ana.run import ConfigSingleTubRun as ConfigRun
from cowbells.ana.run import SimRun
from plot import PlotRun
