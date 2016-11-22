#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi

# ________________________________________________________________________
# build all the things
cd WorkArea/cmt
cmt bro cmt config
cmt bro cmt make
