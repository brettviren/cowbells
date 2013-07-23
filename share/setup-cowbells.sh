#!/bin/bash

# An environment setup script for a locally built cowbells

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

cowbells-source-dir () {
    echo $(dirname $(dirname $(readlink -f $BASH_SOURCE)))
}

cowbells-build-dir () {
    local base=$(dirname $(cowbells-source-dir))
    local maybe
    for maybe in cowbells-build build ; do
	if [ -d "$base/$maybe" ] ; then
	    echo "$base/$maybe"
	    return
	fi
    done
}

cowbells-grinst-dir () {
    local maybe
    for maybe in ./grinst $HOME/grinst $(dirname $(cowbells-source-dir)/grinst) ; do
	if [ -d "$maybe" ] ; then
	    echo $(readlink -f $maybe)
	    return
	fi
    done
}

cowbells-virtualenv () {
    venvdir=$1; shift
    if [ -n "$venvdir" ] ; then
	if [ "$venvdir" = "$VIRTUAL_ENV" ] ; then
	    echo "Virtual env already made: $venvdir"
	    return
	else
	    deactivate
	fi
	echo "Making virutualenv in %venvdir"
	virtualenv $venvdir
	source $venvdir/bin/activate
	pip install networkx
	# fixme: need to install btdtwf
	return
    fi


    if [ -n "$VIRTUAL_ENV" ] ; then
	echo 'Virtual environment already found at $VIRTUAL_ENV'
	return
    fi

    local maybe
    for maybe in $HOME/venv/cowbells/bin ./venv/bin $(dirname $(cowbells-source-dir))/bin ; do
	if [ -d "$maybe" ] ; then
	    source $maybe/activate
	    return
	fi
    done
    echo "No virtual env.  Make one by specifying a directory: cowbells-virtual-env /path/to/dir"
    return
}

cowbells-setup () {
    local srcdir=$(cowbells-source-dir)
    local builddir=$1 ; shift
    if [ ! -d "$builddir" ] ; then
	builddir=$(cowbells-build-dir)
    fi
    if [ ! -d "$builddir" ] ; then
	echo "error: can not find builddir: \"$builddir\""
	echo "usage: setup-cowbells </path/to/cowbells/build/> [/path/to/cowbells/source]" 1>&2
	return
    fi
    local grinstdir=$(cowbells-grinst-dir)
    if [ ! -d "$grinstdir" ] ; then
	echo "can not find grinst directory"
	return
    fi
    pushd $grinstdir > /dev/null
    tosource="$(./grinst.sh cowbells.grinst setup all)"
    source $tosource
    popd > /dev/null

    export PATH=$(pathadd $builddir/bin $PATH)
    export LD_LIBRARY_PATH=$(pathadd $builddir/lib $LD_LIBRARY_PATH)
    export PYTHONPATH=$(pathadd $builddir/python $PYTHONPATH)

    if [ -z "$srcdir" ] ; then
	echo "Not including in-source setup, no such directory: $srcdir"
	return
    fi
    export PATH=$(pathadd $srcdir/share $PATH)
    export PYTHONPATH=$(pathadd $srcdir/python $PYTHONPATH) # don't require constant re-make'ing
    cowbells-virtualenv

    # fixme: this really should be installed into venv
    local maybe
    for maybe in $HOME/git/btdtwf $HOME/work/wbls/btdtwf $HOME/work/wbls/refactor/btdtwf ; do
	if [ -d "$maybe" ] ; then
	    export PYTHONPATH=$(pathadd $maybe $PYTHONPATH)
	    break
	fi
    done
}
