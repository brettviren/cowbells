#!/bin/bash

# run ./inst.sh help

# set -x

help () {
    cat <<EOF
inst.sh - install 3rd party code of fixed versions and set up user environment.

To build and install:

  ./inst.sh build_all

To set up your (bash) shell

  eval \$(./inst.sh setup_all)

All installation and setup is done w.r.t. the CWD.

EOF
    return
}


export INSTBASE=$(pwd)
install_area=$INSTBASE/opt
build_area=$INSTBASE/bld

setup_inst () {
    echo "export INSTBASE=$INSTBASE"
}
isinpath () {
    local thing=$1 ; shift

    local other="" 
    for other in $(echo $@ | tr : ' ')
    do
	if [ "$thing" = "$other" ] ; then
	    echo $thing
	    return
	fi
    done
    return
}

pathadd () {
    local ret=""
    local comma=""
    local thing=""
    for thing in $(echo $@ | tr : ' ')
    do
	if [ ! -d "$thing" ] ; then
	    continue
	fi


	if [ -n "$(isinpath $thing $ret)" ] ; then
	    continue
	fi
	ret="${ret}${comma}${thing}"
	comma=":"
    done
    echo $ret
}

assuredir () {
    local dirname=$1; shift
    if [ -d $dirname ] ; then
	echo "Already have directory: \"$dirname\""
	return
    fi
    mkdir -p $dirname
}

download () {
    url=$1 ; shift
    file=$(basename $url)
    if [ -f $file ] ; then
	echo "Already downloaded URL: \"$url\""
	return
    fi
    wget $url
}

svnco () {
    url=$1 ; shift
    tgt=$1 ; shift
    if [ -d $tgt ] ; then
	echo "Already checked out svn repo $url to $tgt"
	return
    fi
    if [ ! -d ${tgt}.svn ] ; then
	svn co $url ${tgt}.svn
    fi
    mkdir $tgt
    pushd ${tgt}.svn
    tar -cf - $(find . -print | grep -v .svn) | (cd ../$tgt; tar -xf -)
    popd
}

build_cmake () {
    local cmake_build=$build_area/cmake_build
    local cmake_prefix=$install_area/cmake

    assuredir $cmake_build
    pushd $cmake_build

    download http://www.cmake.org/files/v2.8/cmake-2.8.7.tar.gz
    tar -xzvf cmake-2.8.7.tar.gz
    cd cmake-2.8.7
    ./bootstrap  --prefix=$cmake_prefix
    make
    make install

    popd
}

setup_cmake () {
    local cmake_prefix=$install_area/cmake
    PATH=$(pathadd $cmake_prefix/bin $PATH)
    echo "export PATH=$PATH"
}

build_python () {
    local python_build=$build_area/python_build
    local python_prefix=$install_area/python

    assuredir $python_build
    pushd $python_build

    download http://python.org/ftp/python/2.7.2/Python-2.7.2.tgz
    tar -xzvf Python-2.7.2.tgz 
    cd Python-2.7.2
    ./configure --prefix=$python_prefix --enable-shared 
    make 
    make install

    popd
}

setup_python () {
    local python_prefix=$install_area/python
    PATH=$(pathadd $python_prefix/bin $PATH)
    LD_LIBRARY_PATH=$(pathadd $python_prefix/lib $LD_LIBRARY_PATH)
    echo "export PATH=$PATH"
    echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
}

build_ipython () {
    eval $(setup_python)

    local ipython_build=$build_area/ipython_build
    local ipython_prefix=$install_area/ipython

    assuredir $ipython_build
    pushd $ipython_build

    download http://archive.ipython.org/release/0.12/ipython-0.12.tar.gz
    tar -xzvf ipython-0.12.tar.gz 
    cd ipython-0.12
    python setup.py install --prefix=$ipython_prefix
    
    popd
}

setup_ipython () {
    eval $(setup_python)
    setup_python
    local ipython_prefix=$install_area/ipython
    PATH=$(pathadd $ipython_prefix/bin $PATH)
    PYTHONPATH=$(pathadd $ipython_prefix/lib/python2.7/site-packages $PYTHONPATH)
    echo "export PATH=$PATH"
    echo "export PYTHONPATH=$PYTHONPATH"
}

