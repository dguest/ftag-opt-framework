#ifndef JETFITTER_BRANCH_BUFFER_HH
#define JETFITTER_BRANCH_BUFFER_HH

#include <vector>

struct JetFitterAccessors {
  JetFitterAccessors();
  SG::AuxElement::ConstAccessor<std::vector<float> > fittedCov;
  SG::AuxElement::ConstAccessor<std::vector<float> > fittedPosition;
};

struct JetFitterBranchBuffer {

 // JetFitter
  float PV_jf_x; //mod Remco
  float PV_jf_y; //mod Remco
  float PV_jf_z; //mod Remco

  std::vector<std::vector<int> > *v_jet_trk_jf_Vertex;

  std::vector<float> *jet_jf_pb;
  std::vector<float> *jet_jf_pc;
  std::vector<float> *jet_jf_pu;
  std::vector<float> *jet_jf_llr;
  std::vector<float> *jet_jf_m;
  std::vector<float> *jet_jf_mUncorr; //eloi
  std::vector<float> *jet_jf_efc;
  std::vector<float> *jet_jf_deta;
  std::vector<float> *jet_jf_dphi;
  std::vector<float> *jet_jf_dRFlightDir; //eloi
  std::vector<float> *jet_jf_ntrkAtVx;
  std::vector<int> *jet_jf_nvtx;
  std::vector<float> *jet_jf_sig3d;
  std::vector<int> *jet_jf_nvtx1t;
  std::vector<int> *jet_jf_n2t;
  std::vector<int> *jet_jf_VTXsize;
  std::vector<std::vector<float> > *jet_jf_vtx_chi2; //mod Remco
  std::vector<std::vector<float> > *jet_jf_vtx_ndf; //mod Remco
  std::vector<std::vector<int> > *jet_jf_vtx_ntrk; //mod Remco
  std::vector<std::vector<float> > *jet_jf_vtx_L3d; //mod Remco
  std::vector<std::vector<float> > *jet_jf_vtx_sig3d; //mod Remco
  std::vector<std::vector<float> > *jet_jf_vtx_sigTrans;
  std::vector<std::vector<float> > *jet_jf_vtx_x;
  std::vector<std::vector<float> > *jet_jf_vtx_x_err;
  std::vector<std::vector<float> > *jet_jf_vtx_y;
  std::vector<std::vector<float> > *jet_jf_vtx_y_err;
  std::vector<std::vector<float> > *jet_jf_vtx_z;
  std::vector<std::vector<float> > *jet_jf_vtx_z_err;

  std::vector<float> *jet_jf_phi; //mod Remco
  std::vector<float> *jet_jf_phi_err;
  std::vector<float> *jet_jf_theta; //mod Remco
  std::vector<float> *jet_jf_theta_err;

  //additional variables used by MV2cl100 and MV2c100 (for c-tagging)

  // JF vertex closest to primary
  std::vector<int>   *nTrk_vtx1;
  std::vector<float> *mass_first_vtx;
  std::vector<float> *e_first_vtx;
  std::vector<float> *e_frac_vtx1;
  std::vector<float> *closestVtx_L3D;
  std::vector<float> *JF_Lxy1;
  std::vector<float> *vtx1_MaxTrkRapidity_jf_path;
  std::vector<float> *vtx1_AvgTrkRapidity_jf_path;
  std::vector<float> *vtx1_MinTrkRapidity_jf_path;

  // JF vertex second closest to primary
  std::vector<int>   *nTrk_vtx2;
  std::vector<float> *mass_second_vtx;
  std::vector<float> *e_second_vtx;
  std::vector<float> *e_frac_vtx2;
  std::vector<float> *second_closestVtx_L3D;
  std::vector<float> *JF_Lxy2;
  std::vector<float> *vtx2_MaxTrkRapidity_jf_path;
  std::vector<float> *vtx2_AvgTrkRapidity_jf_path;
  std::vector<float> *vtx2_MinTrkRapidity_jf_path;

  // for all tracks in the jet
  std::vector<float> *MaxTrkRapidity_jf_path;
  std::vector<float> *MinTrkRapidity_jf_path;
  std::vector<float> *AvgTrkRapidity_jf_path;


};

#endif // JETFITTER_BRANCH_BUFFER_HH
