#ifndef SUBJET_BRANCHES_HH
#define SUBJET_BRANCHES_HH

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
}

#include <vector>
#include <string>

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct SubjetBranchBuffer;

class SubjetBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  SubjetBranches();
  ~SubjetBranches();

  // disable copying and assignment
  SubjetBranches& operator=(SubjetBranches) = delete;
  SubjetBranches(const SubjetBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const std::vector<const xAOD::Jet*>& subjets);
  void clear();
private:
  SubjetBranchBuffer* m_branches;
};

#endif // SUBJET_BRANCHES_HH
