#ifndef TRACK_BRANCHES_HH
#define TRACK_BRANCHES_HH

#include "xAODTracking/TrackParticleContainer.h"

class TTree;

namespace InDet {
  class IInDetTrackSelectionTool;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct TrackBranchBuffer;

class TrackBranches
{
public:
  typedef std::vector<ElementLink<xAOD::TrackParticleContainer> > TrkLinks;
  // might want to add a prefix to the constructor for the tree branches
  TrackBranches();
  ~TrackBranches();

  // disable copying and assignment
  TrackBranches& operator=(TrackBranches) = delete;
  TrackBranches(const TrackBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const TrkLinks& constituents,
            const InDet::IInDetTrackSelectionTool& cp_tool);
  void clear();
private:
  TrackBranchBuffer* m_branches;
};

#endif // TRACK_BRANCHES_HH
