#include "JetFitterBranches.hh"
#include "JetFitterBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include "xAODJet/Jet.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthVertex.h"

#include <string>
#include <stdexcept>

using xAOD::IParticle;

//!-----------------------------------------------------------------------------------------------------------------------------!//

JetFitterAccessors::JetFitterAccessors():
  fittedCov("JetFitter_fittedCov"),
  fittedPosition("JetFitter_fittedPosition")
{
}

JetFitterBranches::JetFitterBranches():
  m_branches(new JetFitterBranchBuffer),
  m_accessors(new JetFitterAccessors)
{
  // instantiate all the vectors here ...

  m_branches->v_jet_trk_jf_Vertex = new std::vector<std::vector<int> >();

  m_branches->jet_jf_pb           = new std::vector<float>();
  m_branches->jet_jf_pc           = new std::vector<float>();
  m_branches->jet_jf_pu           = new std::vector<float>();
  m_branches->jet_jf_llr          = new std::vector<float>();
  m_branches->jet_jf_m            = new std::vector<float>();
  m_branches->jet_jf_mUncorr      = new std::vector<float>(); //eloi
  m_branches->jet_jf_efc          = new std::vector<float>();
  m_branches->jet_jf_deta         = new std::vector<float>();
  m_branches->jet_jf_dphi         = new std::vector<float>();
  m_branches->jet_jf_dRFlightDir  = new std::vector<float>(); //eloi
  m_branches->jet_jf_ntrkAtVx     = new std::vector<float>();
  m_branches->jet_jf_nvtx         = new std::vector<int>();
  m_branches->jet_jf_sig3d        = new std::vector<float>();
  m_branches->jet_jf_nvtx1t       = new std::vector<int>();
  m_branches->jet_jf_n2t          = new std::vector<int>();
  m_branches->jet_jf_VTXsize      = new std::vector<int>();
  m_branches->jet_jf_vtx_chi2     = new std::vector<std::vector<float> >(); // mod Remco
  m_branches->jet_jf_vtx_ndf      = new std::vector<std::vector<float> >(); // mod Remco
  m_branches->jet_jf_vtx_ntrk     = new std::vector<std::vector<int> >(); // mod Remco
  m_branches->jet_jf_vtx_L3d      = new std::vector<std::vector<float> >(); // mod Remco
  m_branches->jet_jf_vtx_sig3d    = new std::vector<std::vector<float> >(); // mod Remco
  m_branches->jet_jf_phi          = new std::vector<float>(); // mod Remco
  m_branches->jet_jf_theta        = new std::vector<float>(); // mod Remco

  m_branches->jet_jf_vtx_sigTrans = new std::vector<std::vector<float> >();
  m_branches->jet_jf_vtx_x        = new std::vector<std::vector<float> >();
  m_branches->jet_jf_vtx_x_err    = new std::vector<std::vector<float> >();
  m_branches->jet_jf_vtx_y        = new std::vector<std::vector<float> >();
  m_branches->jet_jf_vtx_y_err    = new std::vector<std::vector<float> >();
  m_branches->jet_jf_vtx_z        = new std::vector<std::vector<float> >();
  m_branches->jet_jf_vtx_z_err    = new std::vector<std::vector<float> >();

  m_branches->jet_jf_phi_err      = new std::vector<float>();
  m_branches->jet_jf_theta_err    = new std::vector<float>();

  m_branches->nTrk_vtx1                   = new std::vector<int>();
  m_branches->mass_first_vtx              = new std::vector<float>();
  m_branches->e_first_vtx                 = new std::vector<float>();
  m_branches->e_frac_vtx1                 = new std::vector<float>();
  m_branches->closestVtx_L3D              = new std::vector<float>();
  m_branches->JF_Lxy1                     = new std::vector<float>();
  m_branches->vtx1_MaxTrkRapidity_jf_path = new std::vector<float>();
  m_branches->vtx1_AvgTrkRapidity_jf_path = new std::vector<float>();
  m_branches->vtx1_MinTrkRapidity_jf_path = new std::vector<float>();
  m_branches->nTrk_vtx2                   = new std::vector<int>();
  m_branches->mass_second_vtx             = new std::vector<float>();
  m_branches->e_second_vtx                = new std::vector<float>();
  m_branches->e_frac_vtx2                 = new std::vector<float>();
  m_branches->second_closestVtx_L3D       = new std::vector<float>();
  m_branches->JF_Lxy2                     = new std::vector<float>();
  m_branches->vtx2_MaxTrkRapidity_jf_path = new std::vector<float>();
  m_branches->vtx2_AvgTrkRapidity_jf_path = new std::vector<float>();
  m_branches->vtx2_MinTrkRapidity_jf_path = new std::vector<float>();
  m_branches->MaxTrkRapidity_jf_path      = new std::vector<float>();
  m_branches->MinTrkRapidity_jf_path      = new std::vector<float>();
  m_branches->AvgTrkRapidity_jf_path      = new std::vector<float>();


}

