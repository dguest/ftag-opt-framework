# for some reason this script _has_ to be sourced, something about
# svnco not working otherwise

mkdir -p packages
cd packages

svnco JetTagTools
svnco BTagging

cd ..
