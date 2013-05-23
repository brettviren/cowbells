#!/bin/bash

base=gen-nsrl-histat
mkdir -p $base
pushd $base
for ref in 0.00 # 0.02 0.05 0.10 0.25 0.50 1.00
do
    name=13a-water-ref$ref

    gennsrl.py $name ${name}.json reflectivity=$ref
    cowbells.exe -m hits -n 1000 -p em,op \
                 -k 'kin://beam?vertex=0,0,-5100&name=proton&direction=0,0,1&energy=2000' \
                 -o ${name}.root ${name}.json > log.$name 2>&1 &
    echo $base/log.$name
done

popd
echo "results going to $base"

