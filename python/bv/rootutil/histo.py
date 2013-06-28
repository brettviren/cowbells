#!/usr/bin/env python

import ROOT
from UserDict import DictMixin
from bv.collections import ChainedDict

class Reader(DictMixin):
    def __init__(self, tdir):
        self._tdir = tdir
        self._data = dict()

    def __getitem__(self, name):
        try:
            return self._data[name]
        except KeyError:
            pass
        obj = self._tdir.Get(name)
        if not obj:
            raise KeyError, 'No key "%s" found in %s' % (name, self._tdir.GetName())
        print 'Reader: pull "%s" from %s' % (name, self._tdir.GetName())
        self._data[name] = obj
        return obj

    def __setitem__(self, name, value):
        self._data[name] = value

    def keys(self):
        my_keys = set(self._data.keys())
        file_keys = set([k.GetName() for k in self._tdir.GetListOfKeys()])
        my_keys.update(file_keys)
        return my_keys


class Writer(DictMixin):
    def __init__(self, tdir, lazy = True):
        self._tdir = tdir
        self._lazy = lazy
        self._data = dict()

    def __getitem__(self, name):
        return self._data[name]

    def __setitem__(self, name, value):
        print 'Writer: "%s" --> %s' % (name, self._tdir.GetName())
        if self._lazy:
            value.SetDirectory(self._tdir)
        else:
            self._tdir.WriteTObject(value, name)
        self._data[name] = value

    def keys(self):
        return self._data.keys()

class FileStore(object):
    '''
    Provide a dictionary-like filestore.

    >>> with FileStore('file.root') as fs:
    >>>   fs['somehist'] = ...
    >>>   h = fs['anotherhist']

    '''

    def __init__(self, filename, mode='a', lazy = True):
        mode2opt = dict(a='update', r='', rw='recreate')
        opt = mode2opt[mode]
        self.fp = ROOT.TFile.Open(filename, opt)
        sink = None
        if opt in ['recreate','update']:
            self.lazy = lazy
            sink = Writer(self.fp, lazy=lazy)
        else:
            self.lazy = False
        self.cd = ChainedDict(source=Reader(self.fp), sink=sink)
    def __enter__(self):
        return self.cd
    def __exit__(self, ct, cv, tb):
        if self.lazy:
            self.fp.Write()
        self.fp.Close()
