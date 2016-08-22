#ifndef BHADRON_BRANCH_BUFFER_HH
#define BHADRON_BRANCH_BUFFER_HH

#include <vector>

struct BHadronBranchBuffer {
  std::vector<int> *nBHadr;
  std::vector<int> *nCHadr;
  
  std::vector<std::vector< int> > *bH_pdgId; 
  std::vector<std::vector< float> > *bH_pt;
  std::vector<std::vector< float> > *bH_eta;
  std::vector<std::vector< float> > *bH_phi;
  std::vector<std::vector< float> > *bH_E;
  std::vector<std::vector< float> > *bH_Lxy;
  std::vector<std::vector< float> > *bH_x;
  std::vector<std::vector< float> > *bH_y;
  std::vector<std::vector< float> > *bH_z;
  std::vector<std::vector< float> > *bH_dRjet;      // distance to jetAxis
  std::vector<std::vector< float> > *bH_PtTrk;      // pt made out of associated charged TrkParticle (both tracks from B and C) 
  std::vector<std::vector< float> > *bH_MTrk;       // mass made out of associated charged particles (both tracks from B and C)
  std::vector<std::vector< int> > *bH_nBtracks;     // number of tracks from B-hadron decay
  std::vector<std::vector< int> > *bH_nCtracks;     // number of tracks from c-hadron decays
  std::vector<std::vector< int> > *bH_nBtracks_400; // same but with 400MeV and eta cut
  std::vector<std::vector< int> > *bH_nCtracks_400; // same but with 400MeV and eta cut

  std::vector<std::vector< int> > *cH_pdgId; 
  std::vector<std::vector< float> > *cH_pt;
  std::vector<std::vector< float> > *cH_eta;
  std::vector<std::vector< float> > *cH_phi;
  std::vector<std::vector< float> > *cH_E;
  std::vector<std::vector< float> > *cH_Lxy;
  std::vector<std::vector< float> > *cH_x;
  std::vector<std::vector< float> > *cH_y;
  std::vector<std::vector< float> > *cH_z;
  std::vector<std::vector< float> > *cH_dRjet;      // distance to jetAxis
  std::vector<std::vector< float> > *cH_PtTrk;      // pt made out of associated charged TrkParticle
  std::vector<std::vector< float> > *cH_MTrk;       // mass made out of associated charged particles
  std::vector<std::vector< int> > *cH_nCtracks;     // number of tracks from c-hadron decays
  std::vector<std::vector< int> > *cH_nCtracks_400; // same but with 400MeV and eta cut
};

#endif // BHADRON_BRANCH_BUFFER_HH
