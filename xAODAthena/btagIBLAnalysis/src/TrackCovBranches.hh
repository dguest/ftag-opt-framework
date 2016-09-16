#ifndef TRACK_COV_BRANCHES_HH
#define TRACK_COV_BRANCHES_HH

class TTree;

#include "xAODTracking/TrackParticleContainer.h"

#include <vector>
#include <string>

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct TrackCovBranchBuffer;

class TrackCovBranches
{
public:
  typedef ElementLink<xAOD::TrackParticleContainer> TrackLink;
  typedef std::vector<ElementLink<xAOD::TrackParticleContainer> > Tracks;
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

#endif // TRACK_COV_BRANCHES_HH
