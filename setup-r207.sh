# -*- bash -*- setup script for release 20.7

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
alias setupATLAS='source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh'
echo -n "setting up local ATLAS environment..."
setupATLAS -q
lsetup asetup -q
. $AtlasSetup/scripts/asetup.sh 20.7.8.3,AtlasDerivation,here > /dev/null
echo "done"

# link job options to the run directory
(
    cd $TestArea
    mkdir -p run
    cd $TestArea/run
    for JO in $TestArea/btagIBLAnalysis/share/*.py; do
        ln -sf $JO
    done

)

if [[ ! -d packages ]] ; then
    ./scripts/build-r207.sh
else
    echo 'found `packages` directory, not building.'
    echo 'Use `./scripts/build-r207.sh` to rebuild'
fi
echo 'use `./scripts/checkout-dev-r207.sh` to get development branches'
