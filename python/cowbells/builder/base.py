#!/usr/bin/env python
'''
Base for a generator and  builder of geometry
'''
from cowbells.geom import materials

class Struct:
    def __init__(self, **entries): 
        self.update(entries)
    def update(self, d):
        self.__dict__.update(d)

class Builder(object):
    # redefine/update with parameters of the builder
    default_params = { }

    # redefine/update map of parts to material names
    default_parts = { }


    def __init__(self, **params):
        '''
        Create a builder and check if parameters and materials are available
        '''
        self._top = None
        self.params = dict(self.default_params)
        used = set()
        for k,v in params.items():
            if k in self.params.keys():
                self.params[k] = v
                used.add(k)
            
        self.parts = dict(self.default_parts)
        for k,v in params.items():
            if k in self.parts.keys():
                self.parts[k] = v
                used.add(k)

        unused = set(params.keys()).difference(used)
        if unused:
            print 'Warning: unknown builder arguments in Builder "%s": %s' % (self.basename(), unused)

        # check all needed materials are defined
        for part,mat in self.parts.items():
            assert materials.get(mat), 'No material "%s" for part "%s"' % (mat, part)
            continue

        return

    def pp(self):
        'Return tuple (parameters, parts)'
        return (Struct(**self.params), Struct(**self.parts))

    def top(self):
        '''
        Return top logical volume.
        '''
        if self._top: return self._top
        
        _top = self.make_logical_volumes()
        return _top

    def basename(self):
        return ""

    def pvname(self,part):
        assert part in self.parts.keys(), 'Unknown part: %s'%part
        return 'pv'+self.basename()+self.parts[part]+part

    def lvname(self,part):
        assert part in self.parts.keys(), 'Unknown part: %s'%part
        return 'lv'+self.basename()+self.parts[part]+part

    def shapename(self, part):
        assert part in self.parts.keys(), 'Unknown part: %s'%part
        b = self.basename().lower()
        if b: b += '_'
        return b+self.parts[part].lower()+'_'+part.lower()+'_shape'

    def surfname(self, part):
        assert part in self.parts.keys(), 'Unknown part: %s'%part
        b = self.basename().lower()
        if b: b += '_'
        return b+self.parts[part].lower()+'_'+part.lower()+'_surface'

    def sensname(self, part):
        assert part in self.parts.keys(), 'Unknown part: %s'%part
        return self.basename() + self.parts[part]+part+'_SD'

    def hitcolname(self, part):
        assert part in self.parts.keys(), 'Unknown part: %s'%part
        return self.basename() + self.parts[part]+part+'_HC'
        

    def make_logical_volumes(self):
        '''
        Subclass must make any logical volumes and return a single
        top-level one.
        '''
        notimplemented
        return None

    def place(self):
        '''
        Subclass may do any internal placements.
        '''
        return

    def sensitive(self):
        '''
        Subclass may define and return sensitive detector touchables.
        '''
        return
