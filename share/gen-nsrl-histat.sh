#!/bin/bash

# Make high statistics

base=gen-nsrl-histat
mkdir -p $base
pushd $base

do_seq () {

    reflect=$1 ; shift
    sample=$1 ; shift
    energy=$1 ; shift

    cfgname="13a-${sample}-ref${reflect}"
    jsonfile="${cfgname}.json"
    if [ ! -f "$jsonfile" ] ; then
	gennsrl.py $cfgname ${cfgname}.json reflectivity=$reflect sample=$sample
    else 
	echo "JSON config file already created: $jsonfile"
    fi
    jobname="${cfgname}-energy${energy}"

    for seq in $@
    do
	seqname="$jobname-seq$seq"
	logname="log.$seqname"
	cowbells.exe -m hits -n 1000 -p em,op,had \
	    -s "${seq}000" \
            -k "kin://beam?vertex=0,0,-5100&name=proton&direction=0,0,1&energy=$energy" \
            -o ${seqname}.root ${jsonfile} > $logname 2>&1
    done
}

# Case matters on the sample material name
do_seq 0.02 Water 475 {01..99} &
do_seq 0.02 WBLS01 475 {01..99} &
do_seq 0.02 Water 2000 {01..99} &
do_seq 0.02 WBLS01 2000 {01..99} &


