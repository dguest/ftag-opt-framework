#include "UnclusteredVertexBranches.hh"
#include "UnclusteredVertexBranchBuffer.hh"
#include "DummyValues.hh"

#include "xAODJet/Jet.h"
#include "TTree.h"

#include <string>

namespace {
  void add_jetfitter(const std::vector<const xAOD::Jet*>& subjets,
                     UnclusteredVertexBranchBuffer&);
}

UnclusteredVertexBranches::UnclusteredVertexBranches():
  m_branches(new UnclusteredVertexBranchBuffer)
{
  m_branches->jf_vx_n = new std::vector<std::vector<int>>;
  m_branches->jf_vx_displacement = new
    std::vector<std::vector<std::vector<float>>>;
}

UnclusteredVertexBranches::~UnclusteredVertexBranches()
{
  delete m_branches->jf_vx_displacement;
  delete m_branches->jf_vx_n;

  delete m_branches;
}

void UnclusteredVertexBranches::set_tree(TTree& output_tree,
                                         const std::string& prefix) const {
#define ADD_SIMPLE(nm) \
  output_tree.Branch((prefix + #nm).c_str(), &m_branches->nm)

  // jetfitter
  ADD_SIMPLE(jf_vx_n);
  ADD_SIMPLE(jf_vx_displacement);

#undef ADD_SIMPLE
}

void UnclusteredVertexBranches::fill(const std::vector<const xAOD::Jet*>& subjets) {
  add_jetfitter(subjets, *m_branches);
}

void UnclusteredVertexBranches::clear() {
#define CLEAR(var) m_branches->var->clear()
  // jetfitter
  CLEAR(jf_vx_displacement);

#undef CLEAR
}


// ________________________________________________________________________
// filler functions

namespace {
  typedef std::vector<ElementLink<xAOD::BTagVertexContainer> > BTagVertices;
  void add_jetfitter(const std::vector<const xAOD::Jet*>& subjets,
                     UnclusteredVertexBranchBuffer& buffer) {

    std::vector<std::vector<float>> jf_vx_displacement;
    std::vector<int> jf_vx_n;
    for (const auto* jet: subjets) {
      const xAOD::BTagging* bjet = jet->btagging();
      // get vertices
      const BTagVertices vertices;
      // const BTagVertices& vertices = bjet->auxdata<BTagVertices>(
      //   "JetFitterDG_JFvertices");
      jf_vx_n.push_back(vertices.size());
      jf_vx_displacement.push_back(std::vector<float>());
      for (const auto& vx: vertices) {
        jf_vx_displacement.back().push_back(1);
      }
    }
    // fill for this fat jet
#define PUSH(var) buffer.var->push_back(std::move(var))
    PUSH(jf_vx_n);
    PUSH(jf_vx_displacement);
#undef PUSH

  }


}
