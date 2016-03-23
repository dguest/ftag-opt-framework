#ifndef TRACK_BRANCHES_HH
#define TRACK_BRANCHES_HH

class TTree;

namespace xAOD {
  class TrackParticle_v1;
  typedef TrackParticle_v1 TrackParticle;
}

#include <vector>
#include <string>

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct TrackCovBranchBuffer;

class TrackCovBranches
{
public:
  typedef std::vector<const xAOD::TrackParticle*> Tracks;
  // might want to add a prefix to the constructor for the tree branches
  TrackCovBranches();
  ~TrackCovBranches();

  // disable copying and assignment
  TrackCovBranches& operator=(TrackCovBranches) = delete;
  TrackCovBranches(const TrackCovBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const Tracks& tracks);
  void clear();
private:
  TrackCovBranchBuffer* m_branches;
};

#endif // TRACK_BRANCHES_HH
