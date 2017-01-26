#include "SVBranches.hh"
#include "SVBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include "xAODBTagging/BTagging.h"

#include <string>
#include <stdexcept>

//!-----------------------------------------------------------------------------------------------------------------------------!//
SVBranches::SVBranches(const std::string& prefix):
  m_accessors(new SVAccessors(prefix)),
  m_branches(new SVBranchBuffer)
{
  // instantiate all the vectors here ...
  typedef std::vector<float> vf_t;
  typedef std::vector<int> vi_t;
  m_branches->NVtx = new vi_t;
  m_branches->NGTinJet = new vi_t;
  m_branches->NGTinSvx = new vi_t;
  m_branches->N2Tpair = new vi_t;
  m_branches->masssvx = new vf_t;
  m_branches->efracsvx = new vf_t;
  m_branches->normdist = new vf_t;
  m_branches->significance3D = new vf_t;
}
SVBranches::~SVBranches() {
  // delete all the vectors here ...
  delete m_branches->NGTinJet;
  delete m_branches->NGTinSvx;
  delete m_branches->N2Tpair;
  delete m_branches->masssvx;
  delete m_branches->efracsvx;
  delete m_branches->normdist;
  delete m_branches->significance3D;

  delete m_accessors;
  delete m_branches;
}

void SVBranches::set_tree(TTree& tree, const std::string& pfx) const {
  tree.Branch((pfx + "Nvtx").c_str(), &m_branches->NVtx);
  tree.Branch((pfx + "ntrkj").c_str(), &m_branches->NGTinJet);
  tree.Branch((pfx + "ntrkv").c_str(), &m_branches->NGTinSvx);
  tree.Branch((pfx + "n2t").c_str(), &m_branches->N2Tpair);
  tree.Branch((pfx + "m").c_str(), &m_branches->masssvx);
  tree.Branch((pfx + "efc").c_str(), &m_branches->efracsvx);
  tree.Branch((pfx + "sig3d").c_str(), &m_branches->significance3D);
#define BRANCH(name) tree.Branch((pfx + "_" #name).c_str(), &m_branches->name)
  // BRANCH(NGTinJet);
  // BRANCH(NGTinSvx);
  // BRANCH(N2Tpair);
  // BRANCH(masssvx);
  // BRANCH(efracsvx);
  BRANCH(normdist);
  // BRANCH(significance3D);
#undef BRANCH

}


void SVBranches::fill(const xAOD::BTagging& btag) {
  m_branches->NVtx->push_back(m_accessors->vertices(btag).size());
#define FILL(name) m_branches->name->push_back(m_accessors->name(btag))
  FILL(NGTinJet);
  FILL(NGTinSvx);
  FILL(N2Tpair);
  FILL(masssvx);
  FILL(efracsvx);
  FILL(normdist);
  FILL(significance3D);
#undef FILL
}

void SVBranches::clear() {
  // clear vectors

  m_branches->NVtx->clear();
  m_branches->NGTinJet->clear();
  m_branches->NGTinSvx->clear();
  m_branches->N2Tpair->clear();
  m_branches->masssvx->clear();
  m_branches->efracsvx->clear();
  m_branches->normdist->clear();
  m_branches->significance3D->clear();

}
