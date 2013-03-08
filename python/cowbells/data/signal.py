#!/usr/bin/env python
'''
Opperate on signals.
'''

from array import array

def chi2(signal):
    '''Return the chi^2 sum of the signal w.r.t. its mean'''
    avg = float(sum(signal))/len(signal)
    return sum(map(lambda x: (x-avg)**2, signal))


def repeate_chi2(seed, signal):
    '''Collect "chi^2" by repeatedly comparing seed to signal.  

    Array of len(seed) is returned with each element i compared to
    i+N*step in the signal for N in [0,...].  The step is len(seed).'''

    step = len(seed)
    ret = array('f',[0.0]*step)
    for sind, sig in enumerate(signal):
        ind = sind%step
        ret[ind] += (seed[ind] - sig)**2
    return ret

