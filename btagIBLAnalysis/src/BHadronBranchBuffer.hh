#ifndef BHADRON_BRANCH_BUFFER_HH
#define BHADRON_BRANCH_BUFFER_HH

#include <vector>

struct BHadronBranchBuffer {

  std::vector<std::vector<int> > *v_jet_trk_orig;


  std::vector<int> *nBHadr;
  std::vector<int> *nCHadr;

  std::vector<std::vector< int> >   *bH_pdgId;
  std::vector<std::vector< int> >   *bH_parent_pdgId;
  std::vector<std::vector< float> > *bH_pt;
  std::vector<std::vector< float> > *bH_eta;
  std::vector<std::vector< float> > *bH_phi;
  std::vector<std::vector< float> > *bH_E;
  std::vector<std::vector< float> > *bH_charge;
  std::vector<std::vector< float> > *bH_Lxy;
  std::vector<std::vector< float> > *bH_x;
  std::vector<std::vector< float> > *bH_y;
  std::vector<std::vector< float> > *bH_z;
  std::vector<std::vector< float> > *bH_prod_x;
  std::vector<std::vector< float> > *bH_prod_y;
  std::vector<std::vector< float> > *bH_prod_z;
  std::vector<std::vector< float> > *bH_dRjet;          // distance to jetAxis
  std::vector<std::vector< float> > *bH_PtTrk;          // pt made out of associated charged TrkParticle (both tracks from B and C)
  std::vector<std::vector< float> > *bH_MTrk;           // mass made out of associated charged particles (both tracks from B and C)
  std::vector<std::vector< int> >   *bH_nBtracks;       // number of tracks from B-hadron decay
  std::vector<std::vector< int> >   *bH_nCtracks;       // number of tracks from c-hadron decays
  std::vector<std::vector< int> >   *bH_nBtracks_400;   // same but with pt > 400MeV and eta < 2.5 cut
  std::vector<std::vector< int> >   *bH_nCtracks_400;   // same but with pt > 400MeV and eta < 2.5 cut

  std::vector<std::vector<int> >    *bH_child_hadron_idx;
  std::vector<std::vector<int> >    *bH_child_pdg_id;
  std::vector<std::vector<int> >    *bH_child_parent_pdg_id;
  std::vector<std::vector<int> >    *bH_child_barcode;
  std::vector<std::vector<float> >  *bH_child_charge;
  std::vector<std::vector<float> >  *bH_child_px;
  std::vector<std::vector<float> >  *bH_child_py;
  std::vector<std::vector<float> >  *bH_child_pz;
  std::vector<std::vector<float> >  *bH_child_E;
  std::vector<std::vector<float> >  *bH_child_prod_x;
  std::vector<std::vector<float> >  *bH_child_prod_y;
  std::vector<std::vector<float> >  *bH_child_prod_z;
  std::vector<std::vector<float> >  *bH_child_decay_x;
  std::vector<std::vector<float> >  *bH_child_decay_y;
  std::vector<std::vector<float> >  *bH_child_decay_z;


  std::vector<std::vector< int> > *cH_pdgId;
  std::vector<std::vector< int> > *cH_parent_pdgId;
  std::vector<std::vector< float> > *cH_pt;
  std::vector<std::vector< float> > *cH_eta;
  std::vector<std::vector< float> > *cH_phi;
  std::vector<std::vector< float> > *cH_E;
  std::vector<std::vector< float> > *cH_charge;
  std::vector<std::vector< float> > *cH_Lxy;
  std::vector<std::vector< float> > *cH_x;
  std::vector<std::vector< float> > *cH_y;
  std::vector<std::vector< float> > *cH_z;
  std::vector<std::vector< float> > *cH_prod_x;
  std::vector<std::vector< float> > *cH_prod_y;
  std::vector<std::vector< float> > *cH_prod_z;
  std::vector<std::vector< float> > *cH_dRjet;      // distance to jetAxis
  std::vector<std::vector< float> > *cH_PtTrk;      // pt made out of associated charged TrkParticle
  std::vector<std::vector< float> > *cH_MTrk;       // mass made out of associated charged particles
  std::vector<std::vector< int> > *cH_nCtracks;     // number of tracks from c-hadron decays
  std::vector<std::vector< int> > *cH_nCtracks_400; // same but with pt > 400MeV and eta < 2.5 cut

