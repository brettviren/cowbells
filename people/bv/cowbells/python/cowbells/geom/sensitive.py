#!/usr/bin/env python
'''
Describe sensitive detectors
'''

import base, volumes, placements

store = []


class SensitiveDetector(base.Base):
    def __init__(self, name, hcname, logvol, world_pv = None):
        '''
        Describe a sensitive detector associated with the named
        hitcollection and logical volume.  If world_pv is None it is
        assumed to be the first one in placements.store.
        '''
        if isinstance(logvol, volumes.LogicalVolume):
            logvol = logvol.name
        assert volumes.get(logvol), 'No such logical volume: %s' % logvol
        if not world_pv:
            world_pv = placements.store[0].name
        self.__dict__ = dict(name=name, hcname=hcname, logvol=logvol, world_pv=world_pv)
        store.append(self)
        return

    def touchables(self):
        '''
        Return the touchable path from the world PV to the placement
        of the logvol.
        '''
        ret = []
        for pvs, lvname in placements.walk(self.world_pv):
            if lvname != self.logvol: continue

            path = []
            for pvname in pvs:
                pv = placements.get(pvname)
                path.append('%s:%d'%(pvname,pv.copy))
                continue
            ret.append('/'.join(path))
            continue
        return ret

    def pod(self):
        '''
        Return self as a plain old data structure.
        '''
        me = dict(self.__dict__)
        me['touchables'] = self.touchables()
        return me
    pass

def pod(): return base.pod(store)
