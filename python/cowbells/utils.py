#!/usr/bin/env python

def divine_cast(val):
    'Guess type value in string val'
    if not isinstance(val, basestring):
        return val              # already destringified

    if not val: return None
    if val[0] in ['"', "'"] and val[-1] in ['"', "'"]:
        return val[1:-1]
    try:
        ival = int(val)
    except ValueError:
        pass
    else:
        return ival

    try:
        fval = float(val)
    except ValueError:
        pass
    else:
        return fval

    return val
    
