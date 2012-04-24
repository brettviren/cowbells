#!/usr/bin/env python

def test_load_libs():
    'Basic loading of shared libraries'

    import cowbells
    assert cowbells.geo, 'No geo manager'
    assert cowbells.app, 'No MC app object'
    assert cowbells.mc, 'No gMC'
    assert cowbells.mc.GetName(), 'Failed to get MC name'
    return

if __name__ == '__main__':
    test_load_libs()


