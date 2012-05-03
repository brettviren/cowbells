#!/bin/bash

# Illustrate how to build, setup and run this example
# Assumes Geant4/ROOT is built and set up

echo '(re)building'
make > make.log 2>&1

echo 'Generating geo'
python python/genpregeo.py ex06pregeo.root > ex06pregeo.log 2>&1

echo 'Running MC'
export LD_LIBRARY_PATH=lib/tgt_linuxx8664gcc:$LD_LIBRARY_PATH

echo 'Logging to geomRoot.log'
root -b -q -l 'run_g4.C("g4Config_geomRoot.C")' > geomRoot.log 2>&1

echo 'Logging to geomRootToGeant4.log'
root -b -q -l 'run_g4.C("g4Config_geomRootToGeant4.C")' > geomRootToGeant4.log 2>&1 

echo 'Logging to geomRoot_pregeo.log'
root -b -q -l 'run_g4.C("g4Config_geomRoot.C","ex06pregeo.root")' > geomRoot_pregeo.log 2>&1

echo 'Logging to geomRootToGeant4_pregeo.log'
root -b -q -l 'run_g4.C("g4Config_geomRootToGeant4.C","ex06pregeo.root")' > geomRootToGeant4_pregeo.log 2>&1 
