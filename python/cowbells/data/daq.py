#!/usr/bin/env python
'''
Produce a DAQ-like tree base on the hits produced by the simulation.
'''

daq_Header = dict(
    RunNumber=          ('i',1,'Run number'),
    RunStartTime=       ('i',1,'Absolute start time in Unix epoch'),
    IsThisDataFile=     ('i',1,'Zero if MC'),
    is12or14bit=        ('i',1,'Dynamic range of FADC'),
    frequency=          ('i',1,'FADC sample rate'),
    runtype=            ('i',1,'Run type'),
    sampletype=         ('1',1,'Sample type'),
    Channel0Gain=       ('f',1,'Channel 0 gain'),
    Channel1Gain=       ('f',1,'Channel 1 gain'),
    Channel2Gain=       ('f',1,'Channel 2 gain'),
    Channel3Gain=       ('f',1,'Channel 3 gain'),
    Channel0device=     ('i',1,'Channel 0 device'),
    Channel1device=     ('i',1,'Channel 1 device'),
    Channel2device=     ('i',1,'Channel 2 device'),
    Channel3device=     ('i',1,'Channel 3 device'),
    PedestalSubstructedAtRun=('i',1,'Pedestal already subtracted'),
)
daq_Footer = dict(
    TotalEventsNumber=  ('i',1,'Number of triggers'),
    RunStopTime=        ('i',1,'Absolute stop time in Unix epoch'),
)
daq_FADCData = dict(
    EventNumber=        ('i',1,'Trigger count'),
    TriggerTimeFromRunStart=('d',1,'Time of trigger since start of run'),
    Channel0=           ('H',2560,'FADC Channel 0'),
    Channel1=           ('H',2560,'FADC Channel 1'),
    Channel2=           ('H',2560,'FADC Channel 2'),
    Channel3=           ('H',2560,'FADC Channel 3'),
)

class DaqTreeMaker(object):
    def __init__(self, cb_tree, digitizer):
        '''
        Make the tree DAQ trees in the current file using the cowbells hits tree.

        The header/footer/data trees are available from .h/.f/.d members.
        '''
        self.cb_tree = cb_tree
        self.digitizer = digitizer

        self.h = ROOT.TTree('Header', 'DAQ header from simulation')
        self.hobj = branch(self.h, **daq_Header)
        self.f = ROOT.TTree('Footer', 'DAQ footer from simulation')
        self.fobj = branch(self.f, **daq_Footer)
        self.d = ROOT.TTree('FADCData', 'DAQ FADC data from simulation')
        self.dobj = branch(self.d, **daq_FADCData)
        
        return

    def __call__(self):

        for entry in self.cb_tree:
            hc = entry.event.hc
            channel, signal = self.digitizer(hc)
            # ....... fixme: what about absolute time?
            continue
        
