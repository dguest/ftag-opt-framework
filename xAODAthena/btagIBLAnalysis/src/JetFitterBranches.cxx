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
JetFitterBranches::JetFitterBranches():
  m_branches(new JetFitterBranchBuffer)
{
  // instantiate all the vectors here ...

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

  m_branches->jet_jfcombnn_pb     = new std::vector<float>();
  m_branches->jet_jfcombnn_pc     = new std::vector<float>();
  m_branches->jet_jfcombnn_pu     = new std::vector<float>();
  m_branches->jet_jfcombnn_llr    = new std::vector<float>();


}

//!-----------------------------------------------------------------------------------------------------------------------------!//
JetFitterBranches::~JetFitterBranches() {
  // delete all the vectors here ...
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
  delete m_branches->jet_jfcombnn_pb;
  delete m_branches->jet_jfcombnn_pc;
  delete m_branches->jet_jfcombnn_pu;
  delete m_branches->jet_jfcombnn_llr;

  delete m_branches;
}

void JetFitterBranches::set_tree(TTree& output_tree) const {

  output_tree.Branch("PV_jf_x"            , &m_branches->PV_jf_x); // mod Remco
  output_tree.Branch("PV_jf_y"            , &m_branches->PV_jf_y); // mod Remco
  output_tree.Branch("PV_jf_z"            , &m_branches->PV_jf_z); // mod Remco
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
  output_tree.Branch("jet_jfcombnn_pb"    , &m_branches->jet_jfcombnn_pb);
  output_tree.Branch("jet_jfcombnn_pc"    , &m_branches->jet_jfcombnn_pc);
  output_tree.Branch("jet_jfcombnn_pu"    , &m_branches->jet_jfcombnn_pu);
  output_tree.Branch("jet_jfcombnn_llr"   , &m_branches->jet_jfcombnn_llr);

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

    // JetFitterCombNN
    m_branches->jet_jfcombnn_pb->push_back(bjet->JetFitterCombNN_pb());
    m_branches->jet_jfcombnn_pc->push_back(bjet->JetFitterCombNN_pc());
    m_branches->jet_jfcombnn_pu->push_back(bjet->JetFitterCombNN_pu());
    m_branches->jet_jfcombnn_llr->push_back(bjet->JetFitterCombNN_loglikelihoodratio());



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

    std::vector<float> fittedPosition = bjet->auxdata<std::vector<float> >("JetFitter_fittedPosition"); // mod Remco
    std::vector<float> fittedCov = bjet->auxdata<std::vector<float> >("JetFitter_fittedCov"); // mod Remco

    if (fittedPosition.size() > 0) {
      m_branches->PV_jf_x = fittedPosition[0]; // mod Remco
      m_branches->PV_jf_y = fittedPosition[1]; // mod Remco
      m_branches->PV_jf_z = fittedPosition[2]; // mod Remco

      float jf_theta = fittedPosition[4];
      float jf_theta_err = TMath::Sqrt(fittedCov[4]);
      float jf_phi = fittedPosition[3];
      float jf_phi_err = TMath::Sqrt(fittedCov[3]);

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


    for (unsigned int jfv = 0; jfv < jfvertices.size(); jfv++) {

      if (!jfvertices.at(jfv).isValid()) continue;

      const xAOD::BTagVertex *tmpVertex = *(jfvertices.at(jfv));
      const std::vector< ElementLink<xAOD::TrackParticleContainer> > tmpVect = tmpVertex->track_links(); // mod Remco
      JFTracks.insert(JFTracks.end(), tmpVect.begin(), tmpVect.end()); // mod Remco

      j_jf_vtx_chi2.push_back(tmpVertex->chi2()); // mod Remco
      j_jf_vtx_ndf.push_back(tmpVertex->NDF()); // mod Remco
      j_jf_vtx_ntrk.push_back(tmpVect.size()); // mod Remco

      if (jfv < fittedPosition.size()-5) {

        float jf_theta = fittedPosition[4];
        float jf_theta_err = TMath::Sqrt(fittedCov[4]);
        float jf_phi = fittedPosition[3];
        float jf_phi_err = TMath::Sqrt(fittedCov[3]);
        float jf_vtx_L3d = fittedPosition[jfv + 5];
        float jf_vtx_L3d_err = TMath::Sqrt(fittedCov[jfv + 5]);
        float jf_vtx_Transverse_err = JF_Transverse_error(jf_vtx_L3d, jf_theta, jf_theta_err, jf_phi, jf_phi_err);

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


}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void JetFitterBranches::clear() {
  // clear vectors

  m_branches->PV_jf_x = -999;
  m_branches->PV_jf_y = -999;
  m_branches->PV_jf_z = -999;

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
  m_branches->jet_jfcombnn_pb->clear();
  m_branches->jet_jfcombnn_pc->clear();
  m_branches->jet_jfcombnn_pu->clear();
  m_branches->jet_jfcombnn_llr->clear();
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

// void JetFitterBranches :: GetAllChildren(const xAOD::TruthParticle* particle,
//                                            std::vector<const xAOD::TruthParticle*> &tracksFromB,
//                                            std::vector<const xAOD::TruthParticle*> &tracksFromC,
//                                            bool isFromC){


//   if(!particle->hasDecayVtx()) return;

//   const xAOD::TruthVertex* decayvtx = particle->decayVtx();

//   for(unsigned i=0; i< decayvtx->nOutgoingParticles(); i++){

//      const xAOD::TruthParticle* child = decayvtx->outgoingParticle(i);

//      if (child->barcode() > 200e3) continue;
//      if ( !child->isCharmHadron() && !child->isBottomHadron() ){

//         if(child->isCharged()){ tracksFromB.push_back(child);}
//         if(child->isCharged() && isFromC){
//           tracksFromC.push_back(child);
//         }
//      }

//      if (isFromC) GetAllChildren(child, tracksFromB, tracksFromC, true);
//      else GetAllChildren(child, tracksFromB, tracksFromC, child->isCharmHadron() );


//   }

// }


// std::vector<int> JetFitterBranches :: getDRSortedIndices(std::vector<const xAOD::IParticle*> ghostHads, const xAOD::Jet *jet){
//     std::vector<float> dRofhadrons;

//     for(unsigned int ip = 0; ip < ghostHads.size(); ip++){
//       float dEta = (ghostHads.at(ip))->eta() - (jet->eta()) ;
//       float dPhi = acos(cos( fabs( (ghostHads.at(ip))->phi()-jet->phi() ) ) );
//       float dr = sqrt(pow(dEta, 2) + pow(dPhi, 2));
//       dRofhadrons.push_back(dr);
//     }

//     std::vector<int> y(dRofhadrons.size());
//     std::size_t n(0);
//     std::generate(std::begin(y), std::end(y), [&]{ return n++; });
//     std::sort(std::begin(y),std::end(y),[&](int i1, int i2) { return dRofhadrons[i1] < dRofhadrons[i2]; });

//     return y;
// }
