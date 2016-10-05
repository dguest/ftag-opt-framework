#ifndef UNCLUSTEREDVERTEX_BRANCHES_HH
#define UNCLUSTEREDVERTEX_BRANCHES_HH

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
}

#include <vector>
#include <string>

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct UnclusteredVertexBranchBuffer;

class UnclusteredVertexBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  UnclusteredVertexBranches();
  ~UnclusteredVertexBranches();

  // disable copying and assignment
  UnclusteredVertexBranches& operator=(UnclusteredVertexBranches) = delete;
  UnclusteredVertexBranches(const UnclusteredVertexBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const std::vector<const xAOD::Jet*>& subjets);
  void clear();
private:
  UnclusteredVertexBranchBuffer* m_branches;
};

#endif // UNCLUSTEREDVERTEX_BRANCHES_HH
