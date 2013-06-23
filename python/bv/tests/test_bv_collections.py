#!/usr/bin/env python
'''
'''

from bv.collections import ChainedDict

def test_chaineddict():
    d1 = dict(a=1)
    d2 = dict(a=2,b=3,z=42)
    d3 = dict()
    cd = ChainedDict(source = [d1,d2], sink = d3)
    assert cd['a'] == 1
    assert cd['b'] == 3
    assert not cd.get('c')
    cd['c'] = 4
    assert cd['c'] == 4
    assert d3['c'] == 4
    assert set(cd.keys()) == set(['a','b','c','z'])
    assert len(d3) == 1
    print cd


if '__main__' == __name__:
    test_chaineddict()
