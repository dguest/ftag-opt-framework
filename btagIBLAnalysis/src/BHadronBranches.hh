#ifndef BHADRON_BRANCHES_HH
#define BHADRON_BRANCHES_HH

#include "xAODJet/Jet.h"

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
  class TruthParticle_v1;
  typedef TruthParticle_v1 TruthParticle;
  class TruthVertex_v1;
  typedef TruthVertex_v1 TruthVertex;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time // VD I strongly diagree with it ;-)
struct BHadronBranchBuffer;

class BHadronBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  BHadronBranches();
  ~BHadronBranches();

  // disable copying and assignment
  BHadronBranches& operator=(BHadronBranches) = delete;
  BHadronBranches(const BHadronBranches&) = delete;

  void set_tree(TTree& output_tree, bool reduce_info) const;
  void fill(const xAOD::Jet& jet);
  void clear();
private:

  BHadronBranchBuffer* m_branches;
  void GetAllChildren(const xAOD::TruthParticle* particle,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromB,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromC,
                                           std::vector<bool> &isFrom_C_tracks,
                                           bool isFromC);
  std::vector<int> getDRSortedIndices(std::vector<const xAOD::IParticle*> ghostHads, const xAOD::Jet *jet);

};

#endif // BHADRON_BRANCHES_HH
