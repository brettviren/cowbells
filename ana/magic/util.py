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

def format_dict(dat, formatter = str.format, **kwds):
    '''
    Format all string values of the dict <dat> using its own values
    but overridden by any <kwds> and with the given string
    <formatter>.  Any <kwds> are applied but not copied into the
    output.  Non-string values are passed through untouched.
    '''

    kwds = dict(kwds)
    unformatted = dict(dat)
    formatted = dict()

    while unformatted:
        changed = False
        for k,v in unformatted.items():
            if isinstance(v, basestring):
                try:
                    new_v = formatter(v, **kwds)
                except KeyError:
                    continue        # maybe next time
            changed = True
            formatted[k] = new_v
            kwds[k] = new_v
            unformatted.pop(k)
            continue
        if not changed:
            break
        continue
    if unformatted:
        formatted.update(unformatted)
    return formatted


class ParamPrinter(object):
    def __init__(self, canvas, exts = ('png','svg','pdf'), **kwds):
        self.canvas = canvas
        self.exts = exts
        self.vars = kwds
        return
        
    def outname(self, **kwds):
        flat = format_dict(self.vars, **kwds)
        return flat['print_name']

    def __call__(self, **kwds):
        out = self.outname('print_name', **kwds)
        for ext in self.exts:
            self.canvas.Print(out + '.' + ext,ext)
        return out
            
def format_list_latex(files):
    lines = []
    for file in files:
        lines.append('\\pagebreak\n')
        lines.append('\\includegraphics[width=\\textwidth]{%s}\n'%file)
    return '\n'.join(lines)
