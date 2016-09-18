#ifndef TRACK_COV_BRANCHES_HH
#define TRACK_COV_BRANCHES_HH

namespace xAOD {
  class IParticle;
}
class TTree;

#include <vector>
#include <string>

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct TrackCovBranchBuffer;

class TrackCovBranches
{
public:
  typedef std::vector<const xAOD::IParticle*> PartVector;

  // might want to add a prefix to the constructor for the tree branches
  TrackCovBranches();
  ~TrackCovBranches();

  // disable copying and assignment
  TrackCovBranches& operator=(TrackCovBranches) = delete;
  TrackCovBranches(const TrackCovBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const PartVector& tracks);
  void clear();
private:
  TrackCovBranchBuffer* m_branches;
  // short-circuit the branch filling if no tree is set
  mutable bool m_active;
};

#endif // TRACK_COV_BRANCHES_HH
