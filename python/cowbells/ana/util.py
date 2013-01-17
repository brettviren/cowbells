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
    def __str__(self):
        return '\n'.join(['%s -> %s' % (k,v) for k,v in sorted(self._params.iteritems())])
    def dict(self):
        'Return as dictionary'
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
        return self.get(name)
    def __setattr__(self, name, value):
        self.set(name, value)
    def get(self, name, default = ""):
        'Return value of name interpolated as string using all values'
        return self.string(self.raw(name, default))

    pass

