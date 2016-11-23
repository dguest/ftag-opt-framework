#ifndef KSHORTRECO_BRANCHES_HH
#define KSHORTRECO_BRANCHES_HH

#include "xAODJet/Jet.h"

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time // VD I strongly diagree with it ;-)
struct KShortRecoBranchBuffer;

class KShortRecoBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  KShortRecoBranches();
  ~KShortRecoBranches();

  // disable copying and assignment
  KShortRecoBranches& operator=(KShortRecoBranches) = delete;
  KShortRecoBranches(const KShortRecoBranches&) = delete;

  void set_tree(TTree& output_tree) const;
  void fill(const xAOD::Jet& jet,const xAOD::VertexContainer& v0s, double PV_x, double PV_y, double PV_z); // how many info do I need here?
  void clear();
private:

  KShortRecoBranchBuffer* m_branches;
  std::vector<float> getIPs( double px, double py, double pz, double decayVtx_x, double decayVtx_y, double decayVtx_z, double PV_x, double PV_y, double PV_z );

};

#endif // KSHORTRECO_BRANCHES_HH
