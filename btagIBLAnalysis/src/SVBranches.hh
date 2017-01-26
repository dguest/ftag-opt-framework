#ifndef SV_BRANCHES_HH
#define SV_BRANCHES_HH

#include <string>

class TTree;

namespace xAOD {
  class BTagging_v1;
  typedef BTagging_v1 BTagging;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time // VD I strongly diagree with it ;-)
struct SVBranchBuffer;
struct SVAccessors;

class SVBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  SVBranches(const std::string& prefix);
  ~SVBranches();

  // disable copying and assignment
  SVBranches& operator=(SVBranches) = delete;
  SVBranches(const SVBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const xAOD::BTagging& jet);
  void clear();
private:
  SVAccessors* m_accessors;
  SVBranchBuffer* m_branches;
};

#endif // SV_BRANCHES_HH
