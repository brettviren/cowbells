#!/bin/bash

rundir="/home/bviren/work/wbls/refactor/run/tubs"

for sample in water ; do
    for energy in 210 475 2000; do
	for particle in proton ; do
	    for nevents in 1000 ; do
		base="nsrl12c-${sample}-${energy}-${particle}-${nevents}"
		rootfile="$rundir/${base}.root"
		if [ ! -f "$rootfile" ] ; then
		    echo "No such input file: $rootfile"
		    exit 1
		fi
		echo $base
		python tubs.py $rootfile $base > ${base}.log 2>&1 &
	    done
	done
    done
done
