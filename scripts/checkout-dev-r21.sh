# for some reason this script _has_ to be sourced, something about
# svnco not working otherwise

mkdir -p packages
cd packages

svnco ParticleJetTools-00-03-52
svnco JetTagTools
svnco BTagging

cd ..
