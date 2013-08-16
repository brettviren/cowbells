#!/bin/bash
export cowbells_share_dir=$(dirname $(readlink -f $BASH_SOURCE))
source $cowbells_share_dir/setup-cowbells.sh
cowbells-setup
PS1="(cb)$PS1"
