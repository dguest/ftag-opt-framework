#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi


SCRIPT_DIR=$(dirname $BASH_SOURCE)
JO=jobOptions_ipmp.py
DS=mc15_13TeV:mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_FTAG2.e3698_s2608_s2183_r7725_r7676_p2625/

${SCRIPT_DIR}/ftag-grid-sub.sh -j $JO -d $DS -u



