#!/usr/bin/env python
'''
Check some things with the NSRL setup
'''

from params import get as params

from cowbells.ana.run import BaseRun
BaseRun.file_pattern = '%(study)s-%(label)s-%(sample)s-%(particle)s-%(energy)sMeV-%(nevents)sevts'

from cowbells.ana.run import ConfigNsrlRun as ConfigRun
from cowbells.ana.run import SimRun
from plot import PlotRun
