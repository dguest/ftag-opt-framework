# for some reason this script _has_ to be sourced, something about
# svnco not working otherwise

mkdir -p packages
cd packages

svnco JetTagTools
svnco BTagging
svn co svn+ssh://svn.cern.ch/reps/atlasoff/InnerDetector/InDetRecTools/InDetSVWithMuonTool/tags/InDetSVWithMuonTool-00-00-02 InDetSVWithMuonTool

cd ..
