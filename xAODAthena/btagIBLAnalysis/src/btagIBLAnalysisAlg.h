#ifndef BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
#define BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H 1

// Additions from Dan: classes to manage branches
#include "ClusterBranches.hh"
#include "SubjetBranches.hh"
#include "TrackCovBranches.hh"
#include "TrackBranches.hh"
#include "BHadronBranches.hh"
#include "SubstructureMomentBranches.hh"
#include "UnclusteredVertexBranches.hh"

#include "AthenaBaseComps/AthHistogramAlgorithm.h"
#include "GaudiKernel/ToolHandle.h"

#include "TFile.h"
#include "TTree.h"

#ifndef __MAKECINT__
#include "xAODTracking/TrackParticle.h"
#include "xAODTruth/TruthParticle.h"
#endif // not __MAKECINT__

#include "TrigDecisionTool/TrigDecisionTool.h"

// forward declarations
class IJetSelector;
class IJetCalibrationTool;
namespace InDet { class IInDetTrackSelectionTool; }
namespace CP {
  class ITrackVertexAssociationTool;
  class IPileupReweightingTool;
}
namespace Reco { class ITrackToVertex; }
namespace Trk  { class ITrackToVertexIPEstimator; }

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
}

class IGoodRunsListSelectionTool;
class IJetUpdateJvt;

class ArbitraryJetBranches;

enum TAGGERALGO{ IP2D=0,
		 IP3D,
		 SV0,
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
  std::string m_stream;

  // general event info
  int runnumber;
  int eventnumber;
  int mcchannel;
  double mcweight;
  int lbn;
  int coreFlag;
  int larError;
  int tileError;
  int npv;
  double mu;
  int Act_mu;
  double PV_x;
  double PV_y;
  double PV_z;
  double truth_PV_x;
  double truth_PV_y;
  double truth_PV_z;
  float  truth_LeadJet_pt;

  std::vector<float> *v_PVz;

  bool* v_L1trigger;
  std::vector<std::string> v_L1triggerNames;

  // jet info
  int njets;
  int nbjets;
  int nbjets_q;
  int nbjets_HadI;
  int nbjets_HadF;
  std::vector<float> *v_jet_pt;
  std::vector<float> *v_jet_eta;
  std::vector<float> *v_jet_pt_orig;
  std::vector<float> *v_jet_eta_orig;
  std::vector<float> *v_jet_phi_orig;
  std::vector<float> *v_jet_E_orig;
  std::vector<float> *v_jet_sumtrkS_pt;
  std::vector<float> *v_jet_sumtrkV_pt;
  std::vector<float> *v_jet_sumtrkV_eta;
  std::vector<float> *v_jet_sumtrkV_phi;
  std::vector<int>   *v_jet_sumtrk_ntrk;
  std::vector<float> *v_jet_phi;
  std::vector<float> *v_jet_E;
  std::vector<float> *v_jet_m;
  std::vector<int> *v_jet_nConst;
  std::vector<int> *v_jet_truthflav;

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
  std::vector<int> *v_jet_GhostL_q;
  std::vector<int> *v_jet_GhostL_HadI;
  std::vector<int> *v_jet_GhostL_HadF;
  std::vector<int> *v_jet_LabDr_HadF;
  std::vector<int> *v_jet_aliveAfterOR;
  std::vector<int> *v_jet_aliveAfterORmu;
  std::vector<int> *v_jet_truthMatch;
  std::vector<int> *v_jet_isPU;
  std::vector<int> *v_jet_isBadMedium;
  std::vector<float> *v_jet_truthPt;
  std::vector<float> *v_jet_dRiso;
  std::vector<float> *v_jet_JVT;
  std::vector<float> *v_jet_JVF;

  std::vector<float> *v_jet_dRminToB;
  std::vector<float> *v_jet_dRminToC;
  std::vector<float> *v_jet_dRminToT;

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
  std::vector<int> *v_jet_sv0_Nvtx;
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
  std::vector<int>   *v_jet_sv1_Nvtx;
  std::vector<float> *v_jet_sv1_sig3d;
  std::vector<std::vector<float> > *v_jet_sv1_vtxx;
  std::vector<std::vector<float> > *v_jet_sv1_vtxy;
  std::vector<std::vector<float> > *v_jet_sv1_vtxz;

