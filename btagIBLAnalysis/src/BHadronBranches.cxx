#include "BHadronBranches.hh"
#include "BHadronBranchBuffer.hh"

#include "xAODJet/Jet.h"
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTruth/TruthEventContainer.h"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include <string>
#include <stdexcept>

using xAOD::IParticle;

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::BHadronBranches():
  m_branches(new BHadronBranchBuffer)
{
  // instantiate all the vectors here ...
  m_branches->v_jet_trk_orig = new std::vector<std::vector<int> >();


  m_branches->nBHadr = new std::vector<int>();
  m_branches->nCHadr = new std::vector<int>();

  m_branches->bH_pdgId        = new std::vector<std::vector< int> >();
  m_branches->bH_parent_pdgId = new std::vector<std::vector< int> >();
  m_branches->bH_pt           = new std::vector<std::vector< float> >();
  m_branches->bH_eta          = new std::vector<std::vector< float> >();
  m_branches->bH_phi          = new std::vector<std::vector< float> >();
  m_branches->bH_E            = new std::vector<std::vector< float> >();
  m_branches->bH_charge       = new std::vector<std::vector< float> >();
  m_branches->bH_Lxy          = new std::vector<std::vector< float> >();
  m_branches->bH_x            = new std::vector<std::vector< float> >();
  m_branches->bH_y            = new std::vector<std::vector< float> >();
  m_branches->bH_z            = new std::vector<std::vector< float> >();
  m_branches->bH_prod_x       = new std::vector<std::vector< float> >();
  m_branches->bH_prod_y       = new std::vector<std::vector< float> >();
  m_branches->bH_prod_z       = new std::vector<std::vector< float> >();
  m_branches->bH_dRjet        = new std::vector<std::vector< float> >();
  m_branches->bH_PtTrk        = new std::vector<std::vector< float> >();
  m_branches->bH_MTrk         = new std::vector<std::vector< float> >();
  m_branches->bH_nBtracks     = new std::vector<std::vector< int> >();
  m_branches->bH_nCtracks     = new std::vector<std::vector< int> >();
  m_branches->bH_nBtracks_400 = new std::vector<std::vector< int> >();
  m_branches->bH_nCtracks_400 = new std::vector<std::vector< int> >();

  m_branches->bH_child_hadron_idx         = new std::vector<std::vector<int> > ();
  m_branches->bH_child_pdg_id             = new std::vector<std::vector<int> >();
  m_branches->bH_child_parent_pdg_id      = new std::vector<std::vector<int> >();
  m_branches->bH_child_barcode            = new std::vector<std::vector<int> >();
  m_branches->bH_child_charge             = new std::vector<std::vector<float> >();
  m_branches->bH_child_px                 = new std::vector<std::vector<float> >();
  m_branches->bH_child_py                 = new std::vector<std::vector<float> >();
  m_branches->bH_child_pz                 = new std::vector<std::vector<float> >();
  m_branches->bH_child_E                  = new std::vector<std::vector<float> >();
  m_branches->bH_child_prod_x             = new std::vector<std::vector<float> >();
  m_branches->bH_child_prod_y             = new std::vector<std::vector<float> >();
  m_branches->bH_child_prod_z             = new std::vector<std::vector<float> >();
  m_branches->bH_child_decay_x            = new std::vector<std::vector<float> >();
  m_branches->bH_child_decay_y            = new std::vector<std::vector<float> >();
  m_branches->bH_child_decay_z            = new std::vector<std::vector<float> >();

  m_branches->cH_pdgId        = new std::vector<std::vector< int> >();
  m_branches->cH_parent_pdgId = new std::vector<std::vector< int> >();
  m_branches->cH_pt           = new std::vector<std::vector< float> >();
  m_branches->cH_eta          = new std::vector<std::vector< float> >();
  m_branches->cH_phi          = new std::vector<std::vector< float> >();
  m_branches->cH_E            = new std::vector<std::vector< float> >();
  m_branches->cH_charge       = new std::vector<std::vector< float> >();
  m_branches->cH_Lxy          = new std::vector<std::vector< float> >();
  m_branches->cH_x            = new std::vector<std::vector< float> >();
  m_branches->cH_y            = new std::vector<std::vector< float> >();
  m_branches->cH_z            = new std::vector<std::vector< float> >();
  m_branches->cH_prod_x       = new std::vector<std::vector< float> >();
  m_branches->cH_prod_y       = new std::vector<std::vector< float> >();
  m_branches->cH_prod_z       = new std::vector<std::vector< float> >();
  m_branches->cH_dRjet        = new std::vector<std::vector< float> >();
  m_branches->cH_PtTrk        = new std::vector<std::vector< float> >();
  m_branches->cH_MTrk         = new std::vector<std::vector< float> >();
  m_branches->cH_nCtracks     = new std::vector<std::vector< int> >();
  m_branches->cH_nCtracks_400 = new std::vector<std::vector< int> >();

  m_branches->cH_child_hadron_idx         = new std::vector<std::vector<int> > ();
  m_branches->cH_child_pdg_id             = new std::vector<std::vector<int> >();
  m_branches->cH_child_parent_pdg_id      = new std::vector<std::vector<int> >();
  m_branches->cH_child_barcode            = new std::vector<std::vector<int> >();
  m_branches->cH_child_charge             = new std::vector<std::vector<float> >();
  m_branches->cH_child_px                 = new std::vector<std::vector<float> >();
  m_branches->cH_child_py                 = new std::vector<std::vector<float> >();
  m_branches->cH_child_pz                 = new std::vector<std::vector<float> >();
  m_branches->cH_child_E                  = new std::vector<std::vector<float> >();
  m_branches->cH_child_prod_x             = new std::vector<std::vector<float> >();
  m_branches->cH_child_prod_y             = new std::vector<std::vector<float> >();
  m_branches->cH_child_prod_z             = new std::vector<std::vector<float> >();
  m_branches->cH_child_decay_x            = new std::vector<std::vector<float> >();
  m_branches->cH_child_decay_y            = new std::vector<std::vector<float> >();
  m_branches->cH_child_decay_z            = new std::vector<std::vector<float> >();

  //double b-tagging variables

  m_branches->v_bH1FromParent_pt    = new std::vector<float>();
  m_branches->v_bH1FromParent_eta   = new std::vector<float>();
  m_branches->v_bH1FromParent_phi   = new std::vector<float>();
  m_branches->v_bH1FromParent_Lxy   = new std::vector<float>();
  m_branches->v_bH1FromParent_dRjet = new std::vector<float>();
  m_branches->v_bH1FromParent_x     = new std::vector<float>();
  m_branches->v_bH1FromParent_y     = new std::vector<float>();
  m_branches->v_bH1FromParent_z     = new std::vector<float>();

  m_branches->v_bH2FromParent_pt    = new std::vector<float>();
  m_branches->v_bH2FromParent_eta   = new std::vector<float>();
  m_branches->v_bH2FromParent_phi   = new std::vector<float>();
  m_branches->v_bH2FromParent_Lxy   = new std::vector<float>();
  m_branches->v_bH2FromParent_dRjet = new std::vector<float>();
  m_branches->v_bH2FromParent_x     = new std::vector<float>();
  m_branches->v_bH2FromParent_y     = new std::vector<float>();
  m_branches->v_bH2FromParent_z     = new std::vector<float>();

  m_branches->v_bH1_pt              = new std::vector<float>();
  m_branches->v_bH1_eta             = new std::vector<float>();
  m_branches->v_bH1_phi             = new std::vector<float>();
  m_branches->v_bH1_Lxy             = new std::vector<float>();
  m_branches->v_bH1_dRjet           = new std::vector<float>();
  m_branches->v_bH1_x               = new std::vector<float>();
  m_branches->v_bH1_y               = new std::vector<float>();
  m_branches->v_bH1_z               = new std::vector<float>();

  m_branches->v_bH2_pt              = new std::vector<float>();
  m_branches->v_bH2_eta             = new std::vector<float>();
  m_branches->v_bH2_phi             = new std::vector<float>();
  m_branches->v_bH2_Lxy             = new std::vector<float>();
  m_branches->v_bH2_dRjet           = new std::vector<float>();
  m_branches->v_bH2_x               = new std::vector<float>();
  m_branches->v_bH2_y               = new std::vector<float>();
  m_branches->v_bH2_z               = new std::vector<float>();

  m_branches->v_jet_nGhostBHadrFromParent         = new std::vector<int>();
  m_branches->v_jet_nGhostCHadrFromParent         = new std::vector<int>();
  m_branches->v_jet_nGhostCHadrFromParentNotFromB = new std::vector<int>();
  m_branches->v_jet_nGhostTauFromParent           = new std::vector<int>();
  m_branches->v_jet_nGhostHBosoFromParent         = new std::vector<int>();
  m_branches->v_jet_nGhostBHadr                   = new std::vector<int>();
  m_branches->v_jet_nGhostCHadr                   = new std::vector<int>();
  m_branches->v_jet_nGhostCHadrNotFromB           = new std::vector<int>();
  m_branches->v_jet_nGhostTau                     = new std::vector<int>();
  m_branches->v_jet_nGhostHBoso                   = new std::vector<int>();
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::~BHadronBranches() {
  // delete all the vectors here ...
  delete m_branches->nBHadr;
  delete m_branches->nCHadr;

  delete m_branches->bH_pdgId;
  delete m_branches->bH_parent_pdgId;
  delete m_branches->bH_pt;
  delete m_branches->bH_eta;
  delete m_branches->bH_phi;
  delete m_branches->bH_E;
  delete m_branches->bH_charge;
  delete m_branches->bH_Lxy;
  delete m_branches->bH_x;
  delete m_branches->bH_y;
  delete m_branches->bH_z;
  delete m_branches->bH_prod_x;
  delete m_branches->bH_prod_y;
  delete m_branches->bH_prod_z;
  delete m_branches->bH_dRjet;
  delete m_branches->bH_PtTrk;
  delete m_branches->bH_MTrk;
  delete m_branches->bH_nBtracks;
  delete m_branches->bH_nCtracks;
  delete m_branches->bH_nBtracks_400;
  delete m_branches->bH_nCtracks_400;
  delete m_branches->bH_child_hadron_idx;
  delete m_branches->bH_child_pdg_id;
  delete m_branches->bH_child_parent_pdg_id;
  delete m_branches->bH_child_barcode;
  delete m_branches->bH_child_charge;
  delete m_branches->bH_child_px;
  delete m_branches->bH_child_py;
  delete m_branches->bH_child_pz;
  delete m_branches->bH_child_E;
  delete m_branches->bH_child_prod_x;
  delete m_branches->bH_child_prod_y;
  delete m_branches->bH_child_prod_z;
  delete m_branches->bH_child_decay_x;
  delete m_branches->bH_child_decay_y;
  delete m_branches->bH_child_decay_z;

  delete m_branches->cH_pdgId;
  delete m_branches->cH_parent_pdgId;
  delete m_branches->cH_pt;
  delete m_branches->cH_eta;
  delete m_branches->cH_phi;
  delete m_branches->cH_E;
  delete m_branches->cH_charge;
  delete m_branches->cH_Lxy;
  delete m_branches->cH_x;
  delete m_branches->cH_y;
  delete m_branches->cH_z;
  delete m_branches->cH_prod_x;
  delete m_branches->cH_prod_y;
  delete m_branches->cH_prod_z;
  delete m_branches->cH_dRjet;
  delete m_branches->cH_PtTrk;
  delete m_branches->cH_MTrk;
  delete m_branches->cH_nCtracks;
  delete m_branches->cH_nCtracks_400;
  delete m_branches->cH_child_hadron_idx;
  delete m_branches->cH_child_pdg_id;
  delete m_branches->cH_child_parent_pdg_id;
  delete m_branches->cH_child_barcode;
  delete m_branches->cH_child_charge;
  delete m_branches->cH_child_px;
  delete m_branches->cH_child_py;
  delete m_branches->cH_child_pz;
  delete m_branches->cH_child_E;
  delete m_branches->cH_child_prod_x;
  delete m_branches->cH_child_prod_y;
  delete m_branches->cH_child_prod_z;
  delete m_branches->cH_child_decay_x;
  delete m_branches->cH_child_decay_y;
  delete m_branches->cH_child_decay_z;

  delete m_branches;
}

void BHadronBranches::set_tree(TTree& output_tree, bool extra_info) const {
  //std::string prefix = "jet_bH_";



  output_tree.Branch( "jet_nBHadr"       , &m_branches->nBHadr);
  output_tree.Branch( "jet_nCHadr"       , &m_branches->nCHadr);

  output_tree.Branch( "jet_bH_pdgId"       , &m_branches->bH_pdgId);
  output_tree.Branch( "jet_bH_parent_pdgId", &m_branches->bH_parent_pdgId);
  output_tree.Branch( "jet_bH_pt"          , &m_branches->bH_pt);
  output_tree.Branch( "jet_bH_eta"         , &m_branches->bH_eta);
  output_tree.Branch( "jet_bH_phi"         , &m_branches->bH_phi);
  output_tree.Branch( "jet_bH_E"           , &m_branches->bH_E);
  output_tree.Branch( "jet_bH_charge"      , &m_branches->bH_charge);
  output_tree.Branch( "jet_bH_Lxy"         , &m_branches->bH_Lxy);
  output_tree.Branch( "jet_bH_x"           , &m_branches->bH_x);
  output_tree.Branch( "jet_bH_y"           , &m_branches->bH_y);
  output_tree.Branch( "jet_bH_z"           , &m_branches->bH_z);
  output_tree.Branch( "jet_bH_dRjet"       , &m_branches->bH_dRjet);

  output_tree.Branch( "jet_cH_pdgId"       , &m_branches->cH_pdgId);
  output_tree.Branch( "jet_cH_parent_pdgId",&m_branches->cH_parent_pdgId);
  output_tree.Branch( "jet_cH_pt"          , &m_branches->cH_pt);
  output_tree.Branch( "jet_cH_eta"         , &m_branches->cH_eta);
  output_tree.Branch( "jet_cH_phi"         , &m_branches->cH_phi);
  output_tree.Branch( "jet_cH_E"           , &m_branches->cH_E);
  output_tree.Branch( "jet_cH_charge"      , &m_branches->cH_charge);
  output_tree.Branch( "jet_cH_Lxy"         , &m_branches->cH_Lxy);
  output_tree.Branch( "jet_cH_x"           , &m_branches->cH_x);
  output_tree.Branch( "jet_cH_y"           , &m_branches->cH_y);
  output_tree.Branch( "jet_cH_z"           , &m_branches->cH_z);
  output_tree.Branch( "jet_cH_dRjet"       , &m_branches->cH_dRjet);

  if(extra_info){
      output_tree.Branch("jet_trk_orig", &m_branches->v_jet_trk_orig);

      output_tree.Branch( "jet_bH_prod_x"      , &m_branches->bH_prod_x);
      output_tree.Branch( "jet_bH_prod_y"      , &m_branches->bH_prod_y);
      output_tree.Branch( "jet_bH_prod_z"      , &m_branches->bH_prod_z);
      output_tree.Branch( "jet_bH_PtTrk"       , &m_branches->bH_PtTrk);
      output_tree.Branch( "jet_bH_MTrk"        , &m_branches->bH_MTrk);
      output_tree.Branch( "jet_bH_nBtracks"    , &m_branches->bH_nBtracks);
      output_tree.Branch( "jet_bH_nCtracks"    , &m_branches->bH_nCtracks);
      output_tree.Branch( "jet_bH_nBtracks_400", &m_branches->bH_nBtracks_400);
      output_tree.Branch( "jet_bH_nCtracks_400", &m_branches->bH_nCtracks_400);

      output_tree.Branch( "jet_bH_child_hadron_idx"   , &m_branches->bH_child_hadron_idx);
      output_tree.Branch( "jet_bH_child_pdg_id"       , &m_branches->bH_child_pdg_id);
      output_tree.Branch( "jet_bH_child_parent_pdg_id", &m_branches->bH_child_parent_pdg_id);
      output_tree.Branch( "jet_bH_child_barcode"      , &m_branches->bH_child_barcode);
      output_tree.Branch( "jet_bH_child_charge"       , &m_branches->bH_child_charge);
      output_tree.Branch( "jet_bH_child_px"           , &m_branches->bH_child_px);
      output_tree.Branch( "jet_bH_child_py"           , &m_branches->bH_child_py);
      output_tree.Branch( "jet_bH_child_pz"           , &m_branches->bH_child_pz);
      output_tree.Branch( "jet_bH_child_E"            , &m_branches->bH_child_E);
      output_tree.Branch( "jet_bH_child_prod_x"       , &m_branches->bH_child_prod_x);
      output_tree.Branch( "jet_bH_child_prod_y"       , &m_branches->bH_child_prod_y);
      output_tree.Branch( "jet_bH_child_prod_z"       , &m_branches->bH_child_prod_z);
      output_tree.Branch( "jet_bH_child_decay_x"      , &m_branches->bH_child_decay_x);
      output_tree.Branch( "jet_bH_child_decay_y"      , &m_branches->bH_child_decay_y);
      output_tree.Branch( "jet_bH_child_decay_z"      , &m_branches->bH_child_decay_z);

      output_tree.Branch( "jet_cH_prod_x"      , &m_branches->cH_prod_x);
      output_tree.Branch( "jet_cH_prod_y"      , &m_branches->cH_prod_y);
      output_tree.Branch( "jet_cH_prod_z"      , &m_branches->cH_prod_z);
      output_tree.Branch( "jet_cH_PtTrk"       , &m_branches->cH_PtTrk);
      output_tree.Branch( "jet_cH_MTrk"        , &m_branches->cH_MTrk);
      output_tree.Branch( "jet_cH_nCtracks"    , &m_branches->cH_nCtracks);
      output_tree.Branch( "jet_cH_nCtracks_400", &m_branches->cH_nCtracks_400);


      output_tree.Branch( "jet_cH_child_hadron_idx"   , &m_branches->cH_child_hadron_idx);
      output_tree.Branch( "jet_cH_child_pdg_id"       , &m_branches->cH_child_pdg_id);
      output_tree.Branch( "jet_cH_child_parent_pdg_id", &m_branches->cH_child_parent_pdg_id);
      output_tree.Branch( "jet_cH_child_barcode"      , &m_branches->cH_child_barcode);
      output_tree.Branch( "jet_cH_child_charge"       , &m_branches->cH_child_charge);
      output_tree.Branch( "jet_cH_child_px"           , &m_branches->cH_child_px);
      output_tree.Branch( "jet_cH_child_py"           , &m_branches->cH_child_py);
      output_tree.Branch( "jet_cH_child_pz"           , &m_branches->cH_child_pz);
      output_tree.Branch( "jet_cH_child_E"            , &m_branches->cH_child_E);
      output_tree.Branch( "jet_cH_child_prod_x"       , &m_branches->cH_child_prod_x);
      output_tree.Branch( "jet_cH_child_prod_y"       , &m_branches->cH_child_prod_y);
      output_tree.Branch( "jet_cH_child_prod_z"       , &m_branches->cH_child_prod_z);
      output_tree.Branch( "jet_cH_child_decay_x"      , &m_branches->cH_child_decay_x);
      output_tree.Branch( "jet_cH_child_decay_y"      , &m_branches->cH_child_decay_y);
      output_tree.Branch( "jet_cH_child_decay_z"      , &m_branches->cH_child_decay_z);


      // double b-tagging variables
      output_tree.Branch("jet_nGhostBHadrFromParent"        , &m_branches->v_jet_nGhostBHadrFromParent); // mod nikola
      output_tree.Branch("jet_nGhostCHadrFromParent"        , &m_branches->v_jet_nGhostCHadrFromParent); // mod nikola
      output_tree.Branch("jet_nGhostCHadrFromParentNotFromB", &m_branches->v_jet_nGhostCHadrFromParentNotFromB); // mod nikola
      output_tree.Branch("jet_nGhostTauFromParent"          , &m_branches->v_jet_nGhostTauFromParent); // mod nikola
      output_tree.Branch("jet_nGhostHBosoFromParent"        , &m_branches->v_jet_nGhostHBosoFromParent); // mod nikola
      output_tree.Branch("jet_nGhostBHadr"                  , &m_branches->v_jet_nGhostBHadr); // mod nikola
      output_tree.Branch("jet_nGhostCHadr"                  , &m_branches->v_jet_nGhostCHadr); // mod nikola
      output_tree.Branch("jet_nGhostCHadrNotFromB"          , &m_branches->v_jet_nGhostCHadrNotFromB); // mod nikola
      output_tree.Branch("jet_nGhostTau"                    , &m_branches->v_jet_nGhostTau); // mod nikola
      output_tree.Branch("jet_nGhostHBoso"                  , &m_branches->v_jet_nGhostHBoso); // mod nikola

      output_tree.Branch("bH1FromParent_pt", &m_branches->v_bH1FromParent_pt);
      output_tree.Branch("bH1FromParent_eta", &m_branches->v_bH1FromParent_eta);
      output_tree.Branch("bH1FromParent_phi", &m_branches->v_bH1FromParent_phi);
      output_tree.Branch("bH1FromParent_Lxy", &m_branches->v_bH1FromParent_Lxy);
      output_tree.Branch("bH1FromParent_x", &m_branches->v_bH1FromParent_x);
      output_tree.Branch("bH1FromParent_y", &m_branches->v_bH1FromParent_y);
      output_tree.Branch("bH1FromParent_z", &m_branches->v_bH1FromParent_z);
      output_tree.Branch("bH1FromParent_dRjet", &m_branches->v_bH1FromParent_dRjet);

      output_tree.Branch("bH2FromParent_pt", &m_branches->v_bH2FromParent_pt);
      output_tree.Branch("bH2FromParent_eta", &m_branches->v_bH2FromParent_eta);
      output_tree.Branch("bH2FromParent_phi", &m_branches->v_bH2FromParent_phi);
      output_tree.Branch("bH2FromParent_Lxy", &m_branches->v_bH2FromParent_Lxy);
      output_tree.Branch("bH2FromParent_x", &m_branches->v_bH2FromParent_x);
      output_tree.Branch("bH2FromParent_y", &m_branches->v_bH2FromParent_y);
      output_tree.Branch("bH2FromParent_z", &m_branches->v_bH2FromParent_z);
      output_tree.Branch("bH2FromParent_dRjet", &m_branches->v_bH2FromParent_dRjet);

      output_tree.Branch("bH1_pt", &m_branches->v_bH1_pt);
      output_tree.Branch("bH1_eta", &m_branches->v_bH1_eta);
      output_tree.Branch("bH1_phi", &m_branches->v_bH1_phi);
      output_tree.Branch("bH1_Lxy", &m_branches->v_bH1_Lxy);
      output_tree.Branch("bH1_x", &m_branches->v_bH1_x);
      output_tree.Branch("bH1_y", &m_branches->v_bH1_y);
      output_tree.Branch("bH1_z", &m_branches->v_bH1_z);
      output_tree.Branch("bH1_dRjet", &m_branches->v_bH1_dRjet);

      output_tree.Branch("bH2_pt", &m_branches->v_bH2_pt);
      output_tree.Branch("bH2_eta", &m_branches->v_bH2_eta);
      output_tree.Branch("bH2_phi", &m_branches->v_bH2_phi);
      output_tree.Branch("bH2_Lxy", &m_branches->v_bH2_Lxy);
      output_tree.Branch("bH2_x", &m_branches->v_bH2_x);
      output_tree.Branch("bH2_y", &m_branches->v_bH2_y);
      output_tree.Branch("bH2_z", &m_branches->v_bH2_z);
      output_tree.Branch("bH2_dRjet", &m_branches->v_bH2_dRjet);

  }

}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::fill(const xAOD::Jet& jet, std::string jetCollectionName) {

  bool double_btagging = (strcmp(jetCollectionName.c_str(), "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets") == 0 || strcmp(jetCollectionName.c_str(), "Akt10LCTopoTrmJets") == 0);

  const xAOD::Jet *jet_parent = 0;
    if (double_btagging) {
      jet_parent = GetParentJet(&jet, "Parent");
    }


  //regular b-tagging :
  std::vector<const IParticle*> ghostB;
  const std::string labelB = "ConeExclBHadronsFinal";
  jet.getAssociatedObjects<IParticle>(labelB, ghostB);

  std::vector<const IParticle*> ghostC;
  const std::string labelC = "ConeExclCHadronsFinal";
  jet.getAssociatedObjects<IParticle>(labelC, ghostC);

  // collection of tracks from B and C hadrons with minimal dR to jet, to be used with the track origin variable
  std::vector<const xAOD::TruthParticle*> first_BtracksFromB;
  std::vector<const xAOD::TruthParticle*> first_CtracksFromB;
  std::vector<const xAOD::TruthParticle*> first_CtracksFromC;

  std::vector<int>     j_bH_pdgId;
  std::vector<int>     j_bH_parent_pdgId;
  std::vector<float>   j_bH_pt;
  std::vector<float>   j_bH_eta;
  std::vector<float>   j_bH_phi;
  std::vector<float>   j_bH_E;
  std::vector<float>   j_bH_charge;
  std::vector<float>   j_bH_Lxy;
  std::vector<float>   j_bH_x;
  std::vector<float>   j_bH_y;
  std::vector<float>   j_bH_z;
  std::vector<float>   j_bH_prod_x;
  std::vector<float>   j_bH_prod_y;
  std::vector<float>   j_bH_prod_z;
  std::vector<float>   j_bH_dRjet;
  std::vector<float>   j_bH_PtTrk;
  std::vector<float>   j_bH_MTrk;
  std::vector<int>     j_bH_nBtracks;
  std::vector<int>     j_bH_nCtracks;
  std::vector<int>     j_bH_nBtracks_400;
  std::vector<int>     j_bH_nCtracks_400;
  std::vector<int>     j_bH_child_hadron_idx;
  std::vector<int>     j_bH_child_pdg_id;
  std::vector<int>     j_bH_child_parent_pdg_id;
  std::vector<int>     j_bH_child_barcode;
  std::vector<float>   j_bH_child_charge;
  std::vector<float>   j_bH_child_px;
  std::vector<float>   j_bH_child_py;
  std::vector<float>   j_bH_child_pz;
  std::vector<float>   j_bH_child_E;
  std::vector<float>   j_bH_child_prod_x;
  std::vector<float>   j_bH_child_prod_y;
  std::vector<float>   j_bH_child_prod_z;
  std::vector<float>   j_bH_child_decay_x;
  std::vector<float>   j_bH_child_decay_y;
  std::vector<float>   j_bH_child_decay_z;

  std::vector<int>     j_cH_pdgId;
  std::vector<int>     j_cH_parent_pdgId;
  std::vector<float>   j_cH_pt;
  std::vector<float>   j_cH_eta;
  std::vector<float>   j_cH_phi;
  std::vector<float>   j_cH_E;
  std::vector<float>   j_cH_charge;
  std::vector<float>   j_cH_Lxy;
  std::vector<float>   j_cH_x;
  std::vector<float>   j_cH_y;
  std::vector<float>   j_cH_z;
  std::vector<float>   j_cH_prod_x;
  std::vector<float>   j_cH_prod_y;
  std::vector<float>   j_cH_prod_z;
  std::vector<float>   j_cH_dRjet;
  std::vector<float>   j_cH_PtTrk;
  std::vector<float>   j_cH_MTrk;
  std::vector<int>     j_cH_nCtracks;
  std::vector<int>     j_cH_nCtracks_400;
  std::vector<int>     j_cH_child_hadron_idx;
  std::vector<int>     j_cH_child_pdg_id;
  std::vector<int>     j_cH_child_parent_pdg_id;
  std::vector<int>     j_cH_child_barcode;
  std::vector<float>   j_cH_child_charge;
  std::vector<float>   j_cH_child_px;
  std::vector<float>   j_cH_child_py;
  std::vector<float>   j_cH_child_pz;
  std::vector<float>   j_cH_child_E;
  std::vector<float>   j_cH_child_prod_x;
  std::vector<float>   j_cH_child_prod_y;
  std::vector<float>   j_cH_child_prod_z;
  std::vector<float>   j_cH_child_decay_x;
  std::vector<float>   j_cH_child_decay_y;
  std::vector<float>   j_cH_child_decay_z;

  if ( ghostB.size()>0 && !double_btagging ) {

    //sort hadrons by dR to jet
    std::vector<int> BhadIndices = getDRSortedIndices(ghostB,&jet);


    for ( unsigned int iB=0; iB< ghostB.size(); iB++) {

      const xAOD::TruthParticle * myB=(const xAOD::TruthParticle*)(ghostB.at(BhadIndices[iB]));

      j_bH_pdgId.push_back( myB->pdgId()  );
      j_bH_parent_pdgId.push_back( myB->parent(0)->pdgId() );
      j_bH_pt.push_back( myB->pt()  );
      j_bH_eta.push_back( myB->eta()  );
      j_bH_phi.push_back( myB->phi()  );
      j_bH_E.push_back( myB->e()  );
      j_bH_charge.push_back( myB->charge() );
      j_bH_Lxy.push_back( sqrt( pow(myB->decayVtx()->x(), 2) + pow(myB->decayVtx()->y(), 2) )  );
      j_bH_x.push_back( myB->decayVtx()->x()  );
      j_bH_y.push_back( myB->decayVtx()->y() );
      j_bH_z.push_back( myB->decayVtx()->z()  );
      j_bH_prod_x.push_back( myB->prodVtx()->x()  );
      j_bH_prod_y.push_back( myB->prodVtx()->y() );
      j_bH_prod_z.push_back( myB->prodVtx()->z()  );
      float dEta = (myB->eta()) - (jet.eta()) ;
      float dPhi = acos(cos( fabs( myB->phi()-jet.phi() ) ) );
      j_bH_dRjet.push_back( sqrt(pow(dEta, 2) + pow(dPhi, 2))  );



      //loop over decay products, save tracks that are not c or b hadrons:
      std::vector<const xAOD::TruthParticle*> tracksFromB;
      std::vector<const xAOD::TruthParticle*> tracksFromC;

      GetAllChildren(myB, tracksFromB, tracksFromC, false ); //tracksFromB contains also tracksFromC

      if(iB==0){ // fill the first B (with minimal dR to jet) to check track origin
        GetAllChildren(myB, first_BtracksFromB, first_CtracksFromB, false );
      }


      int nBtrk_400=0;
      int nCtrk_400=0;

      TLorentzVector tracks_p4(0,0,0,0);

      for(unsigned i=0; i< tracksFromB.size(); i++){
        const xAOD::TruthParticle* trk =  tracksFromB.at(i);

        TLorentzVector trkp4;
        trkp4.SetPxPyPzE(trk->px(), trk->py(), trk->pz(), trk->e());

        if( fabs(trk->charge())>0 ){
          tracks_p4 = tracks_p4+trkp4;
        }

        if( trk->pt() > 400 && fabs(trk->eta()) < 2.5 ){ nBtrk_400++; }

        j_bH_child_hadron_idx.push_back(iB);
        j_bH_child_pdg_id.push_back( trk->pdgId() );
        j_bH_child_parent_pdg_id.push_back( trk->parent(0)->pdgId()  );
        j_bH_child_barcode.push_back( trk->barcode()  );
        j_bH_child_charge.push_back( trk->charge()  );
        j_bH_child_px.push_back( trk->px()  );
        j_bH_child_py.push_back( trk->py()  );
        j_bH_child_pz.push_back( trk->pz()  );
        j_bH_child_E.push_back( trk->e()  );
        j_bH_child_prod_x.push_back( trk->prodVtx()->x()  );
        j_bH_child_prod_y.push_back( trk->prodVtx()->y()  );
        j_bH_child_prod_z.push_back( trk->prodVtx()->z()  );

        if(trk->hasDecayVtx() ){
          j_bH_child_decay_x.push_back( trk->decayVtx()->x()  );
          j_bH_child_decay_y.push_back( trk->decayVtx()->y()  );
          j_bH_child_decay_z.push_back( trk->decayVtx()->z()  );
        }else{
          j_bH_child_decay_x.push_back( -999  );
          j_bH_child_decay_y.push_back( -999  );
          j_bH_child_decay_z.push_back( -999  );
        }
      }

      for(unsigned i=0; i< tracksFromC.size(); i++){
        const xAOD::TruthParticle* trk =  tracksFromC.at(i);

        TLorentzVector trkp4;
        trkp4.SetPxPyPzE(trk->px(), trk->py(), trk->pz(), trk->e());
        if( trkp4.Pt() > 400 && fabs(trkp4.Eta()) < 2.5  ){ nCtrk_400++; }

      }


      j_bH_nBtracks.push_back( tracksFromB.size()-tracksFromC.size());
      j_bH_nCtracks.push_back(tracksFromC.size());
      j_bH_nBtracks_400.push_back(nBtrk_400-nCtrk_400);
      j_bH_nCtracks_400.push_back(nCtrk_400);

      j_bH_PtTrk.push_back(tracks_p4.Pt());
      j_bH_MTrk.push_back(tracks_p4.M());


    }
  } // ghost B size >0
  else{
    j_bH_pdgId.push_back(-99);
    j_bH_parent_pdgId.push_back( -99 );
    j_bH_pt.push_back(-99);
    j_bH_eta.push_back(-99);
    j_bH_phi.push_back(-99);
    j_bH_E.push_back(-99);
    j_bH_charge.push_back(NAN);
    j_bH_Lxy.push_back(-99);
    j_bH_x.push_back(-99);
    j_bH_y.push_back(-99);
    j_bH_z.push_back(-99);
    j_bH_prod_x.push_back( -99 );
    j_bH_prod_y.push_back( -99 );
    j_bH_prod_z.push_back( -99 );
    j_bH_dRjet.push_back(-99);
    j_bH_PtTrk.push_back(-99);
    j_bH_MTrk.push_back(-99);
    j_bH_nBtracks.push_back(-99);
    j_bH_nCtracks.push_back(-99);
    j_bH_nBtracks_400.push_back(-99);
    j_bH_nCtracks_400.push_back(-99);
    j_bH_child_hadron_idx.push_back(-99);
    j_bH_child_pdg_id.push_back(-99);
    j_bH_child_parent_pdg_id.push_back(-99);
    j_bH_child_barcode.push_back(-99);
    j_bH_child_charge.push_back(-99);
    j_bH_child_px.push_back(-99);
    j_bH_child_py.push_back(-99);
    j_bH_child_pz.push_back(-99);
    j_bH_child_E.push_back(-99);
    j_bH_child_prod_x.push_back(-99);
    j_bH_child_prod_y.push_back(-99);
    j_bH_child_prod_z.push_back(-99);
    j_bH_child_decay_x.push_back(-99);
    j_bH_child_decay_y.push_back(-99);
    j_bH_child_decay_z.push_back(-99);
  }

  if ( ghostC.size()>0 && !double_btagging ) {

    std::vector<int> ChadIndices = getDRSortedIndices(ghostC,&jet);

    for ( unsigned int iC=0; iC< ghostC.size(); iC++) {

      const xAOD::TruthParticle * myC=(const xAOD::TruthParticle*)(ghostC.at(ChadIndices[iC]));

      j_cH_pdgId.push_back( myC->pdgId()  );
      j_cH_parent_pdgId.push_back( myC->parent(0)->pdgId() );
      j_cH_pt.push_back( myC->pt()  );
      j_cH_eta.push_back( myC->eta()  );
      j_cH_phi.push_back( myC->phi()  );
      j_cH_E.push_back( myC->e()  );
      j_cH_charge.push_back( myC->charge() );
      j_cH_Lxy.push_back( sqrt( pow(myC->decayVtx()->x(), 2) + pow(myC->decayVtx()->y(), 2) )  );
      j_cH_x.push_back( myC->decayVtx()->x()  );
      j_cH_y.push_back( myC->decayVtx()->y() );
      j_cH_z.push_back( myC->decayVtx()->z()  );
      j_cH_prod_x.push_back( myC->prodVtx()->x()  );
      j_cH_prod_y.push_back( myC->prodVtx()->y() );
      j_cH_prod_z.push_back( myC->prodVtx()->z()  );
      float dEta = (myC->eta()) - (jet.eta()) ;
      float dPhi = acos(cos( fabs( myC->phi()-jet.phi() ) ) );
      j_cH_dRjet.push_back( sqrt(pow(dEta, 2) + pow(dPhi, 2))  );



      //loop over decay products, save all decay products
      std::vector<const xAOD::TruthParticle*> tracksFromC;

      GetAllChildren(myC, tracksFromC, tracksFromC,  false );

      if(iC==0){ // fill the first C (with minimal dR to jet) to check track origin
        GetAllChildren(myC, first_CtracksFromC, first_CtracksFromC, false );
      }

      int nCtrk_400=0;

      TLorentzVector tracks_p4(0,0,0,0);

      for(unsigned i=0; i< tracksFromC.size(); i++){
        const xAOD::TruthParticle* trk =  tracksFromC.at(i);

        TLorentzVector trkp4;
        trkp4.SetPxPyPzE(trk->px(), trk->py(), trk->pz(), trk->e());


          tracks_p4 = tracks_p4+trkp4;
          if( trkp4.Pt() > 400 && fabs(trkp4.Eta()) < 2.5 ){ nCtrk_400++; }

        j_cH_child_hadron_idx.push_back(iC);
        j_cH_child_pdg_id.push_back( trk->pdgId() );
        j_cH_child_parent_pdg_id.push_back( trk->parent(0)->pdgId()  );
        j_cH_child_barcode.push_back( trk->barcode()  );
        j_cH_child_charge.push_back( trk->charge()  );
        j_cH_child_px.push_back( trk->px()  );
        j_cH_child_py.push_back( trk->py()  );
        j_cH_child_pz.push_back( trk->pz()  );
        j_cH_child_E.push_back( trk->e()  );
        j_cH_child_prod_x.push_back( trk->prodVtx()->x()  );
        j_cH_child_prod_y.push_back( trk->prodVtx()->y()  );
        j_cH_child_prod_z.push_back( trk->prodVtx()->z()  );

        if(trk->hasDecayVtx() ){
          j_cH_child_decay_x.push_back( trk->decayVtx()->x()  );
          j_cH_child_decay_y.push_back( trk->decayVtx()->y()  );
          j_cH_child_decay_z.push_back( trk->decayVtx()->z()  );
        }else{
          j_cH_child_decay_x.push_back( -999  );
          j_cH_child_decay_y.push_back( -999  );
          j_cH_child_decay_z.push_back( -999  );
        }
      }

      j_cH_nCtracks.push_back(tracksFromC.size());
      j_cH_nCtracks_400.push_back(nCtrk_400);

      j_cH_PtTrk.push_back(tracks_p4.Pt());
      j_cH_MTrk.push_back(tracks_p4.M());
    }

  } // ghost C size >0
  else{
    j_cH_pdgId.push_back(-99);
    j_cH_parent_pdgId.push_back( -99 );
    j_cH_pt.push_back(-99);
    j_cH_eta.push_back(-99);
    j_cH_phi.push_back(-99);
    j_cH_E.push_back(-99);
    j_cH_charge.push_back(NAN);
    j_cH_Lxy.push_back(-99);
    j_cH_x.push_back(-99);
    j_cH_y.push_back(-99);
    j_cH_z.push_back(-99);
    j_cH_prod_x.push_back( -99  );
    j_cH_prod_y.push_back( -99 );
    j_cH_prod_z.push_back( -99  );
    j_cH_dRjet.push_back(-99);
    j_cH_PtTrk.push_back(-99);
    j_cH_MTrk.push_back(-99);
    j_cH_nCtracks.push_back(-99);
    j_cH_nCtracks_400.push_back(-99);
    j_cH_child_hadron_idx.push_back(-99);
    j_cH_child_pdg_id.push_back(-99);
    j_cH_child_parent_pdg_id.push_back(-99);
    j_cH_child_barcode.push_back(-99);
    j_cH_child_charge.push_back(-99);
    j_cH_child_px.push_back(-99);
    j_cH_child_py.push_back(-99);
    j_cH_child_pz.push_back(-99);
    j_cH_child_E.push_back(-99);
    j_cH_child_prod_x.push_back(-99);
    j_cH_child_prod_y.push_back(-99);
    j_cH_child_prod_z.push_back(-99);
    j_cH_child_decay_x.push_back(-99);
    j_cH_child_decay_y.push_back(-99);
    j_cH_child_decay_z.push_back(-99);
  }




  m_branches->nBHadr->push_back(ghostB.size());
  m_branches->nCHadr->push_back(ghostC.size());

  m_branches->bH_pdgId->push_back(j_bH_pdgId);
  m_branches->bH_parent_pdgId->push_back(j_bH_parent_pdgId);
  m_branches->bH_pt->push_back(j_bH_pt);
  m_branches->bH_eta->push_back(j_bH_eta);
  m_branches->bH_phi->push_back(j_bH_phi);
  m_branches->bH_E->push_back(j_bH_E);
  m_branches->bH_charge->push_back(j_bH_charge);
  m_branches->bH_Lxy->push_back(j_bH_Lxy);
  m_branches->bH_x->push_back(j_bH_x);
  m_branches->bH_y->push_back(j_bH_y);
  m_branches->bH_z->push_back(j_bH_z);
  m_branches->bH_prod_x->push_back(j_bH_prod_x);
  m_branches->bH_prod_y->push_back(j_bH_prod_y);
  m_branches->bH_prod_z->push_back(j_bH_prod_z);
  m_branches->bH_dRjet->push_back(j_bH_dRjet);
  m_branches->bH_PtTrk->push_back(j_bH_PtTrk);
  m_branches->bH_MTrk->push_back(j_bH_MTrk);
  m_branches->bH_nBtracks->push_back(j_bH_nBtracks);
  m_branches->bH_nCtracks->push_back(j_bH_nCtracks);
  m_branches->bH_nBtracks_400->push_back(j_bH_nBtracks_400);
  m_branches->bH_nCtracks_400->push_back(j_bH_nCtracks_400);

  m_branches->bH_child_hadron_idx->push_back(j_bH_child_hadron_idx);
  m_branches->bH_child_pdg_id->push_back(j_bH_child_pdg_id);
  m_branches->bH_child_parent_pdg_id->push_back(j_bH_child_parent_pdg_id);
  m_branches->bH_child_barcode->push_back(j_bH_child_barcode);
  m_branches->bH_child_charge->push_back(j_bH_child_charge);
  m_branches->bH_child_px->push_back(j_bH_child_px);
  m_branches->bH_child_py->push_back(j_bH_child_py);
  m_branches->bH_child_pz->push_back(j_bH_child_pz);
  m_branches->bH_child_E->push_back(j_bH_child_E);
  m_branches->bH_child_prod_x->push_back(j_bH_child_prod_x);
  m_branches->bH_child_prod_y->push_back(j_bH_child_prod_y);
  m_branches->bH_child_prod_z->push_back(j_bH_child_prod_z);
  m_branches->bH_child_decay_x->push_back(j_bH_child_decay_x);
  m_branches->bH_child_decay_y->push_back(j_bH_child_decay_y);
  m_branches->bH_child_decay_z->push_back(j_bH_child_decay_z);


  m_branches->cH_pdgId->push_back(j_cH_pdgId);
  m_branches->cH_parent_pdgId->push_back(j_cH_parent_pdgId);
  m_branches->cH_pt->push_back(j_cH_pt);
  m_branches->cH_eta->push_back(j_cH_eta);
  m_branches->cH_phi->push_back(j_cH_phi);
  m_branches->cH_E->push_back(j_cH_E);
  m_branches->cH_charge->push_back(j_cH_charge);
  m_branches->cH_Lxy->push_back(j_cH_Lxy);
  m_branches->cH_x->push_back(j_cH_x);
  m_branches->cH_y->push_back(j_cH_y);
  m_branches->cH_z->push_back(j_cH_z);
  m_branches->cH_prod_x->push_back(j_cH_prod_x);
  m_branches->cH_prod_y->push_back(j_cH_prod_y);
  m_branches->cH_prod_z->push_back(j_cH_prod_z);
  m_branches->cH_dRjet->push_back(j_cH_dRjet);
  m_branches->cH_PtTrk->push_back(j_cH_PtTrk);
  m_branches->cH_MTrk->push_back(j_cH_MTrk);
  m_branches->cH_nCtracks->push_back(j_cH_nCtracks);
  m_branches->cH_nCtracks_400->push_back(j_cH_nCtracks_400);

  m_branches->cH_child_hadron_idx->push_back(j_cH_child_hadron_idx);
  m_branches->cH_child_pdg_id->push_back(j_cH_child_pdg_id);
  m_branches->cH_child_parent_pdg_id->push_back(j_cH_child_parent_pdg_id);
  m_branches->cH_child_barcode->push_back(j_cH_child_barcode);
  m_branches->cH_child_charge->push_back(j_cH_child_charge);
  m_branches->cH_child_px->push_back(j_cH_child_px);
  m_branches->cH_child_py->push_back(j_cH_child_py);
  m_branches->cH_child_pz->push_back(j_cH_child_pz);
  m_branches->cH_child_E->push_back(j_cH_child_E);
  m_branches->cH_child_prod_x->push_back(j_cH_child_prod_x);
  m_branches->cH_child_prod_y->push_back(j_cH_child_prod_y);
  m_branches->cH_child_prod_z->push_back(j_cH_child_prod_z);
  m_branches->cH_child_decay_x->push_back(j_cH_child_decay_x);
  m_branches->cH_child_decay_y->push_back(j_cH_child_decay_y);
  m_branches->cH_child_decay_z->push_back(j_cH_child_decay_z);


  // double b-tagging variables

  // additions by nikola
  const xAOD::TruthParticle *matchedBH1 = NULL;
  const xAOD::TruthParticle *matchedBH2 = NULL;
  const xAOD::TruthParticle *matchedBH1FromParent = NULL;
  const xAOD::TruthParticle *matchedBH2FromParent = NULL;
  const xAOD::TruthParticle *matchedCNotFromB1FromParent = NULL;
  const xAOD::TruthParticle *matchedCNotFromB2FromParent = NULL;
  // double b-tagging (on trimmed large-R jets, AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets)

  if ( double_btagging ) {

    // get ghost B Hadrons from parent jet
    std::vector<const IParticle*> ghostBFromParent; ghostBFromParent.reserve(2);
    jet_parent->getAssociatedObjects<IParticle>("GhostBHadronsFinal", ghostBFromParent);
    m_branches->v_jet_nGhostBHadrFromParent->push_back(ghostBFromParent.size());    // the number of ghost B Hadrons from parent jet

    // get ghost B Hadrons from jet
    std::vector<const IParticle*> ghostB; ghostB.reserve(2);
    jet.getAssociatedObjects<IParticle>("GhostBHadronsFinal", ghostB);
    m_branches->v_jet_nGhostBHadr->push_back(ghostB.size());    // the number of ghost B Hadrons from jet

    // use LEADING 2 ghost B Hadrons from parent jet to later label b-tagging tracks
    if (ghostBFromParent.size() >= 1) {
      matchedBH1FromParent = (const xAOD::TruthParticle*)(ghostBFromParent.at(0));
      if (ghostB.size() >= 2) {
        matchedBH2FromParent=(const xAOD::TruthParticle*)(ghostBFromParent.at(1));
      }
    }

    // use LEADING 2 ghost B Hadrons from jet
    if (ghostB.size() >= 1) {
        matchedBH1 = (const xAOD::TruthParticle*)(ghostB.at(0));
      if (ghostB.size() >= 2) {
        matchedBH2=(const xAOD::TruthParticle*)(ghostB.at(1));
      }
    }

    // get ghost C Hadrons from parent jet
    std::vector<const IParticle*> ghostCFromParent; ghostCFromParent.reserve(2);
    jet_parent->getAssociatedObjects<IParticle>("GhostCHadronsFinal", ghostCFromParent);
    m_branches->v_jet_nGhostCHadrFromParent->push_back(ghostCFromParent.size());    // the number of ghost C Hadrons from parent jet

    // get ghost C Hadrons from jet
    std::vector<const IParticle*> ghostC; ghostC.reserve(2);
    jet.getAssociatedObjects<IParticle>("GhostCHadronsFinal", ghostC);
    m_branches->v_jet_nGhostCHadr->push_back(ghostC.size());    // the number of ghost C Hadrons from jet

    // get ghost C Hadrons from parent jet which are NOT children of ghost B Hadrons from parent jet
    int nGhostCHadrFromParentNotFromB = 0;
    // loop over C Hadrons
    for (unsigned int c = 0; c < ghostCFromParent.size(); c++) {
       const xAOD::TruthParticle* cHadron = (const xAOD::TruthParticle*)(ghostCFromParent.at(c));
       int cHadronComesFromB = 0;

       // loop over B Hadrons
       for (unsigned int b = 0; b < ghostBFromParent.size(); b++) {
         const xAOD::TruthParticle* bHadron = (const xAOD::TruthParticle*)(ghostBFromParent.at(b));

         // loop over C Hadron parents
         const xAOD::TruthParticle* cHadronParent = cHadron->parent(0);
         while (cHadronParent != NULL) {
           if (bHadron == cHadronParent) {
             // ATH_MSG_INFO ("nikola: C Hadron has B Hadron parent");
             cHadronComesFromB = 1;
             break;
           }
           if (cHadronComesFromB) break;
           else cHadronParent = cHadronParent->parent(0);
         }
       }

       // use LEADING 2 ghost C Hadrons from parent jet which are NOT children of ghost B Hadrons from parent jet to later label b-tagging tracks
       if (!cHadronComesFromB) {
         nGhostCHadrFromParentNotFromB += 1;
         if (matchedCNotFromB1FromParent == NULL) matchedCNotFromB1FromParent = cHadron;
         else if (matchedCNotFromB2FromParent == NULL) matchedCNotFromB2FromParent = cHadron;
         else std::cout << "more than 2 C Hadrons which do not come from a B Hadron have been found..." << std::endl;
       }
    }
    m_branches->v_jet_nGhostCHadrFromParentNotFromB->push_back(nGhostCHadrFromParentNotFromB);    // the number of ghost C Hadrons from parent jet which are NOT children of ghost B Hadrons from parent jet

    // get ghost C Hadrons from jet which are NOT children of ghost B Hadrons from jet
    int nGhostCHadrNotFromB = 0;
    // loop over C Hadrons
    for (unsigned int c = 0; c < ghostC.size(); c++) {
       const xAOD::TruthParticle* cHadron = (const xAOD::TruthParticle*)(ghostC.at(c));
       int cHadronComesFromB = 0;

       // loop over B Hadrons
       for (unsigned int b = 0; b < ghostB.size(); b++) {
         const xAOD::TruthParticle* bHadron = (const xAOD::TruthParticle*)(ghostB.at(b));

         // loop over C Hadron parents
         const xAOD::TruthParticle* cHadronParent = cHadron->parent(0);
         while (cHadronParent != NULL) {
           if (bHadron == cHadronParent) {
             // ATH_MSG_INFO ("nikola: C Hadron has B Hadron parent");
             cHadronComesFromB = 1;
             break;
           }
           if (cHadronComesFromB) break;
           else cHadronParent = cHadronParent->parent(0);
         }
       }
    }
    m_branches->v_jet_nGhostCHadrNotFromB->push_back(nGhostCHadrNotFromB);    // the number of ghost C Hadrons from jet which are NOT children of ghost B Hadrons from jet

    // ghost Tau from parent jet
    std::vector<const IParticle*> ghostTauFromParent; ghostTauFromParent.reserve(2);
    jet_parent->getAssociatedObjects<IParticle>("GhostTausFinal", ghostTauFromParent);
    m_branches->v_jet_nGhostTauFromParent->push_back(ghostTauFromParent.size());

    // ghost Tau from jet
    std::vector<const IParticle*> ghostTau; ghostTau.reserve(2);
    jet.getAssociatedObjects<IParticle>("GhostTausFinal", ghostTau);
    m_branches->v_jet_nGhostTau->push_back(ghostTau.size());

    // ghost H from parent jet
    std::vector<const IParticle*> ghostHFromParent; ghostHFromParent.reserve(2);
    jet_parent->getAssociatedObjects<IParticle>("GhostHBosons", ghostHFromParent);
    m_branches->v_jet_nGhostHBosoFromParent->push_back(ghostHFromParent.size());

    // ghost H from jet
    std::vector<const IParticle*> ghostH; ghostH.reserve(2);
    jet.getAssociatedObjects<IParticle>("GhostHBosons", ghostH);
    m_branches->v_jet_nGhostHBoso->push_back(ghostH.size());
  }

  std::vector<const xAOD::TruthParticle*> tracksFromB1FromParent;
  std::vector<const xAOD::TruthParticle*> tracksFromB2FromParent;
  std::vector<const xAOD::TruthParticle*> tracksFromC1FromParent;
  std::vector<const xAOD::TruthParticle*> tracksFromC2FromParent;
  std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB1FromParent;
  std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB2FromParent;

  if (matchedBH1FromParent != NULL) {
    GetAllChildren(matchedBH1FromParent, tracksFromB1FromParent, tracksFromC1FromParent, false);
  }
  if (matchedBH2FromParent != NULL) {
    GetAllChildren(matchedBH2FromParent, tracksFromB2FromParent, tracksFromC2FromParent, false);
  }
  if (matchedCNotFromB1FromParent != NULL) {
    GetAllChildren(matchedCNotFromB1FromParent, tracksFromCNotFromB1FromParent, tracksFromCNotFromB1FromParent, false);
  }
  if (matchedCNotFromB2FromParent != NULL) {
    GetAllChildren(matchedCNotFromB2FromParent, tracksFromCNotFromB2FromParent, tracksFromCNotFromB2FromParent, false);
  }

  // nikola to-do: make this more elegant (maybe loop over all B Hadrons?) maybe add C1 and C2 info
    if (matchedBH1FromParent != NULL) {
      m_branches->v_bH1FromParent_pt->push_back(matchedBH1FromParent->pt());
      m_branches->v_bH1FromParent_eta->push_back(matchedBH1FromParent->eta());
      m_branches->v_bH1FromParent_phi->push_back(matchedBH1FromParent->phi());
      float Lxy = sqrt( pow(matchedBH1FromParent->decayVtx()->x(), 2) + pow(matchedBH1FromParent->decayVtx()->y(), 2) );
      m_branches->v_bH1FromParent_Lxy->push_back(Lxy);
      m_branches->v_bH1FromParent_x->push_back(matchedBH1FromParent->decayVtx()->x());
      m_branches->v_bH1FromParent_y->push_back(matchedBH1FromParent->decayVtx()->y());
      m_branches->v_bH1FromParent_z->push_back(matchedBH1FromParent->decayVtx()->z());
      float dr = deltaR(jet.eta(), matchedBH1FromParent->eta(), jet.phi(), matchedBH1FromParent->phi());
      m_branches->v_bH1FromParent_dRjet->push_back(dr);
    }
    else {
      m_branches->v_bH1FromParent_pt->push_back(-999);
      m_branches->v_bH1FromParent_eta->push_back(-999);
      m_branches->v_bH1FromParent_phi->push_back(-999);
      m_branches->v_bH1FromParent_Lxy->push_back(-999);
      m_branches->v_bH1FromParent_dRjet->push_back(-999);
      m_branches->v_bH1FromParent_x->push_back(-999);
      m_branches->v_bH1FromParent_y->push_back(-999);
      m_branches->v_bH1FromParent_z->push_back(-999);
    }
    if (matchedBH2FromParent != NULL) {
      m_branches->v_bH2FromParent_pt->push_back(matchedBH2FromParent->pt());
      m_branches->v_bH2FromParent_eta->push_back(matchedBH2FromParent->eta());
      m_branches->v_bH2FromParent_phi->push_back(matchedBH2FromParent->phi());
      float Lxy = sqrt( pow(matchedBH2FromParent->decayVtx()->x(), 2) + pow(matchedBH2FromParent->decayVtx()->y(), 2) );
      m_branches->v_bH2FromParent_Lxy->push_back(Lxy);
      m_branches->v_bH2FromParent_x->push_back(matchedBH2FromParent->decayVtx()->x());
      m_branches->v_bH2FromParent_y->push_back(matchedBH2FromParent->decayVtx()->y());
      m_branches->v_bH2FromParent_z->push_back(matchedBH2FromParent->decayVtx()->z());
      float dr = deltaR(jet.eta(), matchedBH2FromParent->eta(), jet.phi(), matchedBH2FromParent->phi());
      m_branches->v_bH2FromParent_dRjet->push_back(dr);
    }
    else {
      m_branches->v_bH2FromParent_pt->push_back(-999);
      m_branches->v_bH2FromParent_eta->push_back(-999);
      m_branches->v_bH2FromParent_phi->push_back(-999);
      m_branches->v_bH2FromParent_Lxy->push_back(-999);
      m_branches->v_bH2FromParent_dRjet->push_back(-999);
      m_branches->v_bH2FromParent_x->push_back(-999);
      m_branches->v_bH2FromParent_y->push_back(-999);
      m_branches->v_bH2FromParent_z->push_back(-999);
    }
    if (matchedBH1 != NULL) {
      m_branches->v_bH1_pt->push_back(matchedBH1->pt());
      m_branches->v_bH1_eta->push_back(matchedBH1->eta());
      m_branches->v_bH1_phi->push_back(matchedBH1->phi());
      float Lxy = sqrt( pow(matchedBH1->decayVtx()->x(), 2) + pow(matchedBH1->decayVtx()->y(), 2) );
      m_branches->v_bH1_Lxy->push_back(Lxy);
      m_branches->v_bH1_x->push_back(matchedBH1->decayVtx()->x());
      m_branches->v_bH1_y->push_back(matchedBH1->decayVtx()->y());
      m_branches->v_bH1_z->push_back(matchedBH1->decayVtx()->z());
      float dr = deltaR(jet.eta(), matchedBH1->eta(), jet.phi(), matchedBH1->phi());
      m_branches->v_bH1_dRjet->push_back(dr);
    }
    else {
      m_branches->v_bH1_pt->push_back(-999);
      m_branches->v_bH1_eta->push_back(-999);
      m_branches->v_bH1_phi->push_back(-999);
      m_branches->v_bH1_Lxy->push_back(-999);
      m_branches->v_bH1_dRjet->push_back(-999);
      m_branches->v_bH1_x->push_back(-999);
      m_branches->v_bH1_y->push_back(-999);
      m_branches->v_bH1_z->push_back(-999);
    }
    if (matchedBH2 != NULL) {
      m_branches->v_bH2_pt->push_back(matchedBH2->pt());
      m_branches->v_bH2_eta->push_back(matchedBH2->eta());
      m_branches->v_bH2_phi->push_back(matchedBH2->phi());
      float Lxy = sqrt( pow(matchedBH2->decayVtx()->x(), 2) + pow(matchedBH2->decayVtx()->y(), 2) );
      m_branches->v_bH2_Lxy->push_back(Lxy);
      m_branches->v_bH2_x->push_back(matchedBH2->decayVtx()->x());
      m_branches->v_bH2_y->push_back(matchedBH2->decayVtx()->y());
      m_branches->v_bH2_z->push_back(matchedBH2->decayVtx()->z());
      float dr = deltaR(jet.eta(), matchedBH2->eta(), jet.phi(), matchedBH2->phi());
      m_branches->v_bH2_dRjet->push_back(dr);
    }
    else {
      m_branches->v_bH2_pt->push_back(-999);
      m_branches->v_bH2_eta->push_back(-999);
      m_branches->v_bH2_phi->push_back(-999);
      m_branches->v_bH2_Lxy->push_back(-999);
      m_branches->v_bH2_dRjet->push_back(-999);
      m_branches->v_bH2_x->push_back(-999);
      m_branches->v_bH2_y->push_back(-999);
      m_branches->v_bH2_z->push_back(-999);
    }

    // track origin - match tracks to particles from B/C decays:
    // get tracks from different track<->jet associators for trimmed large-R jets vs other jet collections
    std::vector< ElementLink< xAOD::TrackParticleContainer > > assocTracks;
    std::vector<const xAOD::TrackParticle*> selectedTracks; // tracks passing number of Pixel and SCT hits requirements

    const xAOD::BTagging *bjet = jet.btagging();

    if (double_btagging) {
      assocTracks = bjet->auxdata<std::vector<ElementLink<xAOD::TrackParticleContainer> > >("BTagTrackToJetAssociatorBB");
    }
    else {
      assocTracks = bjet->auxdata<std::vector<ElementLink<xAOD::TrackParticleContainer> > >("BTagTrackToJetAssociator");
    }

    std::vector<int> j_trk_orig;

    //track loop, select only tracks with 2 or more hits
    uint8_t getInt(0);   // for accessing summary information

    for (unsigned int iT = 0; iT < assocTracks.size(); iT++) {

      if (!assocTracks.at(iT).isValid()) continue;

      const xAOD::TrackParticle *tmpTrk = *(assocTracks.at(iT));

      tmpTrk->summaryValue(getInt, xAOD::numberOfPixelHits);
      int nSi = getInt;
      tmpTrk->summaryValue(getInt, xAOD::numberOfSCTHits);
      nSi += getInt;
      if (nSi < 2) continue;
      selectedTracks.push_back(tmpTrk);

    }

    //track loop, find origin of track

    for (const auto* tmpTrk: selectedTracks) {

      int origin = getTrackOrigin(tmpTrk,
                                  first_BtracksFromB,
                                  first_CtracksFromB,
                                  first_CtracksFromC,
                                  tracksFromB1FromParent,
                                  tracksFromB2FromParent,
                                  tracksFromC1FromParent,
                                  tracksFromC2FromParent,
                                  tracksFromCNotFromB1FromParent,
                                  tracksFromCNotFromB2FromParent);

      j_trk_orig.push_back(origin);
    } //end track loop

    m_branches->v_jet_trk_orig->push_back(j_trk_orig);
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::clear() {
  // clear vectors
  m_branches->v_jet_trk_orig->clear();

  m_branches->nBHadr->clear();
  m_branches->nCHadr->clear();
  m_branches->bH_pdgId->clear();
  m_branches->bH_parent_pdgId->clear();
  m_branches->bH_pt->clear();
  m_branches->bH_eta->clear();
  m_branches->bH_phi->clear();
  m_branches->bH_E->clear();
  m_branches->bH_charge->clear();
  m_branches->bH_Lxy->clear();
  m_branches->bH_x->clear();
  m_branches->bH_y->clear();
  m_branches->bH_z->clear();
  m_branches->bH_prod_x->clear();
  m_branches->bH_prod_y->clear();
  m_branches->bH_prod_z->clear();
  m_branches->bH_dRjet->clear();
  m_branches->bH_PtTrk->clear();
  m_branches->bH_MTrk->clear();
  m_branches->bH_nBtracks->clear();
  m_branches->bH_nCtracks->clear();
  m_branches->bH_nBtracks_400->clear();
  m_branches->bH_nCtracks_400->clear();
  m_branches->bH_child_hadron_idx->clear();
  m_branches->bH_child_pdg_id->clear();
  m_branches->bH_child_parent_pdg_id->clear();
  m_branches->bH_child_barcode->clear();
  m_branches->bH_child_charge->clear();
  m_branches->bH_child_px->clear();
  m_branches->bH_child_py->clear();
  m_branches->bH_child_pz->clear();
  m_branches->bH_child_E->clear();
  m_branches->bH_child_prod_x->clear();
  m_branches->bH_child_prod_y->clear();
  m_branches->bH_child_prod_z->clear();
  m_branches->bH_child_decay_x->clear();
  m_branches->bH_child_decay_y->clear();
  m_branches->bH_child_decay_z->clear();

  m_branches->cH_pdgId->clear();
  m_branches->cH_parent_pdgId->clear();
  m_branches->cH_pt->clear();
  m_branches->cH_eta->clear();
  m_branches->cH_phi->clear();
  m_branches->cH_E->clear();
  m_branches->cH_charge->clear();
  m_branches->cH_Lxy->clear();
  m_branches->cH_x->clear();
  m_branches->cH_y->clear();
  m_branches->cH_z->clear();
  m_branches->cH_prod_x->clear();
  m_branches->cH_prod_y->clear();
  m_branches->cH_prod_z->clear();
  m_branches->cH_dRjet->clear();
  m_branches->cH_PtTrk->clear();
  m_branches->cH_MTrk->clear();
  m_branches->cH_nCtracks->clear();
  m_branches->cH_nCtracks_400->clear();
  m_branches->cH_child_hadron_idx->clear();
  m_branches->cH_child_pdg_id->clear();
  m_branches->cH_child_parent_pdg_id->clear();
  m_branches->cH_child_barcode->clear();
  m_branches->cH_child_charge->clear();
  m_branches->cH_child_px->clear();
  m_branches->cH_child_py->clear();
  m_branches->cH_child_pz->clear();
  m_branches->cH_child_E->clear();
  m_branches->cH_child_prod_x->clear();
  m_branches->cH_child_prod_y->clear();
  m_branches->cH_child_prod_z->clear();
  m_branches->cH_child_decay_x->clear();
  m_branches->cH_child_decay_y->clear();
  m_branches->cH_child_decay_z->clear();

  // double b-tagging
  m_branches->v_jet_nGhostBHadrFromParent->clear(); // mod nikola
  m_branches->v_jet_nGhostCHadrFromParent->clear(); // mod nikola
  m_branches->v_jet_nGhostCHadrFromParentNotFromB->clear(); // mod nikola
  m_branches->v_jet_nGhostTauFromParent->clear(); // mod nikola
  m_branches->v_jet_nGhostHBosoFromParent->clear(); // mod nikola
  m_branches->v_jet_nGhostBHadr->clear(); // mod nikola
  m_branches->v_jet_nGhostCHadr->clear(); // mod nikola
  m_branches->v_jet_nGhostCHadrNotFromB->clear(); // mod nikola
  m_branches->v_jet_nGhostTau->clear(); // mod nikola
  m_branches->v_jet_nGhostHBoso->clear(); // mod nikola

  m_branches->v_bH1FromParent_pt->clear();
  m_branches->v_bH1FromParent_eta->clear();
  m_branches->v_bH1FromParent_phi->clear();
  m_branches->v_bH1FromParent_Lxy->clear();
  m_branches->v_bH1FromParent_dRjet->clear();
  m_branches->v_bH1FromParent_x->clear();
  m_branches->v_bH1FromParent_y->clear();
  m_branches->v_bH1FromParent_z->clear();

  m_branches->v_bH2FromParent_pt->clear();
  m_branches->v_bH2FromParent_eta->clear();
  m_branches->v_bH2FromParent_phi->clear();
  m_branches->v_bH2FromParent_Lxy->clear();
  m_branches->v_bH2FromParent_dRjet->clear();
  m_branches->v_bH2FromParent_x->clear();
  m_branches->v_bH2FromParent_y->clear();
  m_branches->v_bH2FromParent_z->clear();

  m_branches->v_bH1_pt->clear();
  m_branches->v_bH1_eta->clear();
  m_branches->v_bH1_phi->clear();
  m_branches->v_bH1_Lxy->clear();
  m_branches->v_bH1_dRjet->clear();
  m_branches->v_bH1_x->clear();
  m_branches->v_bH1_y->clear();
  m_branches->v_bH1_z->clear();

  m_branches->v_bH2_pt->clear();
  m_branches->v_bH2_eta->clear();
  m_branches->v_bH2_phi->clear();
  m_branches->v_bH2_Lxy->clear();
  m_branches->v_bH2_dRjet->clear();
  m_branches->v_bH2_x->clear();
  m_branches->v_bH2_y->clear();
  m_branches->v_bH2_z->clear();
}

float BHadronBranches :: deltaR(float eta1, float eta2, float phi1, float phi2) {
  float DEta = fabs(eta1 - eta2);
  float DPhi = acos(cos(fabs(phi1 - phi2)));
  return sqrt(pow(DEta, 2) + pow(DPhi, 2));
}


const xAOD::Jet *BHadronBranches :: GetParentJet(const xAOD::Jet *Jet, std::string Keyname) {
  ElementLink<xAOD::JetContainer> el = Jet->auxdata<ElementLink<xAOD::JetContainer> >(Keyname);

  if(el.isValid()) {
    return *el;
  }
  else {
    std::cout << "GetParentJet(): Unable to get parent link %s ! Null ptr is returned." << std::endl;
    return 0;
  }
}


void BHadronBranches :: GetAllChildren(const xAOD::TruthParticle* particle,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromB,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromC,
                                           bool isFromC){


  if(!particle->hasDecayVtx()) return;

  const xAOD::TruthVertex* decayvtx = particle->decayVtx();

  for(unsigned i=0; i< decayvtx->nOutgoingParticles(); i++){

     const xAOD::TruthParticle* child = decayvtx->outgoingParticle(i);

    if (child->barcode() > 200e3) continue;
        if (child->status()==1 && child->isCharged() && !child->isCharmHadron() && !child->isBottomHadron() ){

        tracksFromB.push_back(child);
        if(isFromC){ tracksFromC.push_back(child); }
    }

     if (isFromC) GetAllChildren(child, tracksFromB, tracksFromC, true);
     else GetAllChildren(child, tracksFromB, tracksFromC, child->isCharmHadron() );

  }

}


std::vector<int> BHadronBranches :: getDRSortedIndices(std::vector<const xAOD::IParticle*> ghostHads, const xAOD::Jet *jet){
    std::vector<float> dRofhadrons;

    for(unsigned int ip = 0; ip < ghostHads.size(); ip++){
      float dEta = (ghostHads.at(ip))->eta() - (jet->eta()) ;
      float dPhi = acos(cos( fabs( (ghostHads.at(ip))->phi()-jet->phi() ) ) );
      float dr = sqrt(pow(dEta, 2) + pow(dPhi, 2));
      dRofhadrons.push_back(dr);
    }

    std::vector<int> y(dRofhadrons.size());
    std::size_t n(0);
    std::generate(std::begin(y), std::end(y), [&]{ return n++; });
    std::sort(std::begin(y),std::end(y),[&](int i1, int i2) { return dRofhadrons[i1] < dRofhadrons[i2]; });

    return y;
}


int BHadronBranches :: getTrackOrigin(const xAOD::TrackParticle *tmpTrk,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCc,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB1FromParent,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB2FromParent,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC1FromParent,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC2FromParent,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB1FromParent,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB2FromParent) {
      // origin
      int origin = PUFAKE;
      const xAOD::TruthParticle *truth = NULL;//truthParticle(tmpTrk);

      if(  tmpTrk->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink") ) {
        ElementLink<xAOD::TruthParticleContainer> link = tmpTrk->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");

        if(!link.isValid()){
          return origin;
        }
        truth = *link;
      }

      float truthProb = -1; // need to check MCtruth classifier
      try {
         truthProb = tmpTrk->auxdata< float >("truthMatchProbability");
      } catch(...) {};
      if (truth && truthProb > 0.75) {
        int truthBarcode = truth->barcode();
        if (truthBarcode > 2e5) origin = GEANT;
        else {
          origin = FRAG;
          for (unsigned int iT = 0; iT < tracksFromB.size(); iT++) {
            if (truth == tracksFromB.at(iT)) {
              origin = FROMB;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromC.size(); iT++) {
            if (truth == tracksFromC.at(iT)) {
              origin = FROMC;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromCc.size(); iT++) {
            if (truth == tracksFromCc.at(iT)) {
              origin = FROMC;
              break;
            }
          }
          // additions by nikola
          for (unsigned int iT = 0; iT < tracksFromB1FromParent.size(); iT++) {
            if (truth == tracksFromB1FromParent.at(iT)) {
              origin = 10;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromB2FromParent.size(); iT++) {
            if (truth == tracksFromB2FromParent.at(iT)) {
              origin = 11;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromC1FromParent.size(); iT++) {
            if (truth == tracksFromC1FromParent.at(iT)) {
              origin = 12;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromC2FromParent.size(); iT++) {
            if (truth == tracksFromC2FromParent.at(iT)) {
              origin = 13;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromCNotFromB1FromParent.size(); iT++) {
            if (truth == tracksFromCNotFromB1FromParent.at(iT)) {
              origin = 14;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromCNotFromB2FromParent.size(); iT++) {
            if (truth == tracksFromCNotFromB2FromParent.at(iT)) {
              origin = 15;
              break;
            }
          }
        }
      }
  return origin;
}
