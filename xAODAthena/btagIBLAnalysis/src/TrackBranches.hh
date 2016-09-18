#ifndef TRACK_BRANCHES_HH
#define TRACK_BRANCHES_HH

namespace xAOD {
  class IParticle;
}
class TTree;
namespace Trk  {
  class ITrackToVertexIPEstimator;
}
namespace xAOD {
  class Vertex_v1;
  typedef Vertex_v1 Vertex;
}

#include <vector>
#include <string>

// branch buffers are stored as an external class to cut down on
// (re)compile time
struct TrackBranchBuffer;

class TrackBranches
{
public:
  typedef std::vector<const xAOD::IParticle*> PartVector;

  TrackBranches();
  ~TrackBranches();

  // disable copying and assignment
  TrackBranches& operator=(TrackBranches) = delete;
  TrackBranches(const TrackBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const PartVector& constituents,
            const xAOD::Vertex& primary_vertex,
            const Trk::ITrackToVertexIPEstimator& trk2vrt);
  void clear();
private:
  TrackBranchBuffer* m_branches;
  // short-circuit the branch filling if no tree is set
  mutable bool m_active;
};

#endif // TRACK_BRANCHES_HH
