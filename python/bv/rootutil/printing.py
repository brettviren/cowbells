#!/usr/bin/env python

import os

class PrintManager(object):
    '''
    A dictionary-like object providing management of "printing" to file.
    '''
    def __init__(self, printer, drawer, filenamer, overwrite = False, **kwds):
        '''
        Arguments:
        printer -- a callable taking (filename) to print current graphics
        drawer -- a callable taking (name, **kwds) to draw something by its <name> 
        filenamer -- a callable taking (name, **kwds) returning a filename to print.
        kwds -- passed along to <drawer> and <filenamer>
        '''
        self._printer = printer
        self._drawer = drawer
        self._filenamer = filenamer
        self._overwrite = overwrite
        self._params = kwds
        self._printed = dict()

    def __getitem__(self, name):
        '''
        Get the filename associated with the <name>.  Raises KeyError if print fails.
        '''
        return self.__call__(name)

    def __call__(self, name):
        filename = self._printed.get(name)
        if filename:
            return filename
        filename = self._filenamer(name, **self._params)
        if os.path.exists(filename) and not self._overwrite:
            print 'File exists and overwrite protection enabled, not reprinting: %s' % filename
            return filename
        self._drawer(name, **self._params)
        self._printer(filename)
        self._printed[name] = filename
        return filename

    def __setitem__(self, name, filename):
        '''
        Force the setting of the <filename> for the <name>.  Can be set to
        None to cause getitem to force a reprint.  
        '''
        self._printed[name] = filename

    def __delitem__(self, name):
        del(self._printer[name])

class MultiPrinter(object):
    '''
    A multi-page printer.

    >>> with MultiPrinter(filename='file.pdf', printer = ...) as printer:
    >>>   pm = PrintManager(..., printer = printer, ...)
    >>>   pm('plot1')
    >>>   pm('plot2')
    >>>   pm('plot3')

    '''

    def __init__(self, filename, printer):
        self._filename = filename
        self._format = os.path.splitext(filename)[1][1:]
        self._printer = printer
        
    def __enter__(self):
        self._printer(self._filename + '[', self._format)
        return self.printer
    def __exit__(self, et, ev, tb):
        self._printer(self._filename + ']', self._format)
        return
        
    def printer(self, filename, **kwds):
        '''
        Implement a print-manager printer.  

        Note, this ignores the filename passed here.
        '''
        self._printer(self._filename, self._format)
        
