# -*- bash -*- setup script for release 21

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
if [[ ! $ATLAS_LOCAL_ASETUP_VERSION ]] ; then
    echo -n "setting up local ATLAS environment..."
    setupATLAS -q

    lsetup asetup -q
    . $AtlasSetup/scripts/asetup.sh 21.0.6,here > /dev/null

    echo "done"
else
    echo "ATLAS environment is already setup, not setting up again"
fi


if [[ ! -d build ]] ; then
    ./scripts/build.sh
else
    echo 'already built, run `./scripts/build.sh` to rebuild'
fi

. build/$AtlasEvent_PLATFORM/setup.sh

echo 'run `. /build/checkout-*` to get more packages'