  std::vector<std::vector<int> >    *cH_child_hadron_idx;
  std::vector<std::vector<int> >    *cH_child_pdg_id;
  std::vector<std::vector<int> >    *cH_child_parent_pdg_id;
  std::vector<std::vector<int> >    *cH_child_barcode;
  std::vector<std::vector<float> >  *cH_child_charge;
  std::vector<std::vector<float> >  *cH_child_px;
  std::vector<std::vector<float> >  *cH_child_py;
  std::vector<std::vector<float> >  *cH_child_pz;
  std::vector<std::vector<float> >  *cH_child_E;
  std::vector<std::vector<float> >  *cH_child_prod_x;
  std::vector<std::vector<float> >  *cH_child_prod_y;
  std::vector<std::vector<float> >  *cH_child_prod_z;
  std::vector<std::vector<float> >  *cH_child_decay_x;
  std::vector<std::vector<float> >  *cH_child_decay_y;
  std::vector<std::vector<float> >  *cH_child_decay_z;

  // double b-tagging variables

  std::vector<int> *v_jet_nGhostBHadrFromParent;
  std::vector<int> *v_jet_nGhostCHadrFromParent;
  std::vector<int> *v_jet_nGhostCHadrFromParentNotFromB;
  std::vector<int> *v_jet_nGhostTauFromParent;
  std::vector<int> *v_jet_nGhostHBosoFromParent;
  std::vector<int> *v_jet_nGhostBHadr;
  std::vector<int> *v_jet_nGhostCHadr;
  std::vector<int> *v_jet_nGhostCHadrNotFromB;
  std::vector<int> *v_jet_nGhostTau;
  std::vector<int> *v_jet_nGhostHBoso;

  std::vector<float> *v_bH1FromParent_pt;
  std::vector<float> *v_bH1FromParent_eta;
  std::vector<float> *v_bH1FromParent_phi;
  std::vector<float> *v_bH1FromParent_Lxy;
  std::vector<float> *v_bH1FromParent_x;
  std::vector<float> *v_bH1FromParent_y;
  std::vector<float> *v_bH1FromParent_z;
  std::vector<float> *v_bH1FromParent_dRjet;
  std::vector<float> *v_bH2FromParent_pt;
  std::vector<float> *v_bH2FromParent_eta;
  std::vector<float> *v_bH2FromParent_phi;
  std::vector<float> *v_bH2FromParent_Lxy;
  std::vector<float> *v_bH2FromParent_x;
  std::vector<float> *v_bH2FromParent_y;
  std::vector<float> *v_bH2FromParent_z;
  std::vector<float> *v_bH2FromParent_dRjet;
  std::vector<float> *v_bH1_pt;
  std::vector<float> *v_bH1_eta;
  std::vector<float> *v_bH1_phi;
  std::vector<float> *v_bH1_Lxy;
  std::vector<float> *v_bH1_x;
  std::vector<float> *v_bH1_y;
  std::vector<float> *v_bH1_z;
  std::vector<float> *v_bH1_dRjet;
  std::vector<float> *v_bH2_pt;
  std::vector<float> *v_bH2_eta;
  std::vector<float> *v_bH2_phi;
  std::vector<float> *v_bH2_Lxy;
  std::vector<float> *v_bH2_x;
  std::vector<float> *v_bH2_y;
  std::vector<float> *v_bH2_z;
  std::vector<float> *v_bH2_dRjet;
};

#endif // BHADRON_BRANCH_BUFFER_HH