  // JetFitter
  float PV_jf_x; //mod Remco
  float PV_jf_y; //mod Remco
  float PV_jf_z; //mod Remco

  std::vector<float> *v_jet_jf_pb;
  std::vector<float> *v_jet_jf_pc;
  std::vector<float> *v_jet_jf_pu;
  std::vector<float> *v_jet_jf_llr;
  std::vector<float> *v_jet_jf_m;
  std::vector<float> *v_jet_jf_mUncorr; //eloi
  std::vector<float> *v_jet_jf_efc;
  std::vector<float> *v_jet_jf_deta;
  std::vector<float> *v_jet_jf_dphi;
  std::vector<float> *v_jet_jf_dRFlightDir; //eloi
  std::vector<float> *v_jet_jf_ntrkAtVx;
  std::vector<int> *v_jet_jf_nvtx;
  std::vector<float> *v_jet_jf_sig3d;
  std::vector<int> *v_jet_jf_nvtx1t;
  std::vector<int> *v_jet_jf_n2t;
  std::vector<int> *v_jet_jf_VTXsize;
  std::vector<std::vector<float> > *v_jet_jf_vtx_chi2; //mod Remco
  std::vector<std::vector<float> > *v_jet_jf_vtx_ndf; //mod Remco
  std::vector<std::vector<int> > *v_jet_jf_vtx_ntrk; //mod Remco
  std::vector<std::vector<float> > *v_jet_jf_vtx_L3d; //mod Remco
  std::vector<std::vector<float> > *v_jet_jf_vtx_sig3d; //mod Remco
  std::vector<std::vector<float> > *v_jet_jf_vtx_sigTrans;
  std::vector<std::vector<float> > *v_jet_jf_vtx_x;
  std::vector<std::vector<float> > *v_jet_jf_vtx_x_err;
  std::vector<std::vector<float> > *v_jet_jf_vtx_y;
  std::vector<std::vector<float> > *v_jet_jf_vtx_y_err;
  std::vector<std::vector<float> > *v_jet_jf_vtx_z;
  std::vector<std::vector<float> > *v_jet_jf_vtx_z_err;

  std::vector<float> *v_jet_jf_phi; //mod Remco
  std::vector<float> *v_jet_jf_phi_err;
  std::vector<float> *v_jet_jf_theta; //mod Remco
  std::vector<float> *v_jet_jf_theta_err;

  // JetFitterCombNN
  std::vector<float> *v_jet_jfcombnn_pb;
  std::vector<float> *v_jet_jfcombnn_pc;
  std::vector<float> *v_jet_jfcombnn_pu;
  std::vector<float> *v_jet_jfcombnn_llr;

  std::vector<float> *v_jet_dl1_pb;
  std::vector<float> *v_jet_dl1_pc;
  std::vector<float> *v_jet_dl1_pu;

  // Other
  std::vector<double> *v_jet_sv1ip3d;
  std::vector<double> *v_jet_mv1;
  std::vector<double> *v_jet_mv1c;
  std::vector<double> *v_jet_mv2c00;
  std::vector<double> *v_jet_mv2c10;
  std::vector<double> *v_jet_mv2c20;
  std::vector<double> *v_jet_mv2c100;
  std::vector<double> *v_jet_mv2cl100;
  std::vector<double> *v_jet_mv2m_pb;
  std::vector<double> *v_jet_mv2m_pc;
  std::vector<double> *v_jet_mv2m_pu;
  std::vector<double> *v_jet_mvb;

  std::vector<double> *v_jet_ip2dNT_llr;
  std::vector<double> *v_jet_ip3dNT_llr;
  std::vector<double> *v_jet_sv1flip_llr;
  std::vector<double> *v_jet_jfflip_llr;
  std::vector<double> *v_jet_mv2c20flip;

