#!/usr/bin/env bash

if [[ $- == *i* ]] ; then
    echo "Don't source me!" >&2
    return 1
else
    set -eu
fi

# defaults
JO=jobOptions_ipmp.py
DS=mc15_13TeV:mc15_13TeV.410000.PowhegPythiaEvtGen_P2012_ttbar_hdamp172p5_nonallhad.merge.DAOD_FTAG2.e3698_s2608_s2183_r7725_r7676_p2625/

_usage() {
    echo "usage: ${0##*/} [-h] [options]"
}
_help() {
    _usage
    cat <<EOF

Submit job over some dataset. Internally figures out the output
dataset name and special permissions (based on the options you setup
with voms).

Note that this should be run from the directory where your job options
live.

Requires that:
 - You're in a git-controlled directory
 - All changes have been committed

Options:
 -h: get help
 -n <number>: n files to use (default all)
 -j <python script>: jobOptions to use (default ${JO})
 -d <dataset>: input dataset to use (default ${DS})
 -t <tag>: tag for output dataset
 -z <file>: create / submit a gziped tarball
 -u: upload local json files
 -e: test run, just echo command
 -f: force submit even if uncommited changes exist

EOF
}

OPTS=""
ECHO=""
TAG=""
UPLOAD_LOCAL=""
ZIP=""
FORCE=""
while getopts ":hn:j:d:t:z:uef" opt $@; do
    case $opt in
        h) _help; exit 1;;
        n) OPTS+=" --nFiles ${OPTARG}";;
        j) JO=${OPTARG};;
        d) DS=${OPTARG};;
        t) TAG=${OPTARG};;
        z) ZIP=${OPTARG};;
        u) UPLOAD_LOCAL=1;;
        e) ECHO=1;;
        f) FORCE=1;;
        # handle errors
        \?) _usage; echo "Unknown option: -$OPTARG" >&2; exit 1;;
        :) _usage; echo "Missing argument for -$OPTARG" >&2; exit 1;;
        *) _usage; echo "Unimplemented option: -$OPTARG" >&2; exit 1;;
    esac
done

if ! git diff-index --quiet HEAD && [[ ! $FORCE ]]; then
    echo "ERROR: uncommitted changes in local area, please commit them" >&2
    exit 1
fi

SCRIPT_DIR=$(dirname $BASH_SOURCE)
OUT_OPTS=$(${SCRIPT_DIR}/ftag-grid-name.sh $DS $TAG)

# setup options
OPTS=${OUT_OPTS}
OPTS+=" --nFilesPerJob 5"
# OPTS+=" --excludedSite=ANALY_FZK,ANALY_FZK_HI"
if [[ -n $UPLOAD_LOCAL ]] ; then
    JSON_FILES=$(echo *.json | tr " " ",")
    OPTS+=" --extFile $JSON_FILES"
fi

# pack stuff into a tarball before submitting
if [[ -n $ZIP ]] ; then
    if [[ ! -f $ZIP ]]; then
        echo "making tarball of local files: ${ZIP}" >&2
        pathena ${JO} --outTarBall=${ZIP} $OPTS --noSubmit
    fi
    OPTS+=" --inTarBall=${ZIP}"
fi

echo "submitting over dataset ${DS}" >&2
CMD="pathena ${JO} --inDS $DS $OPTS"
if [[ -n $ECHO ]] ; then
    echo $CMD >&2
else
    eval $CMD
fi
echo $OUT_OPTS | cut -d ' ' -f 2