build_geant4 () {
    eval $(setup_cmake)
    local geant4_build=$build_area/geant4
    local geant4_prefix=$install_area/geant4
    local geant4_verstr=geant4.9.5.p01

    assuredir $geant4_build
    pushd $geant4_build

    download http://geant4.cern.ch/support/source/${geant4_verstr}.tar.gz
    tar -xzvf ${geant4_verstr}.tar.gz 
    assuredir ${geant4_verstr}-cmake-build
    pushd ${geant4_verstr}-cmake-build
    cmake -DGEANT4_INSTALL_DATA=1 \
          -DCMAKE_INSTALL_PREFIX=$geant4_prefix \
          -DGEANT4_USE_OPENGL_X11=ON \
          -DGEANT4_USE_G3TOG4=1 $geant4_build/${geant4_verstr}

    make -j8  # wheeeeee! very fast!
    make install # puts stuff in lib64 

    pushd $geant4_prefix
    ln -s share/Geant4-9.5.1/geant4make/config . # give it that old-school flava
    popd
}

setup_geant4 () {
    eval $(setup_cmake)
    local geant4_prefix="$install_area/geant4"
    local geant4_data="$geant4_prefix/share/Geant4-9.5.1/data"
    PATH=$(pathadd $geant4_prefix/bin $PATH)
    LD_LIBRARY_PATH=$(pathadd $geant4_prefix/lib64 $LD_LIBRARY_PATH)
    cat <<EOF
export PATH=$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH

export G4ABLADATA="$geant4_data/G4ABLA3.0"
export G4LEDATA="$geant4_data/G4EMLOW6.23"
export G4LEVELGAMMADATA="$geant4_data/PhotonEvaporation2.2"
export G4NEUTRONHPDATA="$geant4_data/G4NDL4.0"
export G4NEUTRONXSDATA="$geant4_data/G4NEUTRONXS1.1"
export G4PIIDATA="$geant4_data/G4PII1.3"
export G4RADIOACTIVEDATA="$geant4_data/RadioactiveDecay3.4"
export G4REALSURFACEDATA="$geant4_data/RealSurface1.0"
EOF
}

build_boost () {
    eval $(setup_python)
    local boost_build=$build_area/boost
    local boost_prefix=$install_area/boost
    local python_prefix=$install_area/python

    assuredir $boost_build
    pushd $boost_build
    
    download http://downloads.sourceforge.net/project/boost/boost/1.49.0/boost_1_49_0.tar.gz
    tar -xzf boost_1_49_0.tar.gz 
    cd boost_1_49_0
    ./bootstrap.sh --prefix=$boost_prefix --with-python-root=$python_prefix
    ./b2 install
}

build_g4py () {
    eval $(setup_python)
    eval $(setup_geant4)
    local geant4_build=$build_area/geant4
    local g4py_build=$geant4_build/geant4.9.5.p01/environments/g4py
    local g4py_prefix=$install_area/g4py

    local geant4_prefix=$install_area/geant4
    local boost_prefix=$install_area/boost
    local python_prefix=$install_area/python
    
    pushd $g4py_build
    ./configure linux64 \
	--prefix=$g4py_prefix \
	--with-boost-incdir=$boost_prefix/include \
	--with-boost-libdir=$boost_prefix/lib \
	--with-python-incdir=$python_prefix/include/python2.7 \
	--with-python-libdir=$python_prefix/lib \
	--with-g4-incdir=$geant4_prefix/include/Geant4 \
	--with-g4-libdir=$geant4_prefix/lib64 \
	--with-clhep-incdir=$geant4_prefix/include/Geant4 \
	--with-clhep-libdir=$geant4_prefix/lib64 \
	--with-clhep-lib=G4clhep

    # Kludge warning: need to first comment out the private copy
    # constructor in G4Run.hh and G4Event.hh in the installed Geant4
    # headers in order to make successfully.

    make
    make install

}

setup_g4py () {
    eval $(setup_python)
    eval $(setup_geant4)
    setup_python
    setup_geant4
    
    local g4py_prefix=$install_area/g4py

    PYTHONPATH=$(pathadd $g4py_prefix/lib $PYTHONPATH)
    cat <<EOF
export PYTHONPATH=$PYTHONPATH
EOF
}

build_root_oldfashioned () {
    eval $(setup_python)
    local root_build=$build_area/root
    local root_prefix=$install_area/root
    local python_prefix=$install_area/python

    assuredir $root_build
    pushd $root_build
    download ftp://root.cern.ch/root/root_v5.32.02.source.tar.gz
    tar -xzvf root_v5.32.02.source.tar.gz 
    mv root root_v5.32.02
    cd root_v5.32.02

    ./configure --with-python-libdir=${python_prefix}/lib --with-python-incdir=${python_prefix}/include/python2.7
    make -j8
    export ROOTSYS=$root_prefix
    make install
}

