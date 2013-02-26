#!/usr/bin/env python
'''
Find peaks
'''

def downhill(signal, zero):
    maxy = max(signal)
    if maxy <= zero:
        return
    mini = signal.index(maxy)

    left = mini
    while left > 0:
        if signal[left-1] <= zero:
            break
        if signal[left-1] > signal[left]:
            break
        left -= 1
        continue

    right = mini
    while right < len(signal)-1:
        if signal[right+1] <= zero:
            break
        if signal[right+1] > signal[right]:
            break
        right += 1
        continue

    return (max(0,left), min(len(signal),right+1))
    
def downhills(signal, zero, noise = None):
    if noise is None:
        noise = zero
    
    ret = []
    signal = list(signal)
    while True:
        lr = downhill(signal, zero)
        if lr is None:
            break
        l,r = lr
        chunk = signal[l:r]
        if max(chunk) < noise:
            break
        signal[l:r] = [zero]*len(chunk)
        ret.append((l,r))
        #print (l,r),chunk
        continue
    return ret

