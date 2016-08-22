#ifndef BHADRON_BRANCH_BUFFER_HH
#define BHADRON_BRANCH_BUFFER_HH

#include <vector>

struct BHadronBranchBuffer {
  std::vector<int> *v_jet_nBHadr;
  std::vector<int> *v_jet_nCHadr;
  
  std::vector<std::vector< int> > *v_bH_pdgId; 
  std::vector<std::vector< float> > *v_bH_pt;
  std::vector<std::vector< float> > *v_bH_eta;
  std::vector<std::vector< float> > *v_bH_phi;
  std::vector<std::vector< float> > *v_bH_E;
  std::vector<std::vector< float> > *v_bH_Lxy;
  std::vector<std::vector< float> > *v_bH_x;
  std::vector<std::vector< float> > *v_bH_y;
  std::vector<std::vector< float> > *v_bH_z;
  std::vector<std::vector< float> > *v_bH_dRjet;      // distance to jetAxis
  std::vector<std::vector< float> > *v_bH_PtTrk;      // pt made out of associated charged TrkParticle (both tracks from B and C) 
  std::vector<std::vector< float> > *v_bH_MTrk;       // mass made out of associated charged particles (both tracks from B and C)
  std::vector<std::vector< int> > *v_bH_nBtracks;     // number of tracks from B-hadron decay
  std::vector<std::vector< int> > *v_bH_nCtracks;     // number of tracks from c-hadron decays
  std::vector<std::vector< int> > *v_bH_nBtracks_400; // same but with 400MeV and eta cut
  std::vector<std::vector< int> > *v_bH_nCtracks_400; // same but with 400MeV and eta cut

  std::vector<std::vector< int> > *v_cH_pdgId; 
  std::vector<std::vector< float> > *v_cH_pt;
  std::vector<std::vector< float> > *v_cH_eta;
  std::vector<std::vector< float> > *v_cH_phi;
  std::vector<std::vector< float> > *v_cH_E;
  std::vector<std::vector< float> > *v_cH_Lxy;
  std::vector<std::vector< float> > *v_cH_x;
  std::vector<std::vector< float> > *v_cH_y;
  std::vector<std::vector< float> > *v_cH_z;
  std::vector<std::vector< float> > *v_cH_dRjet;      // distance to jetAxis
  std::vector<std::vector< float> > *v_cH_PtTrk;      // pt made out of associated charged TrkParticle
  std::vector<std::vector< float> > *v_cH_MTrk;       // mass made out of associated charged particles
  std::vector<std::vector< int> > *v_cH_nCtracks;     // number of tracks from c-hadron decays
  std::vector<std::vector< int> > *v_cH_nCtracks_400; // same but with 400MeV and eta cut
};

#endif // BHADRON_BRANCH_BUFFER_HH
