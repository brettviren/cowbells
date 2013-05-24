#!/usr/bin/env python
'''
Stuff shared between various org formatting functions
'''

class CanvasPrinter(object):
    '''
    Canvas printer protocol requirements:
     - callable taking a <variant> string returning a list of any files produced
     - must provide a .canvas data member holding the active TCanvas
    '''
    def __call__(self, variant):
        'Print current canvas for all extensions.'
        self.canvas = None
        return None
    def outname(self, variant, ext = None):
        'Return the output file name that would be printed'
        return None

class OrgCanvasPrinter(object):
    'Encapsulating printing a canvas to files.'
    def __init__(self, canvas, prefix, exts = None):
        '''
        A canvas printer making file suitable for use by org-mode.

        The <prefix> gives a file path to append to the <variant>
        given and call time.
        '''
        self.canvas = canvas
        self.set_prefix(prefix)
        self.exts = exts
        if not self.exts: 
            self.exts = ['png', 'svg', 'pdf']

    def __call__(self, variant):
        '''Print the current canvas with <variant> set in the output
        file name.'''
        printed = []
        for ext in self.exts:
            name = self.outname(variant, ext)
            self.canvas.Print(name,ext)
            printed.append(name)
        return printed

    def set_prefix(self, prefix):
        self.prefix = prefix.replace('.','_')

    def outname(self, variant, ext = None):
        '''
        Return the output name with given variant and optional extension.
        '''
        name = self.prefix +'-'+ variant
        if ext: name = name +'.'+ ext
        return name

