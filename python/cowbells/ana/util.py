#!/usr/bin/env python
'''
Some general utility code
'''



class StringParams(object):
    '''
    A bag of string parameters.  When a paramter is retrieved from the
    bag it is interpolated against all parameters.
    '''
    def __init__(self, params=None,  **kwds):
        if params is None: params = {}
        params.update(kwds)
        self.__dict__['_params'] = dict(params)
        self.__dict__['_locked'] = False
    def lock(self, locked=True):
        '''
        Lock the params.  

        If locked then then accessing a key via a data memember throws
        KeyError if the key does not exist.
        '''
        self.__dict__['_locked'] = locked
    def __str__(self):
        return '\n'.join(['%s -> %s' % (k,v) for k,v in sorted(self._params.iteritems())])
    def copy(self, **kwds):
        'Return copy of self'
        return StringParams(self.dict(),**kwds)
    def dict(self):
        'Return raw values as dictionary'
        return self.__dict__['_params']
    def set(self, name, value):
        'Set named parameter to value'
        self.__dict__['_params'][name] = value
    def string(self, s):
        'Interpolate a string with the bags (raw) values'
        return str(s) % self.__dict__['_params']
    def raw(self, name, default = ""):
        'Return named value or default with no interpolation'
        return self.__dict__['_params'].get(name,default)
    def __getattr__(self, name):
        if self._locked:
            val = self._params[name] # KeyError on unknown param name
            return self.string(val)
        return self.get(name)   # will return default on KeyError
    def __setattr__(self, name, value):
        self.set(name, value)
    def get(self, name, default = ""):
        'Return value of name interpolated as string using all values'
        return self.string(self.raw(name, default))

    pass

