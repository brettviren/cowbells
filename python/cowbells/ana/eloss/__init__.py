#!/usr/bin/env python
'''
Check the energy loss.
'''

# satisfy ana.run's interface to a study

from params import get as params
from cowbells.ana.run import ConfigSingleTubRun as ConfigRun
from cowbells.ana.run import SimRun
from plot import PlotRun

