#!/bin/bash

# make dE/dx plots

# can call do_mat in parallel
do_mat () {
    matsam=$1 ; shift
    matno=$1 ; shift
    matname=$1 ; shift
    particles=$1 ; shift

    for stepsize in 1.0 0.01 ; do
	input=${particles}-????-${matsam}-10000-${stepsize}.root
	python ../cowbells/python/cowbells/ana/dedx.py multiplot $matno $input
	mv dedx_multiplot_${matname}.pdf dedx_multiplot_${matname}_step${stepsize}.pdf
    done
}


# do_mat water 10 Teflon protons &
# do_mat water  0 Water  protons &
# do_mat wbls   1 WBLS   protons &

do_mat water 10 Teflon muons &
do_mat water  0 Water  muons &
do_mat wbls   1 WBLS   muons &
