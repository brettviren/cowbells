#!/usr/bin/env python
'''

'''


from UserDict import DictMixin

class ChainedDict(DictMixin):
    '''A dictionary that can delegate a get item to a sequence of
    other source dictionaries and which broadcasts a set item to a
    sequence of sink dictionaries.  The keys are the union of all keys
    set and those of all sources.  Sinks will be updated when sources
    return values and should be prepared for receiving multiple
    updates of the same key.
    '''
    def __init__(self, source = None, sink = None, *args, **kwds):
        if args:
            initial = args[0]
        else:
            initial = dict()
        d = dict(initial, **kwds)

        self._source =  [d]
        if source is not None:
            if not isinstance(source, list):
                source = [source]
            self._source += source

        self._sink = [d]
        if sink is not None:
            if not isinstance(sink, list):
                sink = [sink]
            self._sink += sink

    def __getitem__(self, name):
        for source in self._source:
            value = source.get(name)
            if value: 
                self[name] = value # propagate to sinks
                return value
        raise KeyError, 'No key "%s" in %d sources' % (name, len(self._source))

    def __setitem__(self, name, value):
        for sink in self._sink:
            sink[name] = value
            
    def keys(self):
        s = set()
        for source in self._source:
            s.update(source.keys())
        return list(s)
    
        
