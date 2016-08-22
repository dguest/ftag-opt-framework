#include "BHadronBranches.hh"
#include "BHadronBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include <string>
#include <stdexcept>

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::BHadronBranches():
  m_branches(new BHadronBranchBuffer)
{
  // instantiate all the vectors here ...
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::~BHadronBranches() {
  // delete all the vectors here ...
  //delete m_branches->FIRST_ENG_DENS;
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
  std::string prefix = "jet_cluster_";
#define ADD_SIMPLE(nm)							\
  output_tree.Branch(brnm(prefix, #nm).c_str(), &m_branches->nm)
  // basic kinematics
  //////////ADD_SIMPLE(pt);
#undef ADD_SIMPLE
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::fill() {
  //std::vector<float> pt;
  //m_branches->pt->push_back(std::move(pt));
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::clear() {
  // clear vectors
  //m_branches->pt->clear();
}