  //MSV
  std::vector<double> *v_jet_multisvbb1;
  std::vector<double> *v_jet_multisvbb2;
  std::vector<int> *v_jet_msv_N2Tpair;
  std::vector<float> *v_jet_msv_energyTrkInJet;
  std::vector<int> *v_jet_msv_nvsec;
  std::vector<float> *v_jet_msv_normdist;
  std::vector<std::vector<float> > *v_jet_msv_vtx_cov0;
  std::vector<std::vector<float> > *v_jet_msv_vtx_cov1;
  std::vector<std::vector<float> > *v_jet_msv_vtx_cov2;
  std::vector<std::vector<float> > *v_jet_msv_vtx_cov3;
  std::vector<std::vector<float> > *v_jet_msv_vtx_cov4;
  std::vector<std::vector<float> > *v_jet_msv_vtx_cov5;
  std::vector<std::vector<float> > *v_jet_msv_vtx_mass;
  std::vector<std::vector<float> > *v_jet_msv_vtx_efrc;
  std::vector<std::vector<float> > *v_jet_msv_vtx_ntrk;
  std::vector<std::vector<float> > *v_jet_msv_vtx_pt;
  std::vector<std::vector<float> > *v_jet_msv_vtx_eta;
  std::vector<std::vector<float> > *v_jet_msv_vtx_phi;
  std::vector<std::vector<float> > *v_jet_msv_vtx_dls;
  std::vector<std::vector<float> > *v_jet_msv_vtx_x;
  std::vector<std::vector<float> > *v_jet_msv_vtx_y;
  std::vector<std::vector<float> > *v_jet_msv_vtx_z;
  std::vector<std::vector<float> > *v_jet_msv_vtx_chi;
  std::vector<std::vector<float> > *v_jet_msv_vtx_ndf;

  // Exktbb
  std::vector<double> *v_jet_ExKtbb_Hbb_DoubleMV2c20;
  std::vector<double> *v_jet_ExKtbb_Hbb_SingleMV2c20;
  std::vector<double> *v_jet_ExKtbb_Hbb_MV2Only;
  std::vector<double> *v_jet_ExKtbb_Hbb_MV2andJFDRSig;
  std::vector<double> *v_jet_ExKtbb_Hbb_MV2andTopos;

  // B hadron
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


  // track info
  std::vector<int>   *v_jet_btag_ntrk;

  std::vector<std::vector<float> > *v_jet_trk_pt;
  std::vector<std::vector<float> > *v_jet_trk_eta;
  std::vector<std::vector<float> > *v_jet_trk_theta;
  std::vector<std::vector<float> > *v_jet_trk_phi;
  std::vector<std::vector<float> > *v_jet_trk_qoverp;
  std::vector<std::vector<float> > *v_jet_trk_dr;
  std::vector<std::vector<int> > *v_jet_trk_assoc_msv;
  std::vector<std::vector<float> > *v_jet_trk_chi2;
  std::vector<std::vector<float> > *v_jet_trk_ndf;

  std::vector<std::vector<int> > *v_jet_trk_algo;
  std::vector<std::vector<int> > *v_jet_trk_orig;
  std::vector<std::vector<int> > *v_jet_trk_pdg_id;
  std::vector<std::vector<int> > *v_jet_trk_barcode;
  std::vector<std::vector<int> > *v_jet_trk_is_tracking_cp_loose;
  std::vector<std::vector<float> > *v_jet_trk_vtx_dx;
  std::vector<std::vector<float> > *v_jet_trk_vtx_dy;
  std::vector<std::vector<float> > *v_jet_trk_vtx_X;
  std::vector<std::vector<float> > *v_jet_trk_vtx_Y;

