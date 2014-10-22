#ifndef BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
#define BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H 1

#include "AthenaBaseComps/AthHistogramAlgorithm.h"

#include "TFile.h"
#include "TTree.h"

class btagIBLAnalysisAlg: public ::AthHistogramAlgorithm { 
 public: 
  btagIBLAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~btagIBLAnalysisAlg(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();


 private: 

  bool isBHadron(int pdgid);

  TFile* output;
  TTree* tree;

  std::vector<float> *v_jet_pt;
  std::vector<float> *v_jet_eta;
  std::vector<float> *v_jet_phi;
  std::vector<int> *v_jet_truthflav;

  std::vector<float> *v_jet_ip2d_pb;
  std::vector<float> *v_jet_ip2d_pc;
  std::vector<float> *v_jet_ip2d_pu;
  std::vector<float> *v_jet_ip2d_llr;

  std::vector<float> *v_jet_ip3d_pb;
  std::vector<float> *v_jet_ip3d_pc;
  std::vector<float> *v_jet_ip3d_pu;
  std::vector<float> *v_jet_ip3d_llr;

  std::vector<float> *v_jet_sv0_sig3d;
  std::vector<float> *v_jet_sv0_ntrkv;
  std::vector<float> *v_jet_sv0_m;
  std::vector<float> *v_jet_sv0_efc;
  std::vector<float> *v_jet_sv0_n2t;

  std::vector<float> *v_jet_sv1_pb;
  std::vector<float> *v_jet_sv1_pc;
  std::vector<float> *v_jet_sv1_pu;
  std::vector<float> *v_jet_sv1_llr;

  std::vector<float> *v_jet_jf_pb;
  std::vector<float> *v_jet_jf_pc;
  std::vector<float> *v_jet_jf_pu;
  std::vector<float> *v_jet_jf_llr;
  std::vector<float> *v_jet_jf_m;
  std::vector<float> *v_jet_jf_efc;
  std::vector<float> *v_jet_jf_deta;
  std::vector<float> *v_jet_jf_dphi;
  std::vector<float> *v_jet_jf_ntrkAtVx;
  std::vector<int> *v_jet_jf_nvtx;
  std::vector<int> *v_jet_jf_sig3d;

  std::vector<float> *v_jet_jfcombnn_pb;
  std::vector<float> *v_jet_jfcombnn_pc;
  std::vector<float> *v_jet_jfcombnn_pu;
  std::vector<float> *v_jet_jfcombnn_llr;

  std::vector<double> *v_jet_sv1ip3d_discr;
  std::vector<double> *v_jet_mv1_discr;

  std::vector<float> *v_bH_pt;
  std::vector<float> *v_bH_eta;
  std::vector<float> *v_bH_phi;
  std::vector<float> *v_bH_Lxy;
  //std::vector<float> *v_jet_Lxy_denom;

}; 

#endif //> !BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
