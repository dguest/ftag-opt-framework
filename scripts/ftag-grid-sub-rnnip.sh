#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi


SCRIPT_DIR=$(dirname $BASH_SOURCE)
JO=jobOptions_ipmp.py
DS=mc16_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.AOD.e3698_s2997_r8903_r8906/

${SCRIPT_DIR}/ftag-grid-sub.sh -j $JO -d $DS -u



