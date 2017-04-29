#ifndef JETFITTER_BRANCHES_HH
#define JETFITTER_BRANCHES_HH

#include "xAODJet/Jet.h"

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
  class TruthParticle_v1;
  typedef TruthParticle_v1 TruthParticle;
  class TruthVertex_v1;
  typedef TruthVertex_v1 TruthVertex;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time // VD I strongly diagree with it ;-)
struct JetFitterBranchBuffer;

class JetFitterBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  JetFitterBranches();
  ~JetFitterBranches();

  // disable copying and assignment
  JetFitterBranches& operator=(JetFitterBranches) = delete;
  JetFitterBranches(const JetFitterBranches&) = delete;

  void set_tree(TTree& output_tree) const;
  void fill(const xAOD::Jet& jet);
  void clear();
private:

  JetFitterBranchBuffer* m_branches;
  std::vector<float> JF_xyz_errors(float L3D, float L3Derr, float Theta, float Theta_err, float Phi, float Phi_err,float Pv_x, float Pv_y, float Pv_z);
  float JF_Transverse_error(float L3D, float Theta, float Theta_err, float Phi, float Phi_err);
  bool particleInCollection( const xAOD::TrackParticle *trkPart, std::vector< ElementLink< xAOD::TrackParticleContainer > > trkColl );
};

#endif // JETFITTER_BRANCHES_HH