//!-----------------------------------------------------------------------------------------------------------------------------!//
JetFitterBranches::~JetFitterBranches() {
  // delete all the vectors here ...
  delete m_branches->v_jet_trk_jf_Vertex;
  delete m_branches->jet_jf_pb;
  delete m_branches->jet_jf_pc;
  delete m_branches->jet_jf_pu;
  delete m_branches->jet_jf_llr;
  delete m_branches->jet_jf_m;
  delete m_branches->jet_jf_mUncorr;
  delete m_branches->jet_jf_efc;
  delete m_branches->jet_jf_deta;
  delete m_branches->jet_jf_dphi;
  delete m_branches->jet_jf_dRFlightDir;
  delete m_branches->jet_jf_ntrkAtVx;
  delete m_branches->jet_jf_nvtx;
  delete m_branches->jet_jf_sig3d;
  delete m_branches->jet_jf_nvtx1t;
  delete m_branches->jet_jf_n2t;
  delete m_branches->jet_jf_VTXsize;
  delete m_branches->jet_jf_vtx_chi2;
  delete m_branches->jet_jf_vtx_ndf;
  delete m_branches->jet_jf_vtx_ntrk;
  delete m_branches->jet_jf_vtx_L3d;
  delete m_branches->jet_jf_vtx_sig3d;
  delete m_branches->jet_jf_phi;
  delete m_branches->jet_jf_theta;
  delete m_branches->jet_jf_vtx_sigTrans;
  delete m_branches->jet_jf_vtx_x;
  delete m_branches->jet_jf_vtx_x_err;
  delete m_branches->jet_jf_vtx_y;
  delete m_branches->jet_jf_vtx_y_err;
  delete m_branches->jet_jf_vtx_z;
  delete m_branches->jet_jf_vtx_z_err;
  delete m_branches->jet_jf_phi_err;
  delete m_branches->jet_jf_theta_err;

  delete m_branches->nTrk_vtx1;
  delete m_branches->mass_first_vtx;
  delete m_branches->e_first_vtx;
  delete m_branches->e_frac_vtx1;
  delete m_branches->closestVtx_L3D;
  delete m_branches->JF_Lxy1;
  delete m_branches->vtx1_MaxTrkRapidity_jf_path;
  delete m_branches->vtx1_AvgTrkRapidity_jf_path;
  delete m_branches->vtx1_MinTrkRapidity_jf_path;
  delete m_branches->nTrk_vtx2;
  delete m_branches->mass_second_vtx;
  delete m_branches->e_second_vtx;
  delete m_branches->e_frac_vtx2;
  delete m_branches->second_closestVtx_L3D;
  delete m_branches->JF_Lxy2;
  delete m_branches->vtx2_MaxTrkRapidity_jf_path;
  delete m_branches->vtx2_AvgTrkRapidity_jf_path;
  delete m_branches->vtx2_MinTrkRapidity_jf_path;
  delete m_branches->MaxTrkRapidity_jf_path;
  delete m_branches->MinTrkRapidity_jf_path;
  delete m_branches->AvgTrkRapidity_jf_path;


  delete m_branches;

  delete m_accessors;
}

