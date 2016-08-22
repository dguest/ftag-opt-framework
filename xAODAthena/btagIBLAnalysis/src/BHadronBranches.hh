#ifndef BHADRON_BRANCHES_HH
#define BHADRON_BRANCHES_HH

#include "xAODJet/Jet.h"

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
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

  void set_tree(TTree& output_tree) const;
  void fill(const xAOD::Jet& jet); // how many info do I need here?
  void clear();
private:
  BHadronBranchBuffer* m_branches;
};

#endif // BHADRON_BRANCHES_HH