  std::vector<std::vector<int> > *v_jet_trk_nNextToInnHits;
  std::vector<std::vector<int> > *v_jet_trk_nInnHits;
  std::vector<std::vector<int> > *v_jet_trk_nBLHits; // soo this will be deprecated
  std::vector<std::vector<int> > *v_jet_trk_nsharedBLHits;
  std::vector<std::vector<int> > *v_jet_trk_nsplitBLHits;
  std::vector<std::vector<int> > *v_jet_trk_nPixHits;
  std::vector<std::vector<int> > *v_jet_trk_nPixHoles;
  std::vector<std::vector<int> > *v_jet_trk_nsharedPixHits;
  std::vector<std::vector<int> > *v_jet_trk_nsplitPixHits;
  std::vector<std::vector<int> > *v_jet_trk_nSCTHits;
  std::vector<std::vector<int> > *v_jet_trk_nSCTHoles;
  std::vector<std::vector<int> > *v_jet_trk_nsharedSCTHits;
  std::vector<std::vector<int> > *v_jet_trk_expectBLayerHit;
  std::vector<std::vector<float> > *v_jet_trk_d0;
  std::vector<std::vector<float> > *v_jet_trk_z0;
  std::vector<std::vector<float> > *v_jet_trk_d0_truth;
  std::vector<std::vector<float> > *v_jet_trk_z0_truth;

  std::vector<std::vector<int> > *v_jet_trk_IP3D_grade;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_d0;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_d02D;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_z0;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_d0sig;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_z0sig;
  std::vector<std::vector<float> > *v_jet_trk_IP2D_llr;
  std::vector<std::vector<float> > *v_jet_trk_IP3D_llr;

  // actual d0 variables (not lifetime-signed)
  std::vector<std::vector<float> > *v_jet_trk_ip_d0;
  std::vector<std::vector<float> > *v_jet_trk_ip_z0;

  std::vector<std::vector<int> > *v_jet_trk_jf_Vertex; //mod Remco

  // those are just quick accessors
  std::vector<int>   *v_jet_sv1_ntrk;
  std::vector<int>   *v_jet_ip3d_ntrk;
  std::vector<int>   *v_jet_jf_ntrk;

  // MVb variables
  std::vector<float> *v_jet_width;
  std::vector<int>   *v_jet_n_trk_sigd0cut;
  std::vector<float> *v_jet_trk3_d0sig;
  std::vector<float> *v_jet_trk3_z0sig;
  std::vector<float> *v_jet_sv_scaled_efc;
  std::vector<float> *v_jet_jf_scaled_efc;

  // additions by Andrea
  std::vector<float> *v_jet_mu_truthflav;
  std::vector<float> *v_jet_mu_dR;
  std::vector<float> *v_jet_mu_pTrel;
  std::vector<float> *v_jet_mu_qOverPratio;
  std::vector<float> *v_jet_mu_mombalsignif;
  std::vector<float> *v_jet_mu_scatneighsignif;
  std::vector<float> *v_jet_mu_VtxTyp;
  std::vector<float> *v_jet_mu_pt;
  std::vector<float> *v_jet_mu_eta;
  std::vector<float> *v_jet_mu_phi;
  std::vector<float> *v_jet_mu_d0;
  std::vector<float> *v_jet_mu_z0;
  std::vector<float> *v_jet_mu_parent_pdgid;
  std::vector<float> *v_jet_mu_ID_qOverP_var;
  std::vector<float> *v_jet_mu_muonType;
  std::vector<float> *v_jet_mu_assJet_pt;
  // additions by nikola
  std::vector<int> *v_jet_mu_fatjet_nMu;
  std::vector<float> *v_jet_mu_fatjet_pTmax_pT;
  std::vector<float> *v_jet_mu_fatjet_pTmax_pTrel;
  std::vector<float> *v_jet_mu_fatjet_pTmax_pTrelFrac;

  void clearvectors();

#ifndef __MAKECINT__
  const xAOD::TruthParticle*  truthParticle(const xAOD::TrackParticle *trkPart) const;
  void GetParentTracks(const xAOD::TruthParticle* part,
		       std::vector<const xAOD::TruthParticle*> &tracksFromB,
		       std::vector<const xAOD::TruthParticle*> &tracksFromC,
		       bool isfromC, std::string indent);
  bool decorateTruth(const xAOD::TruthParticle & particle);
  int parent_classify(const xAOD::TruthParticle * theParticle);
  //bool particleInCollection( const xAOD::TrackParticle *trkPart,
  //std::vector< ElementLink< xAOD::TrackParticleContainer > > trkColl);
  void fillGhostTracks(const xAOD::Jet& jet,
                       const xAOD::Vertex& vx);


#endif // not __MAKECINT__


