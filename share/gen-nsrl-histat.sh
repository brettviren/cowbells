#!/bin/bash

# Make high statistics

base=gen-nsrl-histat
mkdir -p $base
pushd $base

ref='0.02'
cfgname=13a-water-ref$ref
gennsrl.py $cfgname ${cfgname}.json reflectivity=$ref

do_seq () {
    for seq in $@
    do
	seqname="$cfgname-seq$seq"
	logname="log.$seqname"
	cowbells.exe -m hits -n 1000 -p em,op,had \
	    -s ${seq}000 \
            -k 'kin://beam?vertex=0,0,-5100&name=proton&direction=0,0,1&energy=2000' \
            -o ${seqname}.root ${cfgname}.json > $logname 2>&1
    done
}

#do_seq {01..05} &
#do_seq {06..10} &
do_seq {11..20} &
do_seq {21..30} &

