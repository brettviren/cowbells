#!/usr/bin/env python
'''
Test making some water
'''

def test_geo():
    'Make some water medium'
    import cowbells.material.water as water
    med = water.medium()
    print med

if __name__ == '__main__':
    test_geo()
