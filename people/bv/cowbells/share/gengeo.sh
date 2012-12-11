#!/bin/bash

# generate the input geometry files

cmd="python ../cowbells/python/cowbells/prep/gentubdet.py"


$cmd tubdet-water.root Water > gengeo-water.log 2>&1
$cmd tubdet-wbls.root WBLS > gengeo-wbls.log 2>&1