# http://root.cern.ch/drupal/content/building-root-cmake
build_root () {
    eval $(setup_python)
    eval $(setup_cmake)
    local root_build=$build_area/root
    local root_prefix=$install_area/root
    local python_prefix=$install_area/python

    assuredir $root_build/root_v5.32.02-build
    pushd $root_build
    download ftp://root.cern.ch/root/root_v5.32.02.source.tar.gz
    if [ ! -d root_v5.32.02-source ] ; then
	tar -xzvf root_v5.32.02.source.tar.gz 
	mv root root_v5.32.02-source
    fi

    cd root_v5.32.02-build
    cmake $root_build/root_v5.32.02-source -DCMAKE_INSTALL_PREFIX=$root_prefix
    make -j8
    make install
}

setup_root () {
    eval $(setup_python)
    setup_python
    local root_prefix=$install_area/root

    ROOTSYS=$root_prefix
    PATH=$(pathadd $root_prefix/bin $PATH)
    LD_LIBRARY_PATH=$(pathadd $root_prefix/lib $LD_LIBRARY_PATH)
    PYTHONPATH=$(pathadd $root_prefix/lib $PYTHONPATH)
    cat <<EOF
export ROOTSYS=$ROOTSYS
export PATH=$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
EOF
}

build_vgm () {
    eval $(setup_geant4)
    eval $(setup_root)
    local vgm_build=$build_area/vgm
    local vgm_prefix=$install_area/vgm

    assuredir $vgm_build
    pushd $vgm_build

    svnco https://vgm.svn.sourceforge.net/svnroot/vgm/tags/v3-05/vgm vgm.3.05 
    cd vgm.3.05 
    export VGM_INSTALL=`pwd`
    export VGM_SYSTEM=Linux-g++

    ## maybe this is needed just for vgm.  It should NOT be turned on for building geant4_vmc...
    #local geant4_prefix=$install_area/geant4
    #export G4INSTALL=$geant4_prefix
    #export G4INCLUDE=$G4INSTALL/include/Geant4

    cd packages
    make

    cd ..
    assuredir $vgm_prefix
    tar -cf - lib $(find packages -name '*.h' -print) | (cd $vgm_prefix; tar -xf -)
}

setup_vgm () {
    eval $(setup_root)
    eval $(setup_geant4)
    setup_root
    setup_geant4
    local vgm_prefix=$install_area/vgm

    LD_LIBRARY_PATH=$(pathadd $vgm_prefix/lib/Linux-g++ $LD_LIBRARY_PATH)

    cat <<EOF
export VGM_INSTALL=$vgm_prefix
export VGM_SYSTEM=Linux-g++
export USE_VGM=1
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH
EOF
}
    

build_geant4_vmc () {
    eval $(setup_vgm)
    local g4vmc_build=$build_area/g4vmc
    local g4vmc_prefix=$install_area/g4vmc
    
    assuredir $g4vmc_build
    pushd $g4vmc_build

    download ftp://root.cern.ch/root/vmc/geant4_vmc.2.13a.tar.gz
    tar -xzvf geant4_vmc.2.13a.tar.gz
    cd geant4_vmc
    make
    assuredir $g4vmc_prefix
    tar -cf - include lib examples | (cd $g4vmc_prefix; tar -xf -)
}

setup_geant4_vmc () {
    eval $(setup_vgm)
    setup_vgm
    local g4vmc_prefix=$install_area/g4vmc
    
    LD_LIBRARY_PATH=$(pathadd $g4vmc_prefix/lib/tgt_linuxx8664gcc $LD_LIBRARY_PATH)
    echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH"
}


log () {
    local logfile=$1 ; shift
    #echo "Logging to \"$logfile\""
    $@ >> $logfile 2>&1
}

build_all () {
    local logfile='inst.log'

    log $logfile build_cmake
    log $logfile build_python
    log $logfile build_ipython
    log $logfile build_geant4
    log $logfile build_root
    log $logfile build_vgm
    log $logfile build_geant4_vmc
}

setup_all () {
    setup_inst
    setup_cmake
    setup_python
    setup_ipython
    setup_geant4_vmc 		# this also gets g4, root, vgm
}

$@
