#include "BHadronBranches.hh"
#include "BHadronBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include "xAODJet/Jet.h"
#include "xAODTruth/TruthParticle.h"

#include <string>
#include <stdexcept>

using xAOD::IParticle;

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::BHadronBranches():
  m_branches(new BHadronBranchBuffer)
{
  // instantiate all the vectors here ...
  m_branches->bH_pt  = new std::vector<std::vector< float> >();
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::~BHadronBranches() {
  // delete all the vectors here ...
  delete m_branches->bH_pt;
  delete m_branches;
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
namespace {
  // branch name function (lowercase the variable name)
  std::string brnm(const std::string& pfx, std::string in) {
    std::transform(in.begin(), in.end(), in.begin(), ::tolower);
    return pfx + in;
  }
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::set_tree(TTree& output_tree) const {
  std::string prefix = "jet_bH_";
  output_tree.Branch( "jet_bH_pt", &m_branches->bH_pt);
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::fill(const xAOD::Jet& jet) {
  std::vector<const IParticle*> ghostB; 
  const std::string labelB = "ConeExclBHadronsFinal";
  jet.getAssociatedObjects<IParticle>(labelB, ghostB);

  std::vector<float> j_bH_pt;
  //m_branches->pt->push_back(std::move(pt));
  if ( ghostB.size()!=0 ) {

    for ( unsigned int iB=0; iB< ghostB.size(); iB++) {
      const xAOD::TruthParticle * myB=(const xAOD::TruthParticle*)(ghostB.at(iB));
      j_bH_pt .push_back( myB->pt()  );
    }
  }
  m_branches->bH_pt->push_back(j_bH_pt);
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::clear() {
  // clear vectors
  m_branches->bH_pt->clear();
}
