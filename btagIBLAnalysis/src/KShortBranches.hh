#ifndef KSHORT_BRANCHES_HH
#define KSHORT_BRANCHES_HH

#include "xAODJet/Jet.h"
#include "xAODTruth/TruthEvent.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthEventAuxContainer.h"

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
struct KShortBranchBuffer;

class KShortBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  KShortBranches();
  ~KShortBranches();

  // disable copying and assignment
  KShortBranches& operator=(KShortBranches) = delete;
  KShortBranches(const KShortBranches&) = delete;

  void set_tree(TTree& output_tree) const;
  //void fill(const xAOD::Jet& jet,const xAOD::TruthEvent& truth); // how many info do I need here?
  void fill(const xAOD::Jet& jet,const xAOD::TruthEventContainer& xTruthEventContainer,double PV_x, double PV_y, double PV_z); // how many info do I need here?
  void clear();
private:

  KShortBranchBuffer* m_branches;
  bool isMatchedKShort( const xAOD::TruthParticle* particle, const xAOD::Jet& jet );
  int  kShortOrigPdgid( const xAOD::TruthParticle* particle );
  std::vector<float> getIPs( double px, double py, double pz, double decayVtx_x, double decayVtx_y, double decayVtx_z, double PV_x, double PV_y, double PV_z );

};

#endif // KSHORT_BRANCHES_HH
