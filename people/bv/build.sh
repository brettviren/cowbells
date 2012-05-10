#!/bin/bash

base=/data3/lbne/bv/wbls/install
cmake -DGeant4_DIR=$base/geant4/4.9.5.p01/lib64/Geant4-9.5.1 \
      -DROOT_DIR=$base/root/5.32.02 \
      -DGCCXML_home=$base/gccxml/0.9.0_20120309 \
      -DROOT_genreflex_cmd=genreflex \
      -DVGM_DIR=$VGM_INSTALL \
      -DHEPMC_DIR=$base/hepmc/2.06.08 \
      ../cowbells && \
make VERBOSE=1 && \
echo ok

