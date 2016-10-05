#ifndef SSVF_BRANCHES_HH
#define SSVF_BRANCHES_HH

class TTree;

namespace xAOD {
  class BTagging_v1;
  typedef BTagging_v1 BTagging;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct SSVFBranchBuffer;

class SSVFBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  SSVFBranches(bool isSV1=true);
  ~SSVFBranches();

  // disable copying and assignment
  SSVFBranches& operator=(SSVFBranches) = delete;
  SSVFBranches(const SSVFBranches&) = delete;

  void set_tree(TTree& output_tree) const;
  void fill(const xAOD::BTagging& jet);
  void clear();
private:
  SSVFBranchBuffer* m_branches;
  bool m_isSV1;
};

#endif // SSVF_BRANCHES_HH
