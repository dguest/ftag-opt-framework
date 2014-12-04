#ifndef BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
#define BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H 1

#include "AthenaBaseComps/AthHistogramAlgorithm.h"
#include "GaudiKernel/ToolHandle.h" 
//#include "TrkExInterfaces/IExtrapolator.h"

#include "TFile.h"
#include "TTree.h"

#ifndef __MAKECINT__
#include "xAODTracking/TrackParticle.h"
#include "xAODTruth/TruthParticle.h"
#endif // not __MAKECINT__

// forward declarations
class IJetSelector;
class IJetCalibrationTool;

enum TAGGERALGO{ IP3D=0,
		 SV1,
		 JF }; 

enum TRKORIGIN{ PUFAKE=-1,
		FROMB,
		FROMC,
		FRAG,
		GEANT }; 

class btagIBLAnalysisAlg: public ::AthHistogramAlgorithm { 
 public: 
  btagIBLAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~btagIBLAnalysisAlg(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();

  TFile* output;
  TTree* tree;

  // general event info
  int runnumber;
  int eventnumber;
  int mcchannel;
  double mcweight;
  int npv;
  double mu;
  double PV_x;
  double PV_y;
  double PV_z;

  // jet info
  int njets;
  int nbjets;
  int nbjets_q;
  int nbjets_HadI;
  int nbjets_HadF;
  std::vector<float> *v_jet_pt;
  std::vector<float> *v_jet_eta;
  std::vector<float> *v_jet_phi;
  std::vector<float> *v_jet_E;
  std::vector<float> *v_jet_m;
  std::vector<int> *v_jet_truthflav;
  std::vector<int> *v_jet_GhostL_q;
  std::vector<int> *v_jet_GhostL_HadI;
  std::vector<int> *v_jet_GhostL_HadF;
  std::vector<int> *v_jet_aliveAfterOR;
  std::vector<int> *v_jet_truthMatch;
  std::vector<float> *v_jet_truthPt;
  std::vector<float> *v_jet_dRiso;
  // to add: jvf


  // IP2D
  std::vector<float> *v_jet_ip2d_pb;
  std::vector<float> *v_jet_ip2d_pc;
  std::vector<float> *v_jet_ip2d_pu;
  std::vector<float> *v_jet_ip2d_llr;

  // IP3D
  std::vector<float> *v_jet_ip3d_pb;
  std::vector<float> *v_jet_ip3d_pc;
  std::vector<float> *v_jet_ip3d_pu;
  std::vector<float> *v_jet_ip3d_llr;

  // SV0
  std::vector<float> *v_jet_sv0_sig3d;
  std::vector<int> *v_jet_sv0_ntrkj;
  std::vector<int> *v_jet_sv0_ntrkv;
  std::vector<int> *v_jet_sv0_n2t;
  std::vector<float> *v_jet_sv0_m;
  std::vector<float> *v_jet_sv0_efc;
  std::vector<float> *v_jet_sv0_normdist;
  std::vector<std::vector<float> > *v_jet_sv0_vtxx;
  std::vector<std::vector<float> > *v_jet_sv0_vtxy;
  std::vector<std::vector<float> > *v_jet_sv0_vtxz;

  // SV1
  std::vector<int> *v_jet_sv1_ntrkj;
  std::vector<int> *v_jet_sv1_ntrkv;
  std::vector<int> *v_jet_sv1_n2t;
  std::vector<float> *v_jet_sv1_m;
  std::vector<float> *v_jet_sv1_efc;
  std::vector<float> *v_jet_sv1_normdist;
  std::vector<float> *v_jet_sv1_pb;
  std::vector<float> *v_jet_sv1_pc;
  std::vector<float> *v_jet_sv1_pu;
  std::vector<float> *v_jet_sv1_llr;
  std::vector<std::vector<float> > *v_jet_sv1_vtxx;
  std::vector<std::vector<float> > *v_jet_sv1_vtxy;
  std::vector<std::vector<float> > *v_jet_sv1_vtxz;

  // JetFitter
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
  std::vector<int> *v_jet_jf_nvtx1t;
  std::vector<int> *v_jet_jf_n2t;
  std::vector<std::vector<float> > *v_jet_jf_chi2;
  std::vector<std::vector<float> > *v_jet_jf_ndf;

  // JetFitterCombNN
  std::vector<float> *v_jet_jfcombnn_pb;
  std::vector<float> *v_jet_jfcombnn_pc;
  std::vector<float> *v_jet_jfcombnn_pu;
  std::vector<float> *v_jet_jfcombnn_llr;

  // Other
  std::vector<double> *v_jet_sv1ip3d;
  std::vector<double> *v_jet_mv1;
  std::vector<double> *v_jet_mv1c;
  std::vector<double> *v_jet_mv2c00;
  std::vector<double> *v_jet_mv2c10;
  std::vector<double> *v_jet_mv2c20;
  std::vector<double> *v_jet_mvb;

  // B hadron
  std::vector<float> *v_bH_pt;
  std::vector<float> *v_bH_eta;
  std::vector<float> *v_bH_phi;
  std::vector<float> *v_bH_Lxy;
  std::vector<float> *v_bH_dRjet;

  // track info
  std::vector<int>   *v_jet_btag_ntrk;
  
  std::vector<std::vector<float> > *v_jet_trk_pt;
  std::vector<std::vector<float> > *v_jet_trk_eta;
  std::vector<std::vector<float> > *v_jet_trk_theta;
  std::vector<std::vector<float> > *v_jet_trk_phi;
  std::vector<std::vector<float> > *v_jet_trk_chi2;
  std::vector<std::vector<float> > *v_jet_trk_ndf;

  std::vector<std::vector<int> > *v_jet_trk_algo;
  std::vector<std::vector<int> > *v_jet_trk_orig;

  std::vector<std::vector<int> > *v_jet_trk_nBLHits;
  std::vector<std::vector<int> > *v_jet_trk_nsharedBLHits;
  std::vector<std::vector<int> > *v_jet_trk_nsplitBLHits;
  std::vector<std::vector<int> > *v_jet_trk_nPixHits;
  std::vector<std::vector<int> > *v_jet_trk_nsharedPixHits;
  std::vector<std::vector<int> > *v_jet_trk_nsplitPixHits;
  std::vector<std::vector<int> > *v_jet_trk_nSCTHits;
  std::vector<std::vector<int> > *v_jet_trk_expectBLayerHit;
  std::vector<std::vector<float> > *v_jet_trk_d0;
  std::vector<std::vector<float> > *v_jet_trk_z0;
  std::vector<std::vector<float> > *v_jet_trk_d0_truth;
  std::vector<std::vector<float> > *v_jet_trk_z0_truth;
 
  std::vector<std::vector<int> >   *v_jet_trk_IP3D_grade;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_d0;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_z0;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_d0sig;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_z0sig;
  
  // those are just quick accessors
  std::vector<int>   *v_jet_sv1_ntrk;
  std::vector<int>   *v_jet_ip3d_ntrk;
  std::vector<int>   *v_jet_jf_ntrk;

  void clearvectors();

#ifndef __MAKECINT__
  const xAOD::TruthParticle*  truthParticle(const xAOD::TrackParticle *trkPart) const;
  void GetParentTracks(const xAOD::TruthParticle* part, 
		       std::vector<const xAOD::TruthParticle*> &tracksFromB, 
		       std::vector<const xAOD::TruthParticle*> &tracksFromC, 
		       bool isfromC, std::string indent);
  bool decorateTruth(const xAOD::TruthParticle & particle);
#endif // not __MAKECINT__  


 private: 

  /// tool handle for jet cleaning tool
  ToolHandle< IJetSelector > m_jetCleaningTool;

  /// tool handle for jet calibration tool
  ToolHandle< IJetCalibrationTool > m_jetCalibrationTool;

  // determine whether particle is B hadron or not
  bool isBHadron(int pdgid);

  // compute dR between two objects
  float deltaR(float eta1, float eta2, float phi1, float phi2);

  //ToolHandle<IExtrapolator> m_extrapolator; //Trk::
  
}; 

#endif //> !BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
