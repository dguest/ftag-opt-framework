#ifndef CLUSTER_BRANCHES_HH
#define CLUSTER_BRANCHES_HH

class TTree;

namespace xAOD {
  class JetConstituentVector;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct ClusterBranchBuffer;

class ClusterBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  ClusterBranches();
  ~ClusterBranches();

  // disable copying and assignment
  ClusterBranches& operator=(ClusterBranches) = delete;
  ClusterBranches(const ClusterBranches&) = delete;

  void set_tree(TTree& output_tree) const;
  void fill(const xAOD::JetConstituentVector& constituents);
  void clear();
private:
  ClusterBranchBuffer* m_branches;
};

#endif // CLUSTER_BRANCHES_HH
