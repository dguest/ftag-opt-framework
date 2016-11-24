#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi

# ________________________________________________________________________
# checkout packages (some are commented out because we may not need them)

mkdir -p packages
(
    cd packages
    pkgco.py BTagging-00-07-55-branch
    pkgco.py JetTagTools-01-00-82-branch
)
./scripts/build-r207.sh
