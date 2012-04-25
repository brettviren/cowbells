#!/usr/bin/env python
'''
Material definitions.

Modules here produce ROOT TGeo material/mixture/medium and properties.
They should follow these use patterns:

  from material import stuff

  ## While defining geometry:
  stuff_med = stuff.medium()

  ## then, after geometry is defined:
  # mc = ...
  stuff.register(mc)

The "mc" should be a TVirtualMC object.

Material modules can make use of helpers in the material.util module.
'''

