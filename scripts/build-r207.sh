#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi

mkdir -p packages
cd packages

# ________________________________________________________________________
# build all the things
setupWorkArea.py
cd $TestArea/WorkArea/cmt
cmt bro cmt config
cmt bro cmt make
