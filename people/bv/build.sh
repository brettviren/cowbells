#!/bin/bash
cmake -DGeant4_DIR=/data3/lbne/bv/wbls/install/geant4/4.9.5.p01/lib64/Geant4-9.5.1 \
      -DROOT_DIR=/data3/lbne/bv/wbls/install/root/5.32.02 \
      -DGCCXML_home=/data3/lbne/bv/wbls/install/gccxml/0.9.0_20120309 \
      -DROOT_genreflex_cmd=genreflex \
      -DVGM_DIR=$VGM_INSTALL \
      ../cowbells && \
make VERBOSE=1 && \
echo ok

