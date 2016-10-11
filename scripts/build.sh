#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi

SCRIPT_DIR=scripts
BUILD_DIR=build

if [[ ! -d $SCRIPT_DIR ]]; then
    echo "no build dir, quitting" >&2
    exit 1
fi

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake ..
make -j 4
