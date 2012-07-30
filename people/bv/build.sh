#!/bin/bash

build_dir=/data3/lbne/bv/wbls/sim/WbLS/people/bv/cowbells-cmake-build/
mkdir -p $build_dir

log=/data3/lbne/bv/wbls/sim/WbLS/people/bv/cowbells-cmake-build/build.log

do_it () {
    base=/data3/lbne/bv/wbls/install
    pushd $build_dir
    cmake -DGeant4_DIR=$base/geant4/4.9.5.p01/lib64/Geant4-9.5.1 \
	-DROOT_DIR=$base/root/5.32.02 \
	-DGCCXML_home=$base/gccxml/0.9.0_20120309 \
	-DROOT_genreflex_cmd=genreflex \
	-DVGM_DIR=$VGM_INSTALL \
	-DHEPMC_DIR=$base/hepmc/2.06.08 \
	../cowbells && \
	make VERBOSE=1 && \
	popd
    echo ok, log in $log
}

do_it 2>&1 | tee $log

