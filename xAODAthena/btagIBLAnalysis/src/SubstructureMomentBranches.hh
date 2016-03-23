#ifndef SUBSTRUCTUREMOMENT_BRANCHES_HH
#define SUBSTRUCTUREMOMENT_BRANCHES_HH

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct SubstructureMomentBranchBuffer;

class SubstructureMomentBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  SubstructureMomentBranches();
  ~SubstructureMomentBranches();

  // disable copying and assignment
  SubstructureMomentBranches& operator=(SubstructureMomentBranches) = delete;
  SubstructureMomentBranches(const SubstructureMomentBranches&) = delete;

  void set_tree(TTree& output_tree) const;
  void fill(const xAOD::Jet& constituents);
  void clear();
private:
  SubstructureMomentBranchBuffer* m_branches;
};

#endif // SUBSTRUCTUREMOMENT_BRANCHES_HH
