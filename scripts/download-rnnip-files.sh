#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi

cd run/

# download the configs
wget dguest.web.cern.ch/dguest/2016-09-12/ipmp.json
wget dguest.web.cern.ch/dguest/2016-11-21/ipz_pTreweight.json
wget dguest.web.cern.ch/dguest/2016-11-21/ipz.json