  /// from outside
  bool m_reduceInfo;               // if set to true is allows to run over xAOD and not crashing when info are missing
  bool m_essentialInfo;            // basically as slim as possible ntuple which will only allow to make efficiency plots
  bool m_dumpCaloInfo;
  bool m_subjetInfo;
  bool m_dumpTrackCovariance;
  bool m_dumpGATracks;
  bool m_doMSV;                    // if set to true it includes variables from multi SV tagger
  bool m_rel20;                    // if set to true code works for rel20, if set to false it will work for rel19
  bool m_SMT;
  bool m_bHadronInfo;

  std::string m_jetCollectionName; // name of the jet collection to work with
  float m_jetPtCut;                // pT cut to apply
  bool m_calibrateJets;
  bool m_cleanJets;

  std::string m_triggerLogic;

 private:

  // additions by Dan: branch collections

  // B-hadron quantities
  BHadronBranches m_bhadron_branches;

  // cluster dumper
  ClusterBranches m_cluster_branches;
  SubstructureMomentBranches m_substructure_moment_branches;
  // subjet dumper
  SubjetBranches m_exkt_branches;
  SubjetBranches m_trkjet_branches;
  SubjetBranches m_vrtrkjet_branches;
  // track dumper
  TrackCovBranches m_track_cov_branches;
  TrackBranches m_ga_track_branches;
  TrackCovBranches m_ga_track_cov_branches;
  // unclustered vertices
  UnclusteredVertexBranches m_unclustered_vertices;
  // must be initialized after the constructor
  ArbitraryJetBranches* m_arb_branches;
  std::vector<std::string> m_arb_double_names;
  std::vector<std::string> m_arb_float_vec_names;

  /// tool handle for jet cleaning tool
  ToolHandle< IJetSelector > m_jetCleaningTool;

  /// tool handle for jet calibration tool
  ToolHandle< IJetCalibrationTool > m_jetCalibrationTool;

  /** InDetTrackSelectorTool (temporary: to be moved to a separate Tool) */
  ToolHandle< InDet::IInDetTrackSelectionTool > m_InDetTrackSelectorTool;
  ToolHandle< InDet::IInDetTrackSelectionTool > m_CPTrackingLooseLabel;

  /** TrackVertex associator (temporary: to be moved to a separate Tool) */
  ToolHandle< CP::ITrackVertexAssociationTool > m_TightTrackVertexAssociationTool;

  /** TrackToVertex tool */
  //ToolHandle< Reco::ITrackToVertex > m_trackToVertexTool;

  /** GP: Tool for the estimation of the IPs to the Vertex */
  ToolHandle< Trk::ITrackToVertexIPEstimator > m_trackToVertexIPEstimator;

  ToolHandle< Trig::TrigDecisionTool > m_tdt;

  std::string m_GRLname;
  ToolHandle<IGoodRunsListSelectionTool> m_GRLSelectionTool;

  ToolHandle<IJetUpdateJvt> m_jvt;

  ToolHandle<CP::IPileupReweightingTool> m_PUtool;

  // determine whether particle is B hadron or not
  bool isBHadron(int pdgid);

  // compute dR between two objects
  float deltaR(float eta1, float eta2, float phi1, float phi2);
  std::vector<float> JF_xyz_errors(float L3D, float L3Derr, float Theta, float Theta_err, float Phi, float Phi_err,float Pv_x, float Pv_y, float Pv_z);
  float JF_Transverse_error(float L3D, float Theta, float Theta_err, float Phi, float Phi_err);

  const xAOD::Jet* GetParentJet(const xAOD::Jet* Jet, std::string Keyname);

int getTrackOrigin(const xAOD::TrackParticle *tmpTrk,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCc,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB1,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB2,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC1,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC2,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB1,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB2);
};

#endif //> !BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
