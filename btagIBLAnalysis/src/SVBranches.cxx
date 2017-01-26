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
  typedef std::vector<std::vector<float> > vvf_t;
  typedef std::vector<int> vi_t;

  m_branches->NVtx = new vi_t;
  // m_branches->NGTinJet = new vi_t;
  m_branches->NGTinSvx = new vi_t;
  m_branches->N2Tpair = new vi_t;
  m_branches->masssvx = new vf_t;
  m_branches->efracsvx = new vf_t;
  m_branches->normdist = new vf_t;
  m_branches->significance3d = new vf_t;

  m_branches->vtxx = new vvf_t;
  m_branches->vtxy = new vvf_t;
  m_branches->vtxz = new vvf_t;
}
SVBranches::~SVBranches() {
  // delete all the vectors here ...
  // delete m_branches->NGTinJet;
  delete m_branches->NGTinSvx;
  delete m_branches->N2Tpair;
  delete m_branches->masssvx;
  delete m_branches->efracsvx;
  delete m_branches->normdist;
  delete m_branches->significance3d;

  delete m_branches->vtxx;
  delete m_branches->vtxy;
  delete m_branches->vtxz;

  delete m_accessors;
  delete m_branches;
}

void SVBranches::set_tree(TTree& tree, const std::string& pfx) const {
  tree.Branch((pfx + "Nvtx").c_str(), &m_branches->NVtx);
  // tree.Branch((pfx + "ntrkj").c_str(), &m_branches->NGTinJet);
  tree.Branch((pfx + "ntrkv").c_str(), &m_branches->NGTinSvx);
  tree.Branch((pfx + "n2t").c_str(), &m_branches->N2Tpair);
  tree.Branch((pfx + "m").c_str(), &m_branches->masssvx);
  tree.Branch((pfx + "efc").c_str(), &m_branches->efracsvx);
  tree.Branch((pfx + "sig3d").c_str(), &m_branches->significance3d);
#define BRANCH(name) tree.Branch((pfx + "_" #name).c_str(), &m_branches->name)
  BRANCH(normdist);
#undef BRANCH
  tree.Branch((pfx + "vtx_x").c_str(), &m_branches->vtxx);
  tree.Branch((pfx + "vtx_y").c_str(), &m_branches->vtxy);
  tree.Branch((pfx + "vtx_z").c_str(), &m_branches->vtxz);
}


void SVBranches::fill(const xAOD::BTagging& btag) {
  const auto vertices = m_accessors->vertices(btag);
  m_branches->NVtx->push_back(vertices.size());
  if (vertices.size() > 0) {
#define FILL(name) m_branches->name->push_back(m_accessors->name(btag))
    // FILL(NGTinJet);
    FILL(NGTinSvx);
    FILL(N2Tpair);
    FILL(masssvx);
    FILL(efracsvx);
    FILL(normdist);
    FILL(significance3d);
#undef FILL
  } else {
    // this is supposed to mimic what we do in MV2
    // m_branches->NGTinJet->push_back(-1);
    m_branches->NGTinSvx->push_back(-1);
    m_branches->N2Tpair->push_back(-1);
    m_branches->masssvx->push_back(-99);
    m_branches->normdist->push_back(-99);
    m_branches->significance3d->push_back(-99);
  }

  m_branches->vtxx->emplace_back();
  m_branches->vtxy->emplace_back();
  m_branches->vtxz->emplace_back();
  for (const auto& vtx_link: vertices) {
    if (! vtx_link.isValid() ) {
      // NOTE: should probably print warning or something
      continue;
    }
    const xAOD::Vertex* vertex = *vtx_link;
    m_branches->vtxx->back().push_back(vertex->x());
    m_branches->vtxy->back().push_back(vertex->y());
    m_branches->vtxz->back().push_back(vertex->z());
  }
}

void SVBranches::clear() {
  // clear vectors

  m_branches->NVtx->clear();
  // m_branches->NGTinJet->clear();
  m_branches->NGTinSvx->clear();
  m_branches->N2Tpair->clear();
  m_branches->masssvx->clear();
  m_branches->efracsvx->clear();
  m_branches->normdist->clear();
  m_branches->significance3d->clear();

  m_branches->vtxx->clear();
  m_branches->vtxy->clear();
  m_branches->vtxz->clear();

}
