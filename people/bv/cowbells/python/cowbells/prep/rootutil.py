#!/usr/bin/env python
'''
Various utilities for handling ROOT objects
'''

def walk(top):
    '''
    An os.walk-like TDirectory hierarchy generator

    Use like:

        for dirpath, subdirs, objs in walk(top):
            ...

    Unlike os.walk and instead of being strings, "top" is a TDirectory
    object from which to start the descent, "dirpath" is a list of
    TDirectory objects starting with top and including all other
    TDirectory objects to get to the current one, including the
    current one. "subdirs" is a list of TDirectory objects directly
    held by tdir and "objs" are the non-TDirectory objects held by
    tdir.

    Also, unlike os.walk(), no optional arguments ae available.
    '''

    dirs, nondirs = [], []
    keylist = top.GetListOfKeys()
    for key in keylist:
        obj = key.ReadObj()
        if obj.IsA().InheritsFrom("TDirectory"):
            dirs.append(obj)
        else:
            nondirs.append(obj)
            pass
        continue

    # topdown
    yield [top], dirs, nondirs
    for tdir in dirs:
        for a,b,c in walk(tdir):
            yield [top]+a,b,c
            continue
        continue
    return