void JetFitterBranches::set_tree(TTree& output_tree) const {

  output_tree.Branch("PV_jf_x"            , &m_branches->PV_jf_x); // mod Remco
  output_tree.Branch("PV_jf_y"            , &m_branches->PV_jf_y); // mod Remco
  output_tree.Branch("PV_jf_z"            , &m_branches->PV_jf_z); // mod Remco
  output_tree.Branch("jet_trk_jf_Vertex"  , &m_branches->v_jet_trk_jf_Vertex); // mod Remco
  output_tree.Branch("jet_jf_pb"          , &m_branches->jet_jf_pb);
  output_tree.Branch("jet_jf_pc"          , &m_branches->jet_jf_pc);
  output_tree.Branch("jet_jf_pu"          , &m_branches->jet_jf_pu);
  output_tree.Branch("jet_jf_llr"         , &m_branches->jet_jf_llr);
  output_tree.Branch("jet_jf_m"           , &m_branches->jet_jf_m);
  output_tree.Branch("jet_jf_mUncorr"     , &m_branches->jet_jf_mUncorr); //eloi
  output_tree.Branch("jet_jf_efc"         , &m_branches->jet_jf_efc);
  output_tree.Branch("jet_jf_deta"        , &m_branches->jet_jf_deta);
  output_tree.Branch("jet_jf_dphi"        , &m_branches->jet_jf_dphi);
  output_tree.Branch("jet_jf_dRFlightDir" , &m_branches->jet_jf_dRFlightDir); //eloi
  output_tree.Branch("jet_jf_ntrkAtVx"    , &m_branches->jet_jf_ntrkAtVx);
  output_tree.Branch("jet_jf_nvtx"        , &m_branches->jet_jf_nvtx);
  output_tree.Branch("jet_jf_sig3d"       , &m_branches->jet_jf_sig3d);
  output_tree.Branch("jet_jf_nvtx1t"      , &m_branches->jet_jf_nvtx1t);
  output_tree.Branch("jet_jf_n2t"         , &m_branches->jet_jf_n2t);
  output_tree.Branch("jet_jf_VTXsize"     , &m_branches->jet_jf_VTXsize);
  output_tree.Branch("jet_jf_vtx_chi2"    , &m_branches->jet_jf_vtx_chi2); // mod Remco
  output_tree.Branch("jet_jf_vtx_ndf"     , &m_branches->jet_jf_vtx_ndf); // mod Remco
  output_tree.Branch("jet_jf_vtx_ntrk"    , &m_branches->jet_jf_vtx_ntrk); // mod Remco
  output_tree.Branch("jet_jf_vtx_L3D"     , &m_branches->jet_jf_vtx_L3d); // mod Remco
  output_tree.Branch("jet_jf_vtx_sig3D"   , &m_branches->jet_jf_vtx_sig3d); // mod Remco
  output_tree.Branch("jet_jf_phi"         , &m_branches->jet_jf_phi); // mod Remco
  output_tree.Branch("jet_jf_theta"       , &m_branches->jet_jf_theta); // mod Remco
  output_tree.Branch("jet_jf_vtx_sigTrans", &m_branches->jet_jf_vtx_sigTrans);
  output_tree.Branch("jet_jf_vtx_x"       , &m_branches->jet_jf_vtx_x);
  output_tree.Branch("jet_jf_vtx_x_err"   , &m_branches->jet_jf_vtx_x_err);
  output_tree.Branch("jet_jf_vtx_y"       , &m_branches->jet_jf_vtx_y);
  output_tree.Branch("jet_jf_vtx_y_err"   , &m_branches->jet_jf_vtx_y_err);
  output_tree.Branch("jet_jf_vtx_z"       , &m_branches->jet_jf_vtx_z);
  output_tree.Branch("jet_jf_vtx_z_err"   , &m_branches->jet_jf_vtx_z_err);
  output_tree.Branch("jet_jf_theta_err"   , &m_branches->jet_jf_theta_err);
  output_tree.Branch("jet_jf_phi_err"     , &m_branches->jet_jf_phi_err);

  output_tree.Branch( "nTrk_vtx1"                  , &m_branches->nTrk_vtx1);
  output_tree.Branch( "mass_first_vtx"             , &m_branches->mass_first_vtx);
  output_tree.Branch( "e_first_vtx"                , &m_branches->e_first_vtx);
  output_tree.Branch( "e_frac_vtx1"                , &m_branches->e_frac_vtx1);
  output_tree.Branch( "closestVtx_L3D"             , &m_branches->closestVtx_L3D);
  output_tree.Branch( "JF_Lxy1"                    , &m_branches->JF_Lxy1);
  output_tree.Branch( "vtx1_MaxTrkRapidity", &m_branches->vtx1_MaxTrkRapidity_jf_path);
  output_tree.Branch( "vtx1_AvgTrkRapidity", &m_branches->vtx1_AvgTrkRapidity_jf_path);
  output_tree.Branch( "vtx1_MinTrkRapidity", &m_branches->vtx1_MinTrkRapidity_jf_path);
  output_tree.Branch( "nTrk_vtx2"                  , &m_branches->nTrk_vtx2);
  output_tree.Branch( "mass_second_vtx"            , &m_branches->mass_second_vtx);
  output_tree.Branch( "e_second_vtx"               , &m_branches->e_second_vtx);
  output_tree.Branch( "e_frac_vtx2"                , &m_branches->e_frac_vtx2);
  output_tree.Branch( "second_closestVtx_L3D"      , &m_branches->second_closestVtx_L3D);
  output_tree.Branch( "JF_Lxy2"                    , &m_branches->JF_Lxy2);
  output_tree.Branch( "vtx2_MaxTrkRapidity", &m_branches->vtx2_MaxTrkRapidity_jf_path);
  output_tree.Branch( "vtx2_AvgTrkRapidity", &m_branches->vtx2_AvgTrkRapidity_jf_path);
  output_tree.Branch( "vtx2_MinTrkRapidity", &m_branches->vtx2_MinTrkRapidity_jf_path);
  output_tree.Branch( "MaxTrkRapidity"     , &m_branches->MaxTrkRapidity_jf_path);
  output_tree.Branch( "MinTrkRapidity"     , &m_branches->MinTrkRapidity_jf_path);
  output_tree.Branch( "AvgTrkRapidity"     , &m_branches->AvgTrkRapidity_jf_path);

}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void JetFitterBranches::fill(const xAOD::Jet& jet) {

    const xAOD::BTagging *bjet = jet.btagging();

    // JetFitter // VD: check the existence of the vertex and then fill the variables
    // this mimics what's done in MV2
    float jfm         = -99;
    float jfefc       = -99;
    float jfdeta      = -99;
    float jfdphi      = -99;
    int   jfntrkAtVx  = -1;
    int   jfnvtx      = -1;
    float jfsig3d     = -99;
    int   jfnvtx1t    = -1;
    int   jfn2t       = -1;
    float massUncorr  = -99;  // eloi
    float dRFlightDir = -99; // eloi

    int   nTrk_vtx1                   = 0;
    float mass_first_vtx              = -100;
    float e_first_vtx                 = -100;
    float e_frac_vtx1                 = 0;
    float closestVtx_L3D              = -10;
    float JF_Lxy1                     = -5;
    float vtx1_MaxTrkRapidity_jf_path = 0;
    float vtx1_AvgTrkRapidity_jf_path = 0;
    float vtx1_MinTrkRapidity_jf_path = 0;
    int   nTrk_vtx2                   = 0;
    float mass_second_vtx             = -100;
    float e_second_vtx                = -100;
    float e_frac_vtx2                 = 0;
    float second_closestVtx_L3D       = -10;
    float JF_Lxy2                     = -5;
    float vtx2_MaxTrkRapidity_jf_path = 0;
    float vtx2_AvgTrkRapidity_jf_path = 0;
    float vtx2_MinTrkRapidity_jf_path = 0;
    float MaxTrkRapidity_jf_path      = 0;
    float MinTrkRapidity_jf_path      = 0;
    float AvgTrkRapidity_jf_path      = 0;

    std::vector<ElementLink<xAOD::BTagVertexContainer> > jfvertices;
    try {
      jfvertices =  bjet->auxdata<std::vector<ElementLink<xAOD::BTagVertexContainer> > >("JetFitter_JFvertices");
    } catch (...) {};

    int tmpNvtx = 0;
    int tmpNvtx1t = 0;

    bjet->taggerInfo(tmpNvtx, xAOD::JetFitter_nVTX);
    bjet->taggerInfo(tmpNvtx1t, xAOD::JetFitter_nSingleTracks); // 1 track vertices

    if (tmpNvtx > 0 || tmpNvtx1t > 0) {
      bjet->taggerInfo(jfm, xAOD::JetFitter_mass);
      bjet->taggerInfo(jfefc, xAOD::JetFitter_energyFraction);
      bjet->taggerInfo(jfdeta, xAOD::JetFitter_deltaeta);
      bjet->taggerInfo(jfdphi, xAOD::JetFitter_deltaphi);
      bjet->taggerInfo(jfntrkAtVx, xAOD::JetFitter_nTracksAtVtx);
      jfnvtx = tmpNvtx;
      bjet->taggerInfo(jfsig3d, xAOD::JetFitter_significance3d);
      jfnvtx1t = tmpNvtx1t;
      bjet->taggerInfo(jfn2t, xAOD::JetFitter_N2Tpair);

      m_branches->jet_jf_pb->push_back(bjet->JetFitter_pb());
      m_branches->jet_jf_pc->push_back(bjet->JetFitter_pc());
      m_branches->jet_jf_pu->push_back(bjet->JetFitter_pu());
      m_branches->jet_jf_llr->push_back(bjet->JetFitter_loglikelihoodratio());

      bjet->variable<float>("JetFitter", "massUncorr", massUncorr); //eloi
      bjet->variable<float>("JetFitter", "dRFlightDir", dRFlightDir); //eloi
    }
    else {
      m_branches->jet_jf_pb->push_back(-99);
      m_branches->jet_jf_pc->push_back(-99);
      m_branches->jet_jf_pu->push_back(-99);
      m_branches->jet_jf_llr->push_back(-99);
    }

    m_branches->jet_jf_VTXsize->push_back(jfvertices.size());
    m_branches->jet_jf_m->push_back(jfm);
    m_branches->jet_jf_mUncorr->push_back(massUncorr); //eloi
    m_branches->jet_jf_efc->push_back(jfefc);
    m_branches->jet_jf_deta->push_back(jfdeta);
    m_branches->jet_jf_dphi->push_back(jfdphi);
    m_branches->jet_jf_dRFlightDir->push_back(dRFlightDir); //eloi
    m_branches->jet_jf_ntrkAtVx->push_back(jfntrkAtVx);
    m_branches->jet_jf_nvtx->push_back(jfnvtx);
    m_branches->jet_jf_sig3d->push_back(jfsig3d);
    m_branches->jet_jf_nvtx1t->push_back(jfnvtx1t);
    m_branches->jet_jf_n2t->push_back(jfn2t);



    std::vector<float> j_jf_vtx_chi2;   // mod Remco
    std::vector<float> j_jf_vtx_ndf;    // mod Remco
    std::vector<int>   j_jf_vtx_ntrk;   // mod Remco
    std::vector<float> j_jf_vtx_L3d;    // mod Remco
    std::vector<float> j_jf_vtx_sig3d;  // mod Remco
    std::vector<float> j_jf_vtx_sigTrans;

    std::vector<float> j_jf_vtx_x;
    std::vector<float> j_jf_vtx_x_err;
    std::vector<float> j_jf_vtx_y;
    std::vector<float> j_jf_vtx_y_err;
    std::vector<float> j_jf_vtx_z;
    std::vector<float> j_jf_vtx_z_err;

    std::vector< ElementLink< xAOD::TrackParticleContainer > > JFTracks;

    std::vector<float> fittedPosition = m_accessors->fittedPosition(bjet);
    std::vector<float> fittedCov;
    if (m_accessors->fittedCov.isAvailable(bjet)) {
      fittedCov = m_accessors.fittedCov(bjet);
    }

    float jf_theta = 0;
    float jf_phi = 0;

    if (fittedPosition.size() > 0) {
      m_branches->PV_jf_x = fittedPosition[0]; // mod Remco
      m_branches->PV_jf_y = fittedPosition[1]; // mod Remco
      m_branches->PV_jf_z = fittedPosition[2]; // mod Remco

      jf_theta = fittedPosition[4];
      float jf_theta_err = NAN;
      if (fittedCov.size() > 4) jf_theta_err = TMath::Sqrt(fittedCov[4]);
      jf_phi = fittedPosition[3];
      float jf_phi_err = NAN;
      if (fittedCov.size() > 3) jf_phi_err = TMath::Sqrt(fittedCov[3]);

      m_branches->jet_jf_phi->push_back(jf_phi); // mod Remco
      m_branches->jet_jf_theta->push_back(jf_theta); // mod Remco
      m_branches->jet_jf_phi_err->push_back(jf_phi_err);
      m_branches->jet_jf_theta_err->push_back(jf_theta_err);

    }
    else {
      m_branches->jet_jf_phi->push_back(-999); // mod Remco
      m_branches->jet_jf_theta->push_back(-999); // mod Remco
      m_branches->jet_jf_phi_err->push_back(-999);
      m_branches->jet_jf_theta_err->push_back(-999);
    }

    TVector3 flightDir(0,0,0);
    flightDir.SetMagThetaPhi(1., jf_theta, jf_phi ); //flight directon of JF decay chain

    // the index in jfvertices for the vertices closest and second closest to primary vertex
    int secondary_vertex_index = -99;
    int tertiary_vertex_index = -99;

    for (unsigned int jfv = 0; jfv < jfvertices.size(); jfv++) {

      if (!jfvertices.at(jfv).isValid()) continue;

      const xAOD::BTagVertex *tmpVertex = *(jfvertices.at(jfv));
      const std::vector< ElementLink<xAOD::TrackParticleContainer> > tmpVect = tmpVertex->track_links(); // mod Remco
      JFTracks.insert(JFTracks.end(), tmpVect.begin(), tmpVect.end()); // mod Remco

      j_jf_vtx_chi2.push_back(tmpVertex->chi2()); // mod Remco
      j_jf_vtx_ndf.push_back(tmpVertex->NDF()); // mod Remco
      j_jf_vtx_ntrk.push_back(tmpVect.size()); // mod Remco

      if (jfv < fittedPosition.size()-5) {

        jf_theta = fittedPosition[4];
        float jf_theta_err = TMath::Sqrt(fittedCov[4]);
        jf_phi = fittedPosition[3];
        float jf_phi_err = TMath::Sqrt(fittedCov[3]);
        float jf_vtx_L3d = fittedPosition[jfv + 5];
        float jf_vtx_L3d_err = TMath::Sqrt(fittedCov[jfv + 5]);
        float jf_vtx_Transverse_err = JF_Transverse_error(jf_vtx_L3d, jf_theta, jf_theta_err, jf_phi, jf_phi_err);

        //find secondary and tertiary vertices
        if(jf_vtx_L3d > 0){
          if( jf_vtx_L3d < closestVtx_L3D || closestVtx_L3D < 0 ){

              second_closestVtx_L3D = closestVtx_L3D;
              tertiary_vertex_index = secondary_vertex_index;

              closestVtx_L3D = jf_vtx_L3d;
              secondary_vertex_index = jfv;

          }else if( jf_vtx_L3d < second_closestVtx_L3D ){

              second_closestVtx_L3D = jf_vtx_L3d;
              tertiary_vertex_index = jfv;
          }
        }

        j_jf_vtx_L3d.push_back(jf_vtx_L3d); // mod Remco

        j_jf_vtx_sig3d.push_back(jf_vtx_L3d_err); // mod Remco

        j_jf_vtx_sigTrans.push_back(jf_vtx_Transverse_err);

        std::vector<float> xyzresults = JF_xyz_errors(jf_vtx_L3d, jf_vtx_L3d_err, jf_theta, jf_theta_err, jf_phi, jf_phi_err,m_branches->PV_jf_x, m_branches->PV_jf_y, m_branches->PV_jf_z);

        j_jf_vtx_x.push_back(xyzresults[0]);
        j_jf_vtx_x_err.push_back(xyzresults[1]);
        j_jf_vtx_y.push_back(xyzresults[2]);
        j_jf_vtx_y_err.push_back(xyzresults[3]);
        j_jf_vtx_z.push_back(xyzresults[4]);
        j_jf_vtx_z_err.push_back(xyzresults[5]);
      }
      else {
        j_jf_vtx_L3d.push_back(-999); // mod Remco
        j_jf_vtx_sig3d.push_back(-999); // mod Remco
        j_jf_vtx_sigTrans.push_back(-999);

        j_jf_vtx_x.push_back(-999);
        j_jf_vtx_x_err.push_back(-999);
        j_jf_vtx_y.push_back(-999);
        j_jf_vtx_y_err.push_back(-999);
        j_jf_vtx_z.push_back(-999);
        j_jf_vtx_z_err.push_back(-999);
      }

    }

    m_branches->jet_jf_vtx_chi2->push_back(j_jf_vtx_chi2); // mod Remco
    m_branches->jet_jf_vtx_ndf->push_back(j_jf_vtx_ndf); // mod Remco
    m_branches->jet_jf_vtx_ntrk->push_back(j_jf_vtx_ntrk); // mod Remco
    m_branches->jet_jf_vtx_L3d->push_back(j_jf_vtx_L3d); // mod Remco
    m_branches->jet_jf_vtx_sig3d->push_back(j_jf_vtx_sig3d); // mod Remco
    m_branches->jet_jf_vtx_sigTrans->push_back(j_jf_vtx_sigTrans);
    m_branches->jet_jf_vtx_x->push_back(j_jf_vtx_x);
    m_branches->jet_jf_vtx_x_err->push_back(j_jf_vtx_x_err);
    m_branches->jet_jf_vtx_y->push_back(j_jf_vtx_y);
    m_branches->jet_jf_vtx_y_err->push_back(j_jf_vtx_y_err);
    m_branches->jet_jf_vtx_z->push_back(j_jf_vtx_z);
    m_branches->jet_jf_vtx_z_err->push_back(j_jf_vtx_z_err);

    std::vector< ElementLink< xAOD::TrackParticleContainer > > assocTracks;
    std::vector<const xAOD::TrackParticle*> selectedTracks; // tracks passing number of Pixel and SCT hits requirements

    assocTracks = bjet->auxdata<std::vector<ElementLink<xAOD::TrackParticleContainer> > >("BTagTrackToJetAssociator");

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

    //track loop variables
    std::vector<int> j_trk_jf_Vertex;
    TLorentzVector tracksTot4Mom(0,0,0,0);
    TLorentzVector tracksTot4Mom_firstVtx(0,0,0,0);
    TLorentzVector tracksTot4Mom_secondVtx(0,0,0,0);

    float sumTrackRapidity = 0;
    float vtx1_sumTrackRapidity = 0;
    int vtx1_first_track =0;
    float vtx2_sumTrackRapidity = 0;
    int vtx2_first_track =0;
    float track_mass = 139.570;
    int trkIndex=0;


    //track loop

    for (const auto* tmpTrk: selectedTracks) {

      int myVtx = -1;
      for (unsigned int jfv = 0; jfv < jfvertices.size(); jfv++) { // mod Remco

        if (!jfvertices.at(jfv).isValid()) continue;
        const xAOD::BTagVertex *tmpVertex = *(jfvertices.at(jfv));
        const std::vector< ElementLink<xAOD::TrackParticleContainer> > tmpVect = tmpVertex->track_links();

        if (particleInCollection(tmpTrk, tmpVect)) myVtx = jfv;

      }
      j_trk_jf_Vertex.push_back(myVtx);


      TLorentzVector trk;
      trk.SetPtEtaPhiM(tmpTrk->pt(), tmpTrk->eta(), tmpTrk->phi(), track_mass);
      tracksTot4Mom += trk;

      TVector3 trkvector(0,0,0);
      trkvector = trk.Vect();

      float trackRapidity = (trkvector.Mag2()>0 ? tan( 0.5*trkvector.Angle(flightDir) ) : 0); // steps to protect against log(0)

      trackRapidity = (trackRapidity < 0.000001 ? (-1)*log(0.000001) : (-1)*log(trackRapidity) ); // value of 0.000001 should provide enough margin for typical values of trackRapidity

      sumTrackRapidity += trackRapidity;

      if(trkIndex==0){
        MaxTrkRapidity_jf_path = trackRapidity;
        MinTrkRapidity_jf_path = trackRapidity;
      }else{
        MaxTrkRapidity_jf_path = trackRapidity > MaxTrkRapidity_jf_path ? trackRapidity : MaxTrkRapidity_jf_path;
        MinTrkRapidity_jf_path = trackRapidity < MinTrkRapidity_jf_path ? trackRapidity : MinTrkRapidity_jf_path;
      }

      if(myVtx == secondary_vertex_index){
        nTrk_vtx1 += 1;

        tracksTot4Mom_firstVtx += trk;
        vtx1_sumTrackRapidity += trackRapidity;
        if(!vtx1_first_track){
          vtx1_MaxTrkRapidity_jf_path = trackRapidity;
          vtx1_MinTrkRapidity_jf_path = trackRapidity;
          vtx1_first_track=1;
        }else{
          vtx1_MaxTrkRapidity_jf_path = trackRapidity > vtx1_MaxTrkRapidity_jf_path ? trackRapidity : vtx1_MaxTrkRapidity_jf_path;
          vtx1_MinTrkRapidity_jf_path = trackRapidity < vtx1_MinTrkRapidity_jf_path ? trackRapidity : vtx1_MinTrkRapidity_jf_path;
        }
      }
      if(myVtx == tertiary_vertex_index){
        nTrk_vtx2 += 1;

        tracksTot4Mom_secondVtx += trk;
        vtx2_sumTrackRapidity += trackRapidity;
        if(!vtx2_first_track){
          vtx2_MaxTrkRapidity_jf_path = trackRapidity;
          vtx2_MinTrkRapidity_jf_path = trackRapidity;
          vtx2_first_track=1;
        }else{
          vtx2_MaxTrkRapidity_jf_path = trackRapidity > vtx2_MaxTrkRapidity_jf_path ? trackRapidity : vtx2_MaxTrkRapidity_jf_path;
          vtx2_MinTrkRapidity_jf_path = trackRapidity < vtx2_MinTrkRapidity_jf_path ? trackRapidity : vtx2_MinTrkRapidity_jf_path;
        }
      }


      trkIndex++;
    } // end track loop


    AvgTrkRapidity_jf_path = trkIndex > 0 ? sumTrackRapidity/trkIndex : 0;

    if(nTrk_vtx1 > 0){
      JF_Lxy1 = closestVtx_L3D*sin(jf_theta);
      mass_first_vtx = tracksTot4Mom_firstVtx.M();
      e_first_vtx = tracksTot4Mom_firstVtx.E();
      e_frac_vtx1 = e_first_vtx/tracksTot4Mom.E();
      vtx1_AvgTrkRapidity_jf_path = vtx1_sumTrackRapidity/nTrk_vtx1;
    }


    if(nTrk_vtx2 > 0){
      JF_Lxy2 = second_closestVtx_L3D*sin(jf_theta);
      mass_second_vtx = tracksTot4Mom_secondVtx.M();
      e_second_vtx = tracksTot4Mom_secondVtx.E();
      e_frac_vtx2 = e_second_vtx/tracksTot4Mom.E();
      vtx2_AvgTrkRapidity_jf_path = vtx2_sumTrackRapidity/nTrk_vtx2;
    }


    m_branches->v_jet_trk_jf_Vertex->push_back(j_trk_jf_Vertex);

    m_branches->nTrk_vtx1->push_back(nTrk_vtx1);
    m_branches->mass_first_vtx->push_back(mass_first_vtx);
    m_branches->e_first_vtx->push_back(e_first_vtx);
    m_branches->e_frac_vtx1->push_back(e_frac_vtx1);
    m_branches->closestVtx_L3D->push_back(closestVtx_L3D);
    m_branches->JF_Lxy1->push_back(JF_Lxy1);
    m_branches->vtx1_MaxTrkRapidity_jf_path->push_back(vtx1_MaxTrkRapidity_jf_path);
    m_branches->vtx1_AvgTrkRapidity_jf_path->push_back(vtx1_AvgTrkRapidity_jf_path);
    m_branches->vtx1_MinTrkRapidity_jf_path->push_back(vtx1_MinTrkRapidity_jf_path);
    m_branches->nTrk_vtx2->push_back(nTrk_vtx2);
    m_branches->mass_second_vtx->push_back(mass_second_vtx);
    m_branches->e_second_vtx->push_back(e_second_vtx);
    m_branches->e_frac_vtx2->push_back(e_frac_vtx2);
    m_branches->second_closestVtx_L3D->push_back(second_closestVtx_L3D);
    m_branches->JF_Lxy2->push_back(JF_Lxy2);
    m_branches->vtx2_MaxTrkRapidity_jf_path->push_back(vtx2_MaxTrkRapidity_jf_path);
    m_branches->vtx2_AvgTrkRapidity_jf_path->push_back(vtx2_AvgTrkRapidity_jf_path);
    m_branches->vtx2_MinTrkRapidity_jf_path->push_back(vtx2_MinTrkRapidity_jf_path);
    m_branches->MaxTrkRapidity_jf_path->push_back(MaxTrkRapidity_jf_path);
    m_branches->MinTrkRapidity_jf_path->push_back(MinTrkRapidity_jf_path);
    m_branches->AvgTrkRapidity_jf_path->push_back(AvgTrkRapidity_jf_path);
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void JetFitterBranches::clear() {
  // clear vectors

  m_branches->PV_jf_x = -999;
  m_branches->PV_jf_y = -999;
  m_branches->PV_jf_z = -999;

  m_branches->v_jet_trk_jf_Vertex->clear();
  m_branches->jet_jf_pb->clear();
  m_branches->jet_jf_pc->clear();
  m_branches->jet_jf_pu->clear();
  m_branches->jet_jf_llr->clear();
  m_branches->jet_jf_m->clear();
  m_branches->jet_jf_mUncorr->clear();
  m_branches->jet_jf_efc->clear();
  m_branches->jet_jf_deta->clear();
  m_branches->jet_jf_dphi->clear();
  m_branches->jet_jf_dRFlightDir->clear();
  m_branches->jet_jf_ntrkAtVx->clear();
  m_branches->jet_jf_nvtx->clear();
  m_branches->jet_jf_sig3d->clear();
  m_branches->jet_jf_nvtx1t->clear();
  m_branches->jet_jf_n2t->clear();
  m_branches->jet_jf_VTXsize->clear();
  m_branches->jet_jf_vtx_chi2->clear();
  m_branches->jet_jf_vtx_ndf->clear();
  m_branches->jet_jf_vtx_ntrk->clear();
  m_branches->jet_jf_vtx_L3d->clear();
  m_branches->jet_jf_vtx_sig3d->clear();
  m_branches->jet_jf_phi->clear();
  m_branches->jet_jf_theta->clear();
  m_branches->jet_jf_vtx_sigTrans->clear();
  m_branches->jet_jf_vtx_x->clear();
  m_branches->jet_jf_vtx_x_err->clear();
  m_branches->jet_jf_vtx_y->clear();
  m_branches->jet_jf_vtx_y_err->clear();
  m_branches->jet_jf_vtx_z->clear();
  m_branches->jet_jf_vtx_z_err->clear();
  m_branches->jet_jf_phi_err->clear();
  m_branches->jet_jf_theta_err->clear();
  
  m_branches->nTrk_vtx1->clear();
  m_branches->mass_first_vtx->clear();
  m_branches->e_first_vtx->clear();
  m_branches->e_frac_vtx1->clear();
  m_branches->closestVtx_L3D->clear();
  m_branches->JF_Lxy1->clear();
  m_branches->vtx1_MaxTrkRapidity_jf_path->clear();
  m_branches->vtx1_AvgTrkRapidity_jf_path->clear();
  m_branches->vtx1_MinTrkRapidity_jf_path->clear();
  m_branches->nTrk_vtx2->clear();
  m_branches->mass_second_vtx->clear();
  m_branches->e_second_vtx->clear();
  m_branches->e_frac_vtx2->clear();
  m_branches->second_closestVtx_L3D->clear();
  m_branches->JF_Lxy2->clear();
  m_branches->vtx2_MaxTrkRapidity_jf_path->clear();
  m_branches->vtx2_AvgTrkRapidity_jf_path->clear();
  m_branches->vtx2_MinTrkRapidity_jf_path->clear();
  m_branches->MaxTrkRapidity_jf_path->clear();
  m_branches->MinTrkRapidity_jf_path->clear();
  m_branches->AvgTrkRapidity_jf_path->clear();
}

bool JetFitterBranches :: particleInCollection( const xAOD::TrackParticle *trkPart, std::vector< ElementLink< xAOD::TrackParticleContainer > > trkColl ) {
  for (unsigned int iT = 0; iT < trkColl.size(); iT++) {
    if (trkPart == *(trkColl.at(iT))) return true;
  }
  return false;
}


float JetFitterBranches :: JF_Transverse_error(float L3D, float Theta, float Theta_err, float Phi, float Phi_err){
  TVector3 vertexPos;
  vertexPos.SetMagThetaPhi(L3D,Theta,Phi);

  TVector3 vertexPos2;
  vertexPos2.SetMagThetaPhi(L3D,Theta+Theta_err,Phi);
  float Theta_err1 = fabs(L3D*TMath::Tan(vertexPos.Angle(vertexPos2)));

  vertexPos2.SetMagThetaPhi(L3D,Theta-Theta_err,Phi);
  float Theta_err2 = fabs(L3D*TMath::Tan(vertexPos.Angle(vertexPos2)));

  vertexPos2.SetMagThetaPhi(L3D,Theta,Phi+Phi_err);
  float Phi_err1 = fabs(L3D*TMath::Tan(vertexPos.Angle(vertexPos2)));

  vertexPos2.SetMagThetaPhi(L3D,Theta,Phi-Phi_err);
  float Phi_err2 = fabs(L3D*TMath::Tan(vertexPos.Angle(vertexPos2)));

  float transverse_Theta_error = std::max(Theta_err1,Theta_err2);
  float transverse_Phi_error = std::max(Phi_err1,Phi_err2);

  float transverse_err = TMath::Sqrt(transverse_Theta_error*transverse_Theta_error+transverse_Phi_error*transverse_Phi_error);

  return transverse_err;

}

std::vector<float> JetFitterBranches :: JF_xyz_errors(float L3D, float L3Derr, float Theta, float Theta_err, float Phi, float Phi_err,float Pv_x, float Pv_y, float Pv_z){

    TVector3 vertexPos;
    TVector3 vertexPos2;
    vertexPos.SetMagThetaPhi(L3D,Theta,Phi);

    //this is the relative position to primary vertex
    float x = vertexPos.X();
    float y = vertexPos.Y();
    float z = vertexPos.Z();

    vertexPos.SetMagThetaPhi(L3D+L3Derr,Theta,Phi);
    vertexPos2.SetMagThetaPhi(L3D-L3Derr,Theta,Phi);

    float L3D_x_err = std::max(fabs(vertexPos.X()-x),fabs(vertexPos2.X()-x));
    float L3D_y_err = std::max(fabs(vertexPos.Y()-y),fabs(vertexPos2.Y()-y));
    float L3D_z_err = std::max(fabs(vertexPos.Z()-z),fabs(vertexPos2.Z()-z));

    vertexPos.SetMagThetaPhi(L3D,Theta+Theta_err,Phi);
    vertexPos2.SetMagThetaPhi(L3D,Theta-Theta_err,Phi);

    float Theta_x_err = std::max(fabs(vertexPos.X()-x),fabs(vertexPos2.X()-x));
    float Theta_y_err = std::max(fabs(vertexPos.Y()-y),fabs(vertexPos2.Y()-y));
    float Theta_z_err = std::max(fabs(vertexPos.Z()-z),fabs(vertexPos2.Z()-z));

    vertexPos.SetMagThetaPhi(L3D,Theta,Phi+Phi_err);
    vertexPos2.SetMagThetaPhi(L3D,Theta,Phi-Phi_err);

    float Phi_x_err = std::max(fabs(vertexPos.X()-x),fabs(vertexPos2.X()-x));
    float Phi_y_err = std::max(fabs(vertexPos.Y()-y),fabs(vertexPos2.Y()-y));
    float Phi_z_err = std::max(fabs(vertexPos.Z()-z),fabs(vertexPos2.Z()-z));

    float x_err = TMath::Sqrt(L3D_x_err*L3D_x_err+Theta_x_err*Theta_x_err+Phi_x_err*Phi_x_err);
    float y_err = TMath::Sqrt(L3D_y_err*L3D_y_err+Theta_y_err*Theta_y_err+Phi_y_err*Phi_y_err);
    float z_err = TMath::Sqrt(L3D_z_err*L3D_z_err+Theta_z_err*Theta_z_err+Phi_z_err*Phi_z_err);

    //this is the x,y,z position relative to (0,0,0)
    x = Pv_x+x;
    y = Pv_y+y;
    z = Pv_z+z;

    std::vector<float> results;

    results.push_back(x);
    results.push_back(x_err);
    results.push_back(y);
    results.push_back(y_err);
    results.push_back(z);
    results.push_back(z_err);

    return results;
}


