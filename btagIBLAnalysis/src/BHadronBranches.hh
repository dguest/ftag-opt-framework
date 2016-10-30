#ifndef BHADRON_BRANCHES_HH
#define BHADRON_BRANCHES_HH

#ifndef __MAKECINT__
#include "xAODTracking/TrackParticle.h"
#include "xAODTruth/TruthParticle.h"
#endif // not __MAKECINT__


class TTree;

enum TRKORIGIN{ PUFAKE=-1,
    FROMB,
    FROMC,
    FRAG,
    GEANT };

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
}

// branch buffers are stored as an external class to cut down on
// (re)compile time // VD I strongly diagree with it ;-)
struct BHadronBranchBuffer;

class BHadronBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  BHadronBranches();
  ~BHadronBranches();

  // disable copying and assignment
  BHadronBranches& operator=(BHadronBranches) = delete;
  BHadronBranches(const BHadronBranches&) = delete;

  void set_tree(TTree& output_tree, bool extra_info) const;
  void fill(const xAOD::Jet& jet, std::string jetCollectionName);
  void clear();
private:

  BHadronBranchBuffer* m_branches;

  float deltaR(float eta1, float eta2, float phi1, float phi2);

  const xAOD::Jet* GetParentJet(const xAOD::Jet* Jet, std::string Keyname);

  void GetAllChildren(const xAOD::TruthParticle* particle,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromB,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromC,
                                           bool isFromC);

  std::vector<int> getDRSortedIndices(std::vector<const xAOD::IParticle*> ghostHads, const xAOD::Jet *jet);

  int getTrackOrigin(const xAOD::TrackParticle *tmpTrk,
                      std::vector<const xAOD::TruthParticle*> tracksFromB,
                      std::vector<const xAOD::TruthParticle*> tracksFromC,
                      std::vector<const xAOD::TruthParticle*> tracksFromCc,
                      std::vector<const xAOD::TruthParticle*> tracksFromB1FromParent,
                      std::vector<const xAOD::TruthParticle*> tracksFromB2FromParent,
                      std::vector<const xAOD::TruthParticle*> tracksFromC1FromParent,
                      std::vector<const xAOD::TruthParticle*> tracksFromC2FromParent,
                      std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB1FromParent,
                      std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB2FromParent);

};

#endif // BHADRON_BRANCHES_HH
