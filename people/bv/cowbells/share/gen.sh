#!/bin/bash

cb=../build/bin/cowbells.exe

sample=$1 ; shift
particle=$1 ; shift
stepsize=$1 ; shift
count=$1 ; shift

if [ -z "$sample" ] ; then
    sample=wbls
fi
if [ -z "$particle" ] ; then
    particle=proton
fi
if [ -z "$stepsize" ] ; then
    stepsize=1.0
fi
if [ -z "$count" ] ; then
    count=10000
fi

tag=${sample}-${count}-${stepsize}

args="-g tubdet-${sample}.root -p em -n $count --defcut $stepsize"


#150 250 500 1000 1500 2000 2500
#210 475 2500
#150 1000 1500 2000

for en in 150 210 475 1000 1500 2000  2500
do
    enstr=$(printf '%04d' $en)
    #echo $enstr

    kin="kin://beam?name=${particle}&vertex=-100,0,0&direction=1,0,0&energy=${en}&count=1"
    #echo $kin
    out="${particle}s-${enstr}-${tag}.root"
    log="${particle}s-${enstr}-${tag}.log"
    echo "$cb $args -k $kin -o $out > $log" 
    echo "$cb $args -k $kin -o $out" > $log
    $cb $args -k "$kin" -o $out >> $log 2>&1 &
done
