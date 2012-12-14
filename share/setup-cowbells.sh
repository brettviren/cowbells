#!/usr/bin/env python

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


setup-cowbells () {
    local builddir=$1 ; shift
    if [ ! -d "$builddir" ] ; then
	echo "usage: setup-cowbells </path/to/cowbells/build/> [/path/to/cowbells/source]" 1>&2
	return
    fi
    export PATH=$(pathadd $builddir/bin $PATH)
    export LD_LIBRARY_PATH=$(pathadd $builddir/lib $LD_LIBRARY_PATH)
    export PYTHONPATH=$(pathadd $builddir/python $PYTHONPATH)

    local srcdir=$1 ; shift
    if [ -z "$srcdir" ] ; then
	return
    fi
    export PATH=$(pathadd $srcdir/share $PATH)
    export PYTHONPATH=$(pathadd $srcdir/python $PYTHONPATH) # don't require constant re-make'ing

}
