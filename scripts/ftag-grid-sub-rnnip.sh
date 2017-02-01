#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi


SCRIPT_DIR=$(dirname $BASH_SOURCE)
JO=jobOptions_ipmp.py
DSS=(
    mc16_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.AOD.e3698_s2997_r8903_r8906/
    mc16_13TeV.427080.Pythia8EvtGen_A14NNPDF23LO_flatpT_Zprime.merge.AOD.e5362_s2997_r8903_r8906/
)

for DS in ${DSS[*]}; do
    ${SCRIPT_DIR}/ftag-grid-sub.sh -j $JO -d $DS -u -z job.tar
done



