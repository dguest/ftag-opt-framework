#ifndef JETFITTER_BRANCH_BUFFER_HH
#define JETFITTER_BRANCH_BUFFER_HH

#include <vector>

struct JetFitterBranchBuffer {

 // JetFitter
  float PV_jf_x; //mod Remco
  float PV_jf_y; //mod Remco
  float PV_jf_z; //mod Remco

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

  // JetFitterCombNN
  std::vector<float> *jet_jfcombnn_pb;
  std::vector<float> *jet_jfcombnn_pc;
  std::vector<float> *jet_jfcombnn_pu;
  std::vector<float> *jet_jfcombnn_llr;

};

#endif // JETFITTER_BRANCH_BUFFER_HH
