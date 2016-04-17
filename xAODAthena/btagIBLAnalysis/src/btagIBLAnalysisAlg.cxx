///////////////////////////////////////
// btagIBLAnalysisAlg.cxx
///////////////////////////////////////
// Author(s): marx@cern.ch
// Description: athena-based code to process xAODs
///////////////////////////////////////
#include "track_to_vertex_associators.hh"

#include <utility>
#include "GaudiKernel/ITHistSvc.h"
#include "GaudiKernel/ServiceHandle.h"

#include "btagIBLAnalysisAlg.h"
#include "GAFlavourLabel.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODJet/JetContainer.h"
#include "xAODTracking/Vertex.h"
#include "xAODTracking/TrackParticleContainer.h"
#include "xAODMuon/MuonContainer.h"
#include "ParticleJetTools/JetFlavourInfo.h"
#include "xAODBTagging/SecVtxHelper.h"

#include "JetInterface/IJetSelector.h"
#include "JetCalibTools/IJetCalibrationTool.h"
#include "xAODEventShape/EventShape.h"

#include "InDetTrackSelectionTool/IInDetTrackSelectionTool.h"
#include "TrackVertexAssociationTool/ITrackVertexAssociationTool.h"
#include "TrkVertexFitterInterfaces/ITrackToVertexIPEstimator.h"
#include "PileupReweighting/PileupReweightingTool.h"

//#include "xAODTrigger/JetRoIContainer.h"
//#include "xAODTrigger/MuonRoIContainer.h"
#include "TrigDecisionTool/TrigDecisionTool.h"
#include "GoodRunsLists/IGoodRunsListSelectionTool.h"
#include "JetInterface/IJetUpdateJvt.h"

// some tracking mumbo jumbo
#include "TDatabasePDG.h"
#include "TParticlePDG.h"

using xAOD::IParticle;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool particleInCollection( const xAOD::TrackParticle *trkPart, std::vector< ElementLink< xAOD::TrackParticleContainer > > trkColl ) {
  for (unsigned int iT = 0; iT < trkColl.size(); iT++) {
    if (trkPart == *(trkColl.at(iT))) return true;
  }
  return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool xaodJetPtSorting(const xAOD::Jet *jet1, const xAOD::Jet *jet2) {
  return jet1->pt() > jet2->pt();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isFromWZ( const xAOD::TruthParticle* particle ) {
  if ( particle==0 ) return false;
  
  if ( fabs(particle->pdgId())!= 11 && fabs(particle->pdgId())!= 13) return false;
  
  const xAOD::TruthVertex* prodvtx = particle->prodVtx();
  if ( prodvtx==0 ) return false;
  
  if (  prodvtx->nIncomingParticles()==0 ) return false; 
  
  if (  prodvtx->nIncomingParticles()>1 ) {
    int nCharge=0;
    int nNeutral=0;
    for(unsigned j = 0; j < prodvtx->nIncomingParticles(); j++){
      if ( fabs( prodvtx->incomingParticle(j)->pdgId() )==11 || fabs( prodvtx->incomingParticle(j)->pdgId() )==13 ) nCharge++;
      if ( fabs( prodvtx->incomingParticle(j)->pdgId() )==12 || fabs( prodvtx->incomingParticle(j)->pdgId() )==14 ) nNeutral++;
    }
    if ( nCharge>1 ) return true;
    if ( nCharge+nNeutral>1 ) return true;
    return false; 
  }
  int absPDG=fabs(prodvtx->incomingParticle(0)->pdgId());
  if ( absPDG==15) return false;
  else if ( absPDG==24 || absPDG==23 ) return true;
  return isFromWZ( prodvtx->incomingParticle(0) );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
btagIBLAnalysisAlg::btagIBLAnalysisAlg( const std::string& name, ISvcLocator *pSvcLocator ) :
  AthHistogramAlgorithm(name, pSvcLocator),
  m_SMT(false),
  m_stream("BTAGSTREAM"),
  m_dumpCaloInfo(false),
  m_cluster_branches(),
  m_substructure_moment_branches(),
  m_exkt_branches(),
  m_vrtrkjet_branches(),
  m_track_branches(),
  m_unclustered_vertices(),
  m_jetCleaningTool("JetCleaningTool/JetCleaningTool", this),
  m_jetCalibrationTool(""),
  m_InDetTrackSelectorTool(""),
  m_TightTrackVertexAssociationTool(""),
  m_tdt("Trig::TrigDecisionTool/TrigDecisionTool"),
  m_GRLSelectionTool("GoodRunsListSelectionTool/GoodRunsListSelectionTool", this),
  m_jvt("")
{
  m_triggerLogic="";
  declareProperty( "Stream", m_stream );

  declareProperty( "JetCleaningTool", m_jetCleaningTool );
  declareProperty( "JetCalibrationTool", m_jetCalibrationTool );

  declareProperty( "InDetTrackSelectionTool", m_InDetTrackSelectorTool );
  declareProperty( "TrackVertexAssociationTool", m_TightTrackVertexAssociationTool );
  declareProperty( "TrackToVertexIPEstimator", m_trackToVertexIPEstimator );
  declareProperty( "JVTtool", m_jvt );

  declareProperty( "EssentialInfo", m_essentialInfo =true );
  declareProperty( "ReduceInfo"   , m_reduceInfo=false );
  declareProperty( "Rel20", m_rel20 = false );
  declareProperty( "DoMSV", m_doMSV = false );
  declareProperty( "doSMT", m_SMT);
  declareProperty( "CalibrateJets", m_calibrateJets = true );
  declareProperty( "CleanJets", m_cleanJets = true );

  declareProperty( "GRLname", m_GRLname = "" );
  declareProperty( "JetCollectionName", m_jetCollectionName = "AntiKt4LCTopoJets" );
  declareProperty( "JetPtCut", m_jetPtCut = 20.e3 );

  declareProperty( "TriggerLogic", m_triggerLogic );

  declareProperty( "DumpCaloInfo", m_dumpCaloInfo);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
btagIBLAnalysisAlg::~btagIBLAnalysisAlg() {
  // FIXME: we're leaking memory with all the vectors, that we never
  // delete, but I suppose there are bigger issues with this code.
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StatusCode btagIBLAnalysisAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  if (m_essentialInfo) m_reduceInfo=true;

  // Register histograms
  //ATH_CHECK( book( TH1F("hist_Lxy_denom", "Lxy", 200, 0.0, 100.0) ) );

  // Register output tree
  ServiceHandle<ITHistSvc> histSvc("THistSvc",name());
  CHECK( histSvc.retrieve() );

  if (m_GRLname != "") ATH_CHECK(m_GRLSelectionTool.retrieve());

  ATH_MSG_INFO(m_jetCollectionName);

  tree = new TTree( ("bTag_" + m_jetCollectionName).c_str(),
		    ("bTag"  + m_jetCollectionName).c_str() );
  ATH_MSG_INFO ("VALERIO: registering tree in stream: " << m_stream);
  CHECK( histSvc->regTree("/" + m_stream + "/tree_" + m_jetCollectionName, tree) );

  // Retrieve the jet cleaning tool
  CHECK( m_jetCleaningTool.retrieve() );
  CHECK( m_jvt.retrieve() );

  // Retrieve the jet calibration tool
  m_jetCalibrationTool.setTypeAndName("JetCalibrationTool/BTagDumpAlg_" + m_jetCollectionName + "_JCalib");
  if (m_calibrateJets) CHECK( m_jetCalibrationTool.retrieve() );

  // retrieve other special tools
  if (m_InDetTrackSelectorTool.retrieve().isFailure())  {
    ATH_MSG_FATAL("#BTAG# Failed to retrieve tool " << m_InDetTrackSelectorTool);
    return StatusCode::FAILURE;
  }
  else {
    ATH_MSG_DEBUG("#BTAG# Retrieved tool " << m_InDetTrackSelectorTool);
  }
  if (m_TightTrackVertexAssociationTool.retrieve().isFailure())  {
    ATH_MSG_FATAL("#BTAG# Failed to retrieve tool " <<  m_TightTrackVertexAssociationTool);
    return StatusCode::FAILURE;
  }
  if (m_trackToVertexIPEstimator.retrieve().isFailure()) {
    ATH_MSG_FATAL("#BTAG# Failed to retrieve tool " << m_trackToVertexIPEstimator);
    return StatusCode::FAILURE;
  }
  else {
    ATH_MSG_DEBUG("#BTAG# Retrieved tool " << m_trackToVertexIPEstimator);
  }

  m_PUtool.setTypeAndName("CP::PileupReweightingTool/prw");
  CHECK( m_PUtool.retrieve() );

  ATH_CHECK(m_tdt.retrieve());

  // addition from Dan: create cluster branches
  if (m_dumpCaloInfo) {
    m_cluster_branches.set_tree(*tree);
    m_substructure_moment_branches.set_tree(*tree);
  }
  m_exkt_branches.set_tree(*tree, "jet_exktsubjet_");
  m_vrtrkjet_branches.set_tree(*tree, "jet_vrtrkjet_");
  m_unclustered_vertices.set_tree(*tree, "jet_trkjet_");
  m_track_branches.set_tree(*tree, "jet_trk_");

  // Setup branches
  v_jet_pt = new std::vector<float>(); //v_jet_pt->reserve(15);
  v_jet_eta = new std::vector<float>(); //v_jet_eta->reserve(15);
  v_jet_phi = new std::vector<float>(); //v_jet_phi->reserve(15);
  v_jet_pt_orig =new std::vector<float>();
  v_jet_eta_orig = new std::vector<float>();
  v_jet_phi_orig = new std::vector<float>();
  v_jet_E_orig = new std::vector<float>();
  v_jet_sumtrkS_pt = new std::vector<float>();
  v_jet_sumtrkV_pt = new std::vector<float>();
  v_jet_sumtrkV_phi = new std::vector<float>();
  v_jet_sumtrkV_eta = new std::vector<float>();
  v_jet_sumtrk_ntrk = new std::vector<int>();
  v_jet_E = new std::vector<float>(); //v_jet_E->reserve(15);
  v_jet_m = new std::vector<float>(); //v_jet_m->reserve(15);
  v_jet_nConst     =new std::vector<int>();
  v_jet_truthflav = new std::vector<int>();
  v_jet_nBHadr = new std::vector<int>();
  v_jet_nCHadr = new std::vector<int>();
  v_jet_nGhostBHadrFromParent = new std::vector<int>(); // mod nikola
  v_jet_nGhostCHadrFromParent = new std::vector<int>(); // mod nikola
  v_jet_nGhostCHadrFromParentNotFromB = new std::vector<int>(); // mod nikola
  v_jet_nGhostTauFromParent = new std::vector<int>(); // mod nikola
  v_jet_nGhostHBosoFromParent = new std::vector<int>(); // mod nikola
  v_jet_GhostL_q = new std::vector<int>();
  v_jet_GhostL_HadI = new std::vector<int>();
  v_jet_GhostL_HadF = new std::vector<int>();
  v_jet_LabDr_HadF = new std::vector<int>();
  v_jet_aliveAfterOR = new std::vector<int>();
  v_jet_aliveAfterORmu =new std::vector<int>();
  v_jet_truthMatch = new std::vector<int>();
  v_jet_isPU = new std::vector<int>();
  v_jet_isBadMedium = new std::vector<int>();
  v_jet_truthPt = new std::vector<float>();
  v_jet_dRiso = new std::vector<float>();
  v_jet_JVT = new std::vector<float>();
  v_jet_JVF = new std::vector<float>();
  v_jet_dRminToB = new std::vector<float>();
  v_jet_dRminToC = new std::vector<float>();
  v_jet_dRminToT = new std::vector<float>();

  v_jet_ip2d_pb = new std::vector<float>();
  v_jet_ip2d_pc = new std::vector<float>();
  v_jet_ip2d_pu = new std::vector<float>();
  v_jet_ip2d_llr = new std::vector<float>();
  v_jet_ip3d_pb = new std::vector<float>();
  v_jet_ip3d_pc = new std::vector<float>();
  v_jet_ip3d_pu = new std::vector<float>();
  v_jet_ip3d_llr = new std::vector<float>();
  v_jet_sv0_sig3d = new std::vector<float>();
  v_jet_sv0_ntrkj = new std::vector<int>();
  v_jet_sv0_ntrkv = new std::vector<int>();
  v_jet_sv0_n2t = new std::vector<int>();
  v_jet_sv0_m = new std::vector<float>();
  v_jet_sv0_efc = new std::vector<float>();
  v_jet_sv0_normdist = new std::vector<float>();
  v_jet_sv0_Nvtx = new std::vector<int>();
  v_jet_sv0_vtxx = new std::vector<std::vector<float> >();
  v_jet_sv0_vtxy = new std::vector<std::vector<float> >();
  v_jet_sv0_vtxz = new std::vector<std::vector<float> >();
  v_jet_sv1_ntrkj =new std::vector<int>();
  v_jet_sv1_ntrkv = new std::vector<int>();
  v_jet_sv1_n2t = new std::vector<int>();
  v_jet_sv1_m = new std::vector<float>();
  v_jet_sv1_efc = new std::vector<float>();
  v_jet_sv1_normdist =new std::vector<float>();
  v_jet_sv1_Nvtx = new std::vector<int>();
  v_jet_sv1_pb = new std::vector<float>();
  v_jet_sv1_pc = new std::vector<float>();
  v_jet_sv1_pu = new std::vector<float>();
  v_jet_sv1_llr = new std::vector<float>();
  v_jet_sv1_sig3d = new std::vector<float>();
  v_jet_sv1_vtxx = new std::vector<std::vector<float> >();
  v_jet_sv1_vtxy = new std::vector<std::vector<float> >();
  v_jet_sv1_vtxz = new std::vector<std::vector<float> >();

  v_jet_jf_pb = new std::vector<float>();
  v_jet_jf_pc = new std::vector<float>();
  v_jet_jf_pu = new std::vector<float>();
  v_jet_jf_llr = new std::vector<float>();
  v_jet_jf_m = new std::vector<float>();
  v_jet_jf_mUncorr=new std::vector<float>(); //eloi
  v_jet_jf_efc = new std::vector<float>();
  v_jet_jf_deta = new std::vector<float>();
  v_jet_jf_dphi = new std::vector<float>();
  v_jet_jf_dRFlightDir=new std::vector<float>(); //eloi
  v_jet_jf_ntrkAtVx = new std::vector<float>();
  v_jet_jf_nvtx = new std::vector<int>();
  v_jet_jf_sig3d = new std::vector<float>();
  v_jet_jf_nvtx1t = new std::vector<int>();
  v_jet_jf_n2t = new std::vector<int>();
  v_jet_jf_VTXsize = new std::vector<int>();
  v_jet_jf_vtx_chi2 = new std::vector<std::vector<float> >(); // mod Remco
  v_jet_jf_vtx_ndf = new std::vector<std::vector<float> >(); // mod Remco
  v_jet_jf_vtx_ntrk = new std::vector<std::vector<int> >(); // mod Remco
  v_jet_jf_vtx_L3d = new std::vector<std::vector<float> >(); // mod Remco
  v_jet_jf_vtx_sig3d = new std::vector<std::vector<float> >(); // mod Remco
  v_jet_jf_vtx_nvtx = new std::vector<std::vector<int> >(); // mod Remco
  v_jet_jf_phi = new std::vector<float>(); // mod Remco
  v_jet_jf_theta = new std::vector<float>(); // mod Remco

  v_jet_jfcombnn_pb = new std::vector<float>();
  v_jet_jfcombnn_pc = new std::vector<float>();
  v_jet_jfcombnn_pu = new std::vector<float>();
  v_jet_jfcombnn_llr = new std::vector<float>();

  v_jet_dl1_pb=new std::vector<float>();
  v_jet_dl1_pc=new std::vector<float>();
  v_jet_dl1_pu=new std::vector<float>();

  v_jet_sv1ip3d = new std::vector<double>();
  v_jet_mv1 = new std::vector<double>();
  v_jet_mv1c = new std::vector<double>();
  v_jet_mv2c00 = new std::vector<double>();
  v_jet_mv2c10 = new std::vector<double>();
  v_jet_mv2c20 = new std::vector<double>();
  v_jet_mv2c100 = new std::vector<double>();
  v_jet_mv2cl100 = new std::vector<double>();
  v_jet_mv2m_pu = new std::vector<double>();
  v_jet_mv2m_pc = new std::vector<double>();
  v_jet_mv2m_pb = new std::vector<double>();
  v_jet_mvb = new std::vector<double>();

  v_jet_ip2dNT_llr = new std::vector<double>();
  v_jet_ip3dNT_llr = new std::vector<double>();
  v_jet_sv1flip_llr = new std::vector<double>();
  v_jet_jfflip_llr = new std::vector<double>();
  v_jet_mv2c20flip = new std::vector<double>();

  v_jet_multisvbb1 = new std::vector<double>();
  v_jet_multisvbb2 = new std::vector<double>();
  v_jet_msv_N2Tpair = new std::vector<int>();
  v_jet_msv_energyTrkInJet = new std::vector<float>();
  v_jet_msv_nvsec = new std::vector<int>();
  v_jet_msv_normdist = new std::vector<float>();
  v_jet_msv_vtx_cov0 = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_cov1 = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_cov2 = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_cov3 = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_cov4 = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_cov5 = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_mass = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_efrc = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_ntrk = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_pt = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_eta = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_phi = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_dls = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_x = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_y = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_z = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_chi = new std::vector<std::vector<float> >();
  v_jet_msv_vtx_ndf = new std::vector<std::vector<float> >();

  v_jet_ExKtbb_Hbb_DoubleMV2c20 = new std::vector<double>();
  v_jet_ExKtbb_Hbb_SingleMV2c20 = new std::vector<double>();
  v_jet_ExKtbb_Hbb_MV2Only = new std::vector<double>();
  v_jet_ExKtbb_Hbb_MV2andJFDRSig = new std::vector<double>();
  v_jet_ExKtbb_Hbb_MV2andTopos = new std::vector<double>();

  v_bH1_pt = new std::vector<float>();
  v_bH1_eta = new std::vector<float>();
  v_bH1_phi = new std::vector<float>();
  v_bH1_Lxy = new std::vector<float>();
  v_bH1_dRjet = new std::vector<float>();
  v_bH1_x = new std::vector<float>();
  v_bH1_y = new std::vector<float>();
  v_bH1_z = new std::vector<float>();

  v_bH2_pt = new std::vector<float>();
  v_bH2_eta = new std::vector<float>();
  v_bH2_phi = new std::vector<float>();
  v_bH2_Lxy = new std::vector<float>();
  v_bH2_dRjet = new std::vector<float>();
  v_bH2_x = new std::vector<float>();
  v_bH2_y = new std::vector<float>();
  v_bH2_z = new std::vector<float>();

  v_bH_pt = new std::vector<float>();
  v_bH_eta = new std::vector<float>();
  v_bH_phi = new std::vector<float>();
  v_bH_Lxy = new std::vector<float>();
  v_bH_dRjet = new std::vector<float>();
  v_bH_x = new std::vector<float>();
  v_bH_y = new std::vector<float>();
  v_bH_z = new std::vector<float>();
  v_bH_nBtracks = new std::vector<int>();
  v_bH_nCtracks = new std::vector<int>();

  v_cH_pt = new std::vector<float>();
  v_cH_eta = new std::vector<float>();
  v_cH_phi = new std::vector<float>();
  v_cH_Lxy = new std::vector<float>();
  v_cH_dRjet = new std::vector<float>();
  v_cH_x = new std::vector<float>();
  v_cH_y = new std::vector<float>();
  v_cH_z = new std::vector<float>();
  v_cH_nCtracks = new std::vector<int>();

  v_jet_btag_ntrk = new std::vector<int>();
  v_jet_trk_pt = new std::vector<std::vector<float> >();
  v_jet_trk_eta = new std::vector<std::vector<float> >();
  v_jet_trk_theta = new std::vector<std::vector<float> >();
  v_jet_trk_phi = new std::vector<std::vector<float> >();
  v_jet_trk_qoverp = new std::vector<std::vector<float> >();
  v_jet_trk_dr = new std::vector<std::vector<float> >();
  v_jet_trk_assoc_msv = new std::vector<std::vector<int> >();   // mod nikola
  v_jet_trk_chi2 = new std::vector<std::vector<float> >();
  v_jet_trk_ndf = new std::vector<std::vector<float> >();
  v_jet_trk_algo = new std::vector<std::vector<int> >();
  v_jet_trk_orig = new std::vector<std::vector<int> >();

  v_jet_trk_vtx_X = new std::vector<std::vector<float> >();
  v_jet_trk_vtx_Y = new std::vector<std::vector<float> >();
  v_jet_trk_vtx_dx = new std::vector<std::vector<float> >();
  v_jet_trk_vtx_dy = new std::vector<std::vector<float> >();

  v_jet_trk_nNextToInnHits = new std::vector<std::vector<int> >();
  v_jet_trk_nInnHits = new std::vector<std::vector<int> >();
  v_jet_trk_nBLHits = new std::vector<std::vector<int> >();
  v_jet_trk_nsharedBLHits = new std::vector<std::vector<int> >();
  v_jet_trk_nsplitBLHits = new std::vector<std::vector<int> >();
  v_jet_trk_nPixHits = new std::vector<std::vector<int> >();
  v_jet_trk_nsharedPixHits = new std::vector<std::vector<int> >();
  v_jet_trk_nsplitPixHits = new std::vector<std::vector<int> >();
  v_jet_trk_nSCTHits = new std::vector<std::vector<int> >();
  v_jet_trk_nsharedSCTHits = new std::vector<std::vector<int> >();
  v_jet_trk_expectBLayerHit = new std::vector<std::vector<int> >();
  v_jet_trk_d0 = new std::vector<std::vector<float> >();
  v_jet_trk_z0 = new std::vector<std::vector<float> >();
  // v_jet_trk_d0sig = new std::vector<std::vector<float> >();
  // v_jet_trk_z0sig = new std::vector<std::vector<float> >();

  v_jet_trk_d0_truth = new std::vector<std::vector<float> >();
  v_jet_trk_z0_truth = new std::vector<std::vector<float> >();

  v_jet_trk_IP3D_grade = new std::vector<std::vector<int> >();
  v_jet_trk_IP3D_d0 = new std::vector<std::vector<float> >();
  v_jet_trk_IP3D_d02D = new std::vector<std::vector<float> >();
  v_jet_trk_IP3D_z0 = new std::vector<std::vector<float> >();
  v_jet_trk_IP3D_d0sig = new std::vector<std::vector<float> >();
  v_jet_trk_IP3D_z0sig = new std::vector<std::vector<float> >();
  v_jet_trk_IP2D_llr = new std::vector<std::vector<float> >();
  v_jet_trk_IP3D_llr = new std::vector<std::vector<float> >();

  // non-lifetime signed
  v_jet_trk_ip_d0 = new std::vector<std::vector<float> >();
  v_jet_trk_ip_z0 = new std::vector<std::vector<float> >();

  v_jet_trk_jf_Vertex = new std::vector<std::vector<int> >(); // mod Remco

  // those are just quick accessors
  v_jet_sv1_ntrk = new std::vector<int>();
  v_jet_ip3d_ntrk = new std::vector<int>();
  v_jet_jf_ntrk = new std::vector<int>();

  // MVb variables
  v_jet_width  = new std::vector<float>();
  v_jet_n_trk_sigd0cut  = new std::vector<int>();
  v_jet_trk3_d0sig  = new std::vector<float>();
  v_jet_trk3_z0sig  = new std::vector<float>();
  v_jet_sv_scaled_efc  = new std::vector<float>();
  v_jet_jf_scaled_efc  = new std::vector<float>();

  // additions by andrea
  v_jet_mu_assJet_pt = new std::vector<float>();
  v_jet_mu_truthflav = new std::vector<float>();
  v_jet_mu_dR = new std::vector<float>();
  v_jet_mu_pTrel = new std::vector<float>();;
  v_jet_mu_qOverPratio = new std::vector<float>();
  v_jet_mu_mombalsignif = new std::vector<float>();
  v_jet_mu_scatneighsignif = new std::vector<float>();
  v_jet_mu_VtxTyp = new std::vector<float>();
  v_jet_mu_pt = new std::vector<float>();
  v_jet_mu_eta = new std::vector<float>();
  v_jet_mu_phi = new std::vector<float>();
  v_jet_mu_d0 = new std::vector<float>();
  v_jet_mu_z0 = new std::vector<float>();
  v_jet_mu_parent_pdgid = new std::vector<float>();
  v_jet_mu_ID_qOverP_var = new std::vector<float>();
  v_jet_mu_muonType = new std::vector<float>();
  // additions by nikola
  v_jet_mu_fatjet_nMu = new std::vector<int>();
  v_jet_mu_fatjet_pTmax_pT = new std::vector<float>();
  v_jet_mu_fatjet_pTmax_pTrel = new std::vector<float>();
  v_jet_mu_fatjet_pTmax_pTrelFrac = new std::vector<float>();

  tree->Branch("runnb", &runnumber);
  tree->Branch("eventnb", &eventnumber);
  tree->Branch("mcchan", &mcchannel);
  tree->Branch("mcwg", &mcweight);
  tree->Branch("lbn", &lbn);
  tree->Branch("coreFlag", &coreFlag);
  tree->Branch("larError", &larError);
  tree->Branch("tileError", &tileError);
  tree->Branch("nPV", &npv);
  tree->Branch("avgmu", &mu);
  tree->Branch("actmu",&Act_mu);
  tree->Branch("PVx", &PV_x);
  tree->Branch("PVy", &PV_y);
  tree->Branch("PVz", &PV_z);
  tree->Branch("truth_PVx", &truth_PV_x);
  tree->Branch("truth_PVy", &truth_PV_y);
  tree->Branch("truth_PVz", &truth_PV_z);
  tree->Branch("truth_LeadJet_pt", &truth_LeadJet_pt);

  v_PVz=new std::vector<float>();
  //tree->Branch("PVzV",&v_PVz); ///to decomment if you want ALL the PV
 
  tree->Branch("njets", &njets);
  if (!m_essentialInfo) tree->Branch("nbjets", &nbjets);
  if (!m_essentialInfo) tree->Branch("nbjets_q", &nbjets_q);
  if (!m_essentialInfo) tree->Branch("nbjets_HadI", &nbjets_HadI);
  if (!m_essentialInfo) tree->Branch("nbjets_HadF", &nbjets_HadF);
  tree->Branch("jet_pt", &v_jet_pt);
  tree->Branch("jet_eta", &v_jet_eta);
  tree->Branch("jet_phi", &v_jet_phi);
  tree->Branch("jet_pt_orig", &v_jet_pt_orig);
  tree->Branch("jet_eta_orig", &v_jet_eta_orig);
  if (!m_essentialInfo) tree->Branch("jet_phi_orig", &v_jet_phi_orig);
  if (!m_essentialInfo) tree->Branch("jet_E_orig", &v_jet_E_orig);
  if (!m_essentialInfo) tree->Branch("jet_sumtrkS_pt", &v_jet_sumtrkS_pt);
  tree->Branch("jet_sumtrkV_pt", &v_jet_sumtrkV_pt);
  tree->Branch("jet_sumtrkV_eta", &v_jet_sumtrkV_eta);
  if (!m_essentialInfo) tree->Branch("jet_sumtrkV_phi", &v_jet_sumtrkV_phi);
  tree->Branch("jet_sumtrk_ntrk", &v_jet_sumtrk_ntrk);
  tree->Branch("jet_E", &v_jet_E);
  if (!m_essentialInfo) tree->Branch("jet_m", &v_jet_m);
  tree->Branch("jet_nConst",&v_jet_nConst);
  if (!m_essentialInfo) tree->Branch("jet_truthflav", &v_jet_truthflav);
  tree->Branch("jet_nBHadr", &v_jet_nBHadr);
  tree->Branch("jet_nCHadr", &v_jet_nCHadr);
  if (!m_essentialInfo) tree->Branch("jet_nGhostBHadrFromParent", &v_jet_nGhostBHadrFromParent); // mod nikola
  if (!m_essentialInfo) tree->Branch("jet_nGhostCHadrFromParent", &v_jet_nGhostCHadrFromParent); // mod nikola
  if (!m_essentialInfo) tree->Branch("jet_nGhostCHadrFromParentNotFromB", &v_jet_nGhostCHadrFromParentNotFromB); // mod nikola
  if (!m_essentialInfo) tree->Branch("jet_nGhostTauFromParent", &v_jet_nGhostTauFromParent); // mod nikola
  if (!m_essentialInfo) tree->Branch("jet_nGhostHBosoFromParent", &v_jet_nGhostHBosoFromParent); // mod nikola
  if (!m_essentialInfo) tree->Branch("jet_GhostL_q", &v_jet_GhostL_q);
  if (!m_essentialInfo) tree->Branch("jet_GhostL_HadI", &v_jet_GhostL_HadI);
  if (!m_essentialInfo) tree->Branch("jet_GhostL_HadF", &v_jet_GhostL_HadF);
  tree->Branch("jet_LabDr_HadF", &v_jet_LabDr_HadF);
  tree->Branch("jet_aliveAfterOR", &v_jet_aliveAfterOR);
  tree->Branch("jet_aliveAfterORmu" ,&v_jet_aliveAfterORmu);
  tree->Branch("jet_truthMatch", &v_jet_truthMatch);
  tree->Branch("jet_isPU", &v_jet_isPU);
  tree->Branch("jet_isBadMedium", &v_jet_isBadMedium);
  tree->Branch("jet_truthPt", &v_jet_truthPt);
  tree->Branch("jet_dRiso", &v_jet_dRiso);
  tree->Branch("jet_JVT", &v_jet_JVT);
  if (!m_essentialInfo) tree->Branch("jet_JVF", &v_jet_JVF);
  tree->Branch("jet_dRminToB", &v_jet_dRminToB);
  tree->Branch("jet_dRminToC", &v_jet_dRminToC);
  tree->Branch("jet_dRminToT", &v_jet_dRminToT);

  if (!m_essentialInfo) tree->Branch("jet_ip2d_pb", &v_jet_ip2d_pb);
  if (!m_essentialInfo) tree->Branch("jet_ip2d_pc", &v_jet_ip2d_pc);
  if (!m_essentialInfo) tree->Branch("jet_ip2d_pu", &v_jet_ip2d_pu);
  tree->Branch("jet_ip2d_llr", &v_jet_ip2d_llr);

  if (!m_essentialInfo) tree->Branch("jet_ip3d_pb", &v_jet_ip3d_pb);
  if (!m_essentialInfo) tree->Branch("jet_ip3d_pc", &v_jet_ip3d_pc);
  if (!m_essentialInfo) tree->Branch("jet_ip3d_pu", &v_jet_ip3d_pu);
  tree->Branch("jet_ip3d_llr", &v_jet_ip3d_llr);

  tree->Branch("jet_sv0_sig3d", &v_jet_sv0_sig3d);
  if (!m_essentialInfo) tree->Branch("jet_sv0_ntrkj", &v_jet_sv0_ntrkj);
  if (!m_essentialInfo) tree->Branch("jet_sv0_ntrkv", &v_jet_sv0_ntrkv);
  if (!m_essentialInfo) tree->Branch("jet_sv0_n2t", &v_jet_sv0_n2t);
  tree->Branch("jet_sv0_m", &v_jet_sv0_m);
  if (!m_essentialInfo) tree->Branch("jet_sv0_efc", &v_jet_sv0_efc);
  if (!m_essentialInfo)  tree->Branch("jet_sv0_normdist", &v_jet_sv0_normdist);
  if (!m_essentialInfo) tree->Branch("jet_sv0_Nvtx", &v_jet_sv0_Nvtx);
  if (!m_essentialInfo) tree->Branch("jet_sv0_vtx_x", &v_jet_sv0_vtxx);
  if (!m_essentialInfo) tree->Branch("jet_sv0_vtx_y", &v_jet_sv0_vtxy);
  if (!m_essentialInfo) tree->Branch("jet_sv0_vtx_z", &v_jet_sv0_vtxz);

  if (!m_essentialInfo) tree->Branch("jet_sv1_ntrkj", &v_jet_sv1_ntrkj);
  if (!m_essentialInfo) tree->Branch("jet_sv1_ntrkv", &v_jet_sv1_ntrkv);
  if (!m_essentialInfo) tree->Branch("jet_sv1_n2t", &v_jet_sv1_n2t);
  if (!m_essentialInfo) tree->Branch("jet_sv1_m", &v_jet_sv1_m);
  if (!m_essentialInfo) tree->Branch("jet_sv1_efc", &v_jet_sv1_efc);
  if (!m_essentialInfo) tree->Branch("jet_sv1_normdist", &v_jet_sv1_normdist);
  if (!m_essentialInfo) tree->Branch("jet_sv1_pb", &v_jet_sv1_pb);
  if (!m_essentialInfo) tree->Branch("jet_sv1_pc", &v_jet_sv1_pc);
  if (!m_essentialInfo) tree->Branch("jet_sv1_pu", &v_jet_sv1_pu);
  tree->Branch("jet_sv1_llr", &v_jet_sv1_llr);
  tree->Branch("jet_sv1_Nvtx", &v_jet_sv1_Nvtx);
  if (!m_essentialInfo) tree->Branch("jet_sv1_sig3d", &v_jet_sv1_sig3d);
  tree->Branch("jet_sv1_vtx_x", &v_jet_sv1_vtxx);
  tree->Branch("jet_sv1_vtx_y", &v_jet_sv1_vtxy);
  if (!m_essentialInfo)  tree->Branch("jet_sv1_vtx_z", &v_jet_sv1_vtxz);

  if (!m_essentialInfo) tree->Branch("PV_jf_x", &PV_jf_x); // mod Remco
  if (!m_essentialInfo) tree->Branch("PV_jf_y", &PV_jf_y); // mod Remco
  if (!m_essentialInfo) tree->Branch("PV_jf_z", &PV_jf_z); // mod Remco

  if (!m_essentialInfo) tree->Branch("jet_jf_pb", &v_jet_jf_pb);
  if (!m_essentialInfo) tree->Branch("jet_jf_pc", &v_jet_jf_pc);
  if (!m_essentialInfo) tree->Branch("jet_jf_pu", &v_jet_jf_pu);
  tree->Branch("jet_jf_llr", &v_jet_jf_llr);
  if (!m_essentialInfo) tree->Branch("jet_jf_m", &v_jet_jf_m);
  if (!m_essentialInfo) tree->Branch("jet_jf_mUncorr",&v_jet_jf_mUncorr); //eloi
  if (!m_essentialInfo) tree->Branch("jet_jf_efc", &v_jet_jf_efc);
  if (!m_essentialInfo) tree->Branch("jet_jf_deta", &v_jet_jf_deta);
  if (!m_essentialInfo) tree->Branch("jet_jf_dphi", &v_jet_jf_dphi);
  if (!m_essentialInfo) tree->Branch("jet_jf_dRFlightDir",&v_jet_jf_dRFlightDir); //eloi
  if (!m_essentialInfo) tree->Branch("jet_jf_ntrkAtVx", &v_jet_jf_ntrkAtVx);
  if (!m_essentialInfo) tree->Branch("jet_jf_nvtx", &v_jet_jf_nvtx);
  if (!m_essentialInfo) tree->Branch("jet_jf_sig3d", &v_jet_jf_sig3d);
  if (!m_essentialInfo) tree->Branch("jet_jf_nvtx1t", &v_jet_jf_nvtx1t);
  if (!m_essentialInfo) tree->Branch("jet_jf_n2t", &v_jet_jf_n2t);
  if (!m_essentialInfo) tree->Branch("jet_jf_VTXsize", &v_jet_jf_VTXsize);
  if (!m_essentialInfo) tree->Branch("jet_jf_vtx_chi2", &v_jet_jf_vtx_chi2); // mod Remco
  if (!m_essentialInfo) tree->Branch("jet_jf_vtx_ndf", &v_jet_jf_vtx_ndf); // mod Remco
  if (!m_essentialInfo) tree->Branch("jet_jf_vtx_ntrk", &v_jet_jf_vtx_ntrk); // mod Remco
  if (!m_essentialInfo) tree->Branch("jet_jf_vtx_L3D", &v_jet_jf_vtx_L3d); // mod Remco
  if (!m_essentialInfo) tree->Branch("jet_jf_vtx_sig3D", &v_jet_jf_vtx_sig3d); // mod Remco
  //tree->Branch("jet_jf_vtx_nvtx", &v_jet_jf_vtx_nvtx); // mod Remco
  if (!m_essentialInfo) tree->Branch("jet_jf_phi", &v_jet_jf_phi); // mod Remco
  if (!m_essentialInfo) tree->Branch("jet_jf_theta", &v_jet_jf_theta); // mod Remco

  if (!m_essentialInfo) tree->Branch("jet_jfcombnn_pb", &v_jet_jfcombnn_pb);
  if (!m_essentialInfo) tree->Branch("jet_jfcombnn_pc", &v_jet_jfcombnn_pc);
  if (!m_essentialInfo) tree->Branch("jet_jfcombnn_pu", &v_jet_jfcombnn_pu);
  tree->Branch("jet_jfcombnn_llr", &v_jet_jfcombnn_llr);

  if (!m_essentialInfo) tree->Branch("jet_dl1_pb",&v_jet_dl1_pb);
  if (!m_essentialInfo) tree->Branch("jet_dl1_pc",&v_jet_dl1_pc);
  if (!m_essentialInfo) tree->Branch("jet_dl1_pu",&v_jet_dl1_pu);

  if (!m_essentialInfo) tree->Branch("jet_sv1ip3d", &v_jet_sv1ip3d);
  if (!m_essentialInfo) tree->Branch("jet_mv1", &v_jet_mv1);
  if (!m_essentialInfo) tree->Branch("jet_mv1c", &v_jet_mv1c);
  tree->Branch("jet_mv2c00", &v_jet_mv2c00);
  tree->Branch("jet_mv2c10", &v_jet_mv2c10);
  tree->Branch("jet_mv2c20", &v_jet_mv2c20);
  tree->Branch("jet_mv2c100", &v_jet_mv2c100);
  tree->Branch("jet_mv2cl100", &v_jet_mv2cl100);
  tree->Branch("jet_mv2m_pu", &v_jet_mv2m_pu);
  tree->Branch("jet_mv2m_pc", &v_jet_mv2m_pc);
  tree->Branch("jet_mv2m_pb", &v_jet_mv2m_pb);
  tree->Branch("jet_mvb", &v_jet_mvb);

  /*
  tree->Branch("jet_ip2dNT_llr", &v_jet_ip2dNT_llr);
  tree->Branch("jet_ip3dNT_llr", &v_jet_ip3dNT_llr);
  tree->Branch("jet_sv1Flip_llr", &v_jet_sv1flip_llr);
  tree->Branch("jet_jfFlip_llr", &v_jet_jfflip_llr);
  tree->Branch("jet_mv2c20Flip", &v_jet_mv2c20flip);
  */

  if (!m_reduceInfo && m_doMSV) {
    tree->Branch("jet_multisvbb1", &v_jet_multisvbb1);
    tree->Branch("jet_multisvbb2", &v_jet_multisvbb2);
    tree->Branch("jet_msv_N2Tpair", &v_jet_msv_N2Tpair);
    tree->Branch("jet_msv_energyTrkInJet", &v_jet_msv_energyTrkInJet);
    tree->Branch("jet_msv_nvsec", &v_jet_msv_nvsec);
    tree->Branch("jet_msv_normdist", &v_jet_msv_normdist);
    tree->Branch("jet_msv_vtx_cov0", &v_jet_msv_vtx_cov0);
    tree->Branch("jet_msv_vtx_cov1", &v_jet_msv_vtx_cov1);
    tree->Branch("jet_msv_vtx_cov2", &v_jet_msv_vtx_cov2);
    tree->Branch("jet_msv_vtx_cov3", &v_jet_msv_vtx_cov3);
    tree->Branch("jet_msv_vtx_cov4", &v_jet_msv_vtx_cov4);
    tree->Branch("jet_msv_vtx_cov5", &v_jet_msv_vtx_cov5);
    tree->Branch("jet_msv_vtx_mass", &v_jet_msv_vtx_mass);
    tree->Branch("jet_msv_vtx_efrc", &v_jet_msv_vtx_efrc);
    tree->Branch("jet_msv_vtx_ntrk", &v_jet_msv_vtx_ntrk);
    tree->Branch("jet_msv_vtx_pt", &v_jet_msv_vtx_pt);
    tree->Branch("jet_msv_vtx_eta", &v_jet_msv_vtx_eta);
    tree->Branch("jet_msv_vtx_phi", &v_jet_msv_vtx_phi);
    tree->Branch("jet_msv_vtx_dls", &v_jet_msv_vtx_dls);
    tree->Branch("jet_msv_vtx_x", &v_jet_msv_vtx_x);
    tree->Branch("jet_msv_vtx_y", &v_jet_msv_vtx_y);
    tree->Branch("jet_msv_vtx_z", &v_jet_msv_vtx_z);
    tree->Branch("jet_msv_vtx_chi", &v_jet_msv_vtx_chi);
    tree->Branch("jet_msv_vtx_ndf", &v_jet_msv_vtx_ndf);
  }

  if (!m_essentialInfo) tree->Branch("jet_ExKtbb_Hbb_DoubleMV2c20", &v_jet_ExKtbb_Hbb_DoubleMV2c20);
  if (!m_essentialInfo) tree->Branch("jet_ExKtbb_Hbb_SingleMV2c20", &v_jet_ExKtbb_Hbb_SingleMV2c20);
  if (!m_essentialInfo) tree->Branch("jet_ExKtbb_Hbb_MV2Only", &v_jet_ExKtbb_Hbb_MV2Only);
  if (!m_essentialInfo) tree->Branch("jet_ExKtbb_Hbb_MV2andJFDRSig", &v_jet_ExKtbb_Hbb_MV2andJFDRSig);
  if (!m_essentialInfo) tree->Branch("jet_ExKtbb_Hbb_MV2andTopos", &v_jet_ExKtbb_Hbb_MV2andTopos);

  if (!m_essentialInfo) tree->Branch("bH1_pt", &v_bH1_pt);
  if (!m_essentialInfo) tree->Branch("bH1_eta", &v_bH1_eta);
  if (!m_essentialInfo) tree->Branch("bH1_phi", &v_bH1_phi);
  if (!m_essentialInfo) tree->Branch("bH1_Lxy", &v_bH1_Lxy);
  if (!m_essentialInfo) tree->Branch("bH1_x", &v_bH1_x);
  if (!m_essentialInfo) tree->Branch("bH1_y", &v_bH1_y);
  if (!m_essentialInfo) tree->Branch("bH1_z", &v_bH1_z);
  if (!m_essentialInfo) tree->Branch("bH1_dRjet", &v_bH1_dRjet);

  if (!m_essentialInfo) tree->Branch("bH2_pt", &v_bH2_pt);
  if (!m_essentialInfo) tree->Branch("bH2_eta", &v_bH2_eta);
  if (!m_essentialInfo) tree->Branch("bH2_phi", &v_bH2_phi);
  if (!m_essentialInfo) tree->Branch("bH2_Lxy", &v_bH2_Lxy);
  if (!m_essentialInfo) tree->Branch("bH2_x", &v_bH2_x);
  if (!m_essentialInfo) tree->Branch("bH2_y", &v_bH2_y);
  if (!m_essentialInfo) tree->Branch("bH2_z", &v_bH2_z);
  if (!m_essentialInfo) tree->Branch("bH2_dRjet", &v_bH2_dRjet);

  tree->Branch("bH_pt", &v_bH_pt);
  tree->Branch("bH_eta", &v_bH_eta);
  if (!m_essentialInfo) tree->Branch("bH_phi", &v_bH_phi);
  tree->Branch("bH_Lxy", &v_bH_Lxy);
  if (!m_essentialInfo) tree->Branch("bH_x", &v_bH_x);
  if (!m_essentialInfo) tree->Branch("bH_y", &v_bH_y);
  if (!m_essentialInfo) tree->Branch("bH_z", &v_bH_z);
  tree->Branch("bH_dRjet", &v_bH_dRjet);
  if (!m_essentialInfo) tree->Branch("bH_nBtracks", &v_bH_nBtracks);
  if (!m_essentialInfo) tree->Branch("bH_nCtracks", &v_bH_nCtracks);

  tree->Branch("cH_pt", &v_cH_pt);
  tree->Branch("cH_eta", &v_cH_eta);
  if (!m_essentialInfo) tree->Branch("cH_phi", &v_cH_phi);
  tree->Branch("cH_Lxy", &v_cH_Lxy);
  if (!m_essentialInfo) tree->Branch("cH_x", &v_cH_x);
  if (!m_essentialInfo) tree->Branch("cH_y", &v_cH_y);
  if (!m_essentialInfo) tree->Branch("cH_z", &v_cH_z);
  tree->Branch("cH_dRjet", &v_cH_dRjet);
  if (!m_essentialInfo) tree->Branch("cH_nCtracks", &v_cH_nCtracks);

  tree->Branch("jet_btag_ntrk", &v_jet_btag_ntrk);
  if (!m_reduceInfo) {
    tree->Branch("jet_trk_pt", &v_jet_trk_pt);
    tree->Branch("jet_trk_eta", &v_jet_trk_eta);
    tree->Branch("jet_trk_theta", &v_jet_trk_theta);
    tree->Branch("jet_trk_phi", &v_jet_trk_phi);
    tree->Branch("jet_trk_qoverp", &v_jet_trk_qoverp);
    tree->Branch("jet_trk_dr", &v_jet_trk_dr);
    tree->Branch("jet_trk_assoc_msv", &v_jet_trk_assoc_msv);    // mod nikola
    tree->Branch("jet_trk_chi2", &v_jet_trk_chi2);
    tree->Branch("jet_trk_ndf", &v_jet_trk_ndf);
    tree->Branch("jet_trk_algo", &v_jet_trk_algo);
    tree->Branch("jet_trk_orig", &v_jet_trk_orig);

    tree->Branch("jet_trk_vtx_X", &v_jet_trk_vtx_X);
    tree->Branch("jet_trk_vtx_Y", &v_jet_trk_vtx_Y);
    //tree->Branch("jet_trk_vtx_dx", &v_jet_trk_vtx_dx);
    //tree->Branch("jet_trk_vtx_dy", &v_jet_trk_vtx_dy);

    tree->Branch("jet_trk_nInnHits", &v_jet_trk_nInnHits);
    tree->Branch("jet_trk_nNextToInnHits", &v_jet_trk_nNextToInnHits);
    tree->Branch("jet_trk_nBLHits", &v_jet_trk_nBLHits);
    tree->Branch("jet_trk_nsharedBLHits", &v_jet_trk_nsharedBLHits);
    tree->Branch("jet_trk_nsplitBLHits", &v_jet_trk_nsplitBLHits);
    tree->Branch("jet_trk_nPixHits", &v_jet_trk_nPixHits);
    tree->Branch("jet_trk_nsharedPixHits", &v_jet_trk_nsharedPixHits);
    tree->Branch("jet_trk_nsplitPixHits", &v_jet_trk_nsplitPixHits);
    tree->Branch("jet_trk_nSCTHits", &v_jet_trk_nSCTHits);
    tree->Branch("jet_trk_nsharedSCTHits", &v_jet_trk_nsharedSCTHits);
    tree->Branch("jet_trk_expectBLayerHit", &v_jet_trk_expectBLayerHit);

    tree->Branch("jet_trk_d0", &v_jet_trk_d0);
    tree->Branch("jet_trk_z0", &v_jet_trk_z0);
    tree->Branch("jet_trk_d0_truth", &v_jet_trk_d0_truth);
    tree->Branch("jet_trk_z0_truth", &v_jet_trk_z0_truth);
    tree->Branch("jet_trk_ip3d_grade", &v_jet_trk_IP3D_grade);
    tree->Branch("jet_trk_ip3d_d0", &v_jet_trk_IP3D_d0);
    tree->Branch("jet_trk_ip3d_d02D", &v_jet_trk_IP3D_d02D);
    tree->Branch("jet_trk_ip3d_z0", &v_jet_trk_IP3D_z0);
    tree->Branch("jet_trk_ip3d_d0sig", &v_jet_trk_IP3D_d0sig);
    tree->Branch("jet_trk_ip3d_z0sig", &v_jet_trk_IP3D_z0sig);

    tree->Branch("jet_trk_ip_d0", &v_jet_trk_ip_d0);
    tree->Branch("jet_trk_ip_z0", &v_jet_trk_ip_z0);

    tree->Branch("jet_trk_ip2d_llr", &v_jet_trk_IP2D_llr);
    tree->Branch("jet_trk_ip3d_llr", &v_jet_trk_IP3D_llr);

    tree->Branch("jet_trk_jf_Vertex", &v_jet_trk_jf_Vertex); // mod Remco
  }

  if (!m_essentialInfo) tree->Branch("jet_sv1_ntrk",&v_jet_sv1_ntrk);
  if (!m_essentialInfo) tree->Branch("jet_ip3d_ntrk",&v_jet_ip3d_ntrk);
  if (!m_essentialInfo) tree->Branch("jet_jf_ntrk",&v_jet_jf_ntrk);

  // MVb variables
  if (!m_essentialInfo) tree->Branch("jet_width", &v_jet_width);
  if (!m_essentialInfo) tree->Branch("jet_n_trk_sigd0cut", &v_jet_n_trk_sigd0cut);
  if (!m_essentialInfo) tree->Branch("jet_trk3_d0sig", &v_jet_trk3_d0sig);
  if (!m_essentialInfo) tree->Branch("jet_trk3_z0sig", &v_jet_trk3_z0sig);
  if (!m_essentialInfo) tree->Branch("jet_sv_scaled_efc", &v_jet_sv_scaled_efc);
  if (!m_essentialInfo) tree->Branch("jet_jf_scaled_efc", &v_jet_jf_scaled_efc);

  // additions by andrea
  if (m_SMT) {
    tree->Branch("jet_mu_assJet_pt", &v_jet_mu_assJet_pt);
    tree->Branch("jet_mu_truthflav", &v_jet_mu_truthflav);
    tree->Branch("jet_mu_dR", &v_jet_mu_dR);
    tree->Branch("jet_mu_pTrel", &v_jet_mu_pTrel);
    tree->Branch("jet_mu_qOverPratio", &v_jet_mu_qOverPratio);
    tree->Branch("jet_mu_mombalsignif", &v_jet_mu_mombalsignif);
    tree->Branch("jet_mu_scatneighsignif", &v_jet_mu_scatneighsignif);
    tree->Branch("jet_mu_VtxTyp", &v_jet_mu_VtxTyp);
    tree->Branch("jet_mu_pt", &v_jet_mu_pt);
    tree->Branch("jet_mu_eta", &v_jet_mu_eta);
    tree->Branch("jet_mu_phi", &v_jet_mu_phi);
    tree->Branch("jet_mu_d0", &v_jet_mu_d0);
    tree->Branch("jet_mu_z0", &v_jet_mu_z0);
    tree->Branch("jet_mu_parent_pdgid", &v_jet_mu_parent_pdgid);
    tree->Branch("jet_mu_ID_qOverP_var", &v_jet_mu_ID_qOverP_var);
    tree->Branch("jet_mu_muonType", &v_jet_mu_muonType);
    // additions by nikola
    tree->Branch("jet_mu_fatjet_nMu", &v_jet_mu_fatjet_nMu);
    tree->Branch("jet_mu_fatjet_pTmax_pT", &v_jet_mu_fatjet_pTmax_pT);
    tree->Branch("jet_mu_fatjet_pTmax_pTrel", &v_jet_mu_fatjet_pTmax_pTrel);
    tree->Branch("jet_mu_fatjet_pTmax_pTrelFrac", &v_jet_mu_fatjet_pTmax_pTrelFrac);
  }

  clearvectors();
  return StatusCode::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
StatusCode btagIBLAnalysisAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");

  // Write tree into file
  tree->Write();

  // Clean up
  CHECK( m_jetCleaningTool.release() );
  CHECK( m_jetCalibrationTool.release() );

  return StatusCode::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
StatusCode btagIBLAnalysisAlg::execute() {
  ATH_MSG_DEBUG ("Executing " << name() << "...");

  std::string triggerLogic = "HLT_j[0-9]+|L1_MBTS_1_1|L1_RD0_FILLED";

  if (v_L1triggerNames.size() == 0 && m_tdt != 0) {
    ///ATH_MSG_INFO ("Setting up trigger informations " << name() << "...");
    auto chainGroup = m_tdt->getChainGroup(m_triggerLogic);
    v_L1trigger = new bool[chainGroup->getListOfTriggers().size()];
    int count = -1;
    for (auto & trig : chainGroup->getListOfTriggers()) {
      count++;
      // std::cout << trig << std::endl;
      v_L1trigger[count] = 0;
      v_L1triggerNames.push_back(trig);
      tree->Branch(trig.c_str(), &(v_L1trigger[count]));
    }
  }
  clearvectors();

  //-------------------------
  // Event information
  //---------------------------
  const xAOD::EventInfo* eventInfo = 0;
  CHECK( evtStore()->retrieve(eventInfo) );

  // check if data or MC
  bool isData = !eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION);

  if (m_GRLname != "") {
    bool eventPasses = m_GRLSelectionTool->passRunLB(*eventInfo);
    if (!eventPasses) {
      ATH_MSG_DEBUG( " .... Event not passing GRL!!!");
      return StatusCode::SUCCESS;
    }
  }

  runnumber = eventInfo->runNumber();
  eventnumber = eventInfo->eventNumber();

  mcchannel = ( isData ? 0 : eventInfo->mcChannelNumber() );
  mcweight = ( isData ? 1 : eventInfo->mcEventWeight() );
  mu = eventInfo->averageInteractionsPerCrossing();
  Act_mu      = eventInfo->actualInteractionsPerCrossing();
  lbn = ( !isData ? 0 : eventInfo->lumiBlock() );

  larError = eventInfo->errorState(xAOD::EventInfo::LAr) == xAOD::EventInfo::Error;
  tileError = eventInfo->errorState(xAOD::EventInfo::Tile) == xAOD::EventInfo::Error;
  coreFlag = eventInfo->isEventFlagBitSet(xAOD::EventInfo::Core, 18);

  // Pileup reweighting for mc:
  //  if (strcmp(m_jetCollectionName.c_str(), "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets"))
  //  {
  //    puweight = 1;
  //    puweight = m_tool->getCombinedWeight( *eventInfo );
  float tmpMu = m_PUtool->getLumiBlockMu( *eventInfo );
  // std::cout << " origMu: " << mu << " newValue: " <<  tmpMu << std::endl;
  if (isData) mu = tmpMu;
  //  }
  
  // primary vertex
  const xAOD::VertexContainer *vertices = 0;
  CHECK( evtStore()->retrieve(vertices, "PrimaryVertices") );
  npv = 0;
  int m_indexPV = -1;
  xAOD::VertexContainer::const_iterator vtx_itr = vertices->begin();
  xAOD::VertexContainer::const_iterator vtx_end = vertices->end();
  int count = -1;
  for (; vtx_itr != vtx_end; ++vtx_itr) {
    count++;
    if ((*vtx_itr)->nTrackParticles() >= 2) {
      v_PVz->push_back(  (*vtx_itr)->z() );
      npv++;
      if ((*vtx_itr)->vertexType() == 1) {
	if (PV_x != -999) ATH_MSG_WARNING( ".... second PV in the events ...!!!!!!");
	m_indexPV = count;
	PV_x = (*vtx_itr)->x(); // VALERIO !!!!!!!!
 	PV_y = (*vtx_itr)->y(); // VALERIO !!!!!!!!
	PV_z = (*vtx_itr)->z();
      }
    }
  }
  if (m_indexPV == -1) {
    ATH_MSG_WARNING( ".... rejecting the event due to missing PV!!!!");
    return StatusCode::SUCCESS;
  }
  const xAOD::Vertex *myVertex = vertices->at(m_indexPV); // the (reco?) primary vertex

  //---------------------------------------------------------------------------------------------
  // Trigger part
  //---------------------------------------------------------------------------------------------
  if (m_tdt != 0) {
    bool passTrigger = false;
    for (unsigned int it = 0; it < v_L1triggerNames.size(); it++) {
      v_L1trigger[it] = m_tdt->isPassed(v_L1triggerNames[it]);
      if (v_L1trigger[it]) passTrigger = true;

//      auto cg = m_tdt->getChainGroup(v_L1triggerNames[it]);
//      auto fc = cg->features();
//      auto L1jetFeatureContainers = fc.containerFeature<xAOD::JetRoIContainer>();
//      for (auto jLV1 : *L1jetFeatureContainers.cptr()) {
//        std::cout << " L1ROI: " << jLV1->pt() / 1e3 << " : " << jLV1->pt() << std::endl;
//      }
    }

    if (isData) {
      if (!passTrigger) {
//        ATH_MSG_WARNING(" .... Event not passing any of the selected triggers!");
        return StatusCode::SUCCESS;
      }
    }
  }

  const xAOD::TrackParticleContainer *trackParticles = 0;
  CHECK( evtStore()->retrieve(trackParticles, "InDetTrackParticles") );
  int n_putracks = 0;
  for (const auto* trk : *trackParticles) {
    if ( m_InDetTrackSelectorTool->accept(*trk, myVertex) && trk->vertex() && trk->vertex() != myVertex && trk->pt() < 30e3) n_putracks++;
  }
  if (!n_putracks) n_putracks++;

  if (!isData) {
    const xAOD::TruthVertexContainer *truthVertices = 0;
    CHECK( evtStore()->retrieve(truthVertices, "TruthVertices") );
    for ( const auto* truthV : *truthVertices ) {
      // record the truth primary vertex position
      truth_PV_x = truthV->x();
      truth_PV_y = truthV->y();
      truth_PV_z = truthV->z();
      break;
      // std::cout << " truthV: z " << truthV->z() << " while PV: " << PV_z << std::endl;
    }
  }

  //---------------------------
  // Truth stuff
  //---------------------------
  const xAOD::TruthEventContainer *xTruthEventContainer = NULL;
  std::string truthevt = "TruthEvent";
  if (m_rel20) truthevt = "TruthEvents";
  if (!isData) CHECK( evtStore()->retrieve(xTruthEventContainer, truthevt) );

  std::vector<const xAOD::TruthParticle* > m_partonB;
  std::vector<const xAOD::TruthParticle* > m_partonC;
  std::vector<const xAOD::TruthParticle* > m_partonT;
  std::vector<TLorentzVector> truth_electrons;
  std::vector<TLorentzVector> truth_muons;
  
  if (!isData) {
    // select truth electrons for electron-jet overlap removal
    for ( const auto* truth : *xTruthEventContainer ) {
      const xAOD::TruthVertex *newTruthVertex = truth->signalProcessVertex();
      if (newTruthVertex != 0) {
        // std::cout << " mah: " << newTruthVertex << std::endl;
        // rewrite the truth primary vertex position
	truth_PV_x = newTruthVertex->x();
	truth_PV_y = newTruthVertex->y();
	truth_PV_z = newTruthVertex->z();
      }
      
      // loop over truth particles in the truth event container
      for(unsigned int i = 0; i < truth->nTruthParticles(); i++) {
	const xAOD::TruthParticle *particle = truth->truthParticle(i);
	// VALERIO !!!!!!!!
	if (particle->pt() > 3e3) {
	  if (fabs(particle->pdgId()) == 15) m_partonT.push_back(particle);
	  if (particle->isCharmHadron()) m_partonC.push_back(particle);
	  if (particle->isBottomHadron()) m_partonB.push_back(particle);
	}
	if (particle->pt() < 10e3)     continue; //was 15
	if (particle->status() != 1)   continue;
	if (particle->barcode() > 2e5) continue;
	
	if ( fabs(particle->pdgId()) != 11 && fabs(particle->pdgId()) != 13) continue;
	
	//std::cout << " Lepton=  pt: " << particle->pt()/1e3 << "  eta: " << particle->eta() 
	//<< " pdg: " << particle->pdgId() << " status: " << particle->status() 
	//<< "  barcode: " << particle->barcode() << "  --> nIncoming: " << particle->prodVtx()->nIncomingParticles() << std::endl;

	// see if this electron is coming from a W boson decay
	bool isfromW = isFromWZ( particle );

	/*
	const xAOD::TruthVertex* prodvtx = particle->prodVtx();
	for(unsigned j = 0; j < prodvtx->nIncomingParticles(); j++){
	  std::cout << "    mother=  pt: " << prodvtx->incomingParticle(j)->pt()/1e3 << "  eta: " << prodvtx->incomingParticle(j)->eta() 
		    << " pdg: " << prodvtx->incomingParticle(j)->pdgId() << " status: " << prodvtx->incomingParticle(j)->status() 
		    << "  barcode: " << prodvtx->incomingParticle(j)->barcode() << std::endl;
	  //<< "  --> nIncoming: " << prodvtx->incomingParticle(j)->prodVtx()->nIncomingParticles() << std::endl;
	  //int absPDG=fabs( (prodvtx->incomingParticle(j))->pdgId() );
	  //if( absPDG==24 || absPDG==23 ) isfromW = true;
	*/

	if(!isfromW) continue;
	TLorentzVector telec;
	telec.SetPtEtaPhiM(particle->pt(), particle->eta(), particle->phi(), particle->m());
	
	if (fabs(particle->pdgId()) == 11) truth_electrons.push_back(telec);
	if (fabs(particle->pdgId()) == 13) truth_muons.push_back(telec);
      }
    }
  }

  //---------------------------
  // Jets
  //---------------------------
  const xAOD::JetContainer *jets = 0;
  CHECK( evtStore()->retrieve(jets, m_jetCollectionName) );
  const xAOD::JetContainer *truthjets = 0;
  if (!isData) {
    CHECK( evtStore()->retrieve( truthjets, "AntiKt4TruthJets") );
    truth_LeadJet_pt=-1;
    if (truthjets->size()!=0) truth_LeadJet_pt=(truthjets->at(0))->pt();
  }
  else truthjets = new xAOD::JetContainer();
  njets = 0;
  nbjets = 0;
  nbjets_q = 0;
  nbjets_HadI = 0;
  nbjets_HadF = 0;

  // keep in mind, jets will not have tracks if their eta is greater than 2.5
  float etaCut = 3.0;
  if (isData) etaCut = 2.5;

  // Loop over jets, apply calibration and only keep the ones with pT > XX
  std::vector<const xAOD::Jet*> selJets; selJets.reserve(10);

  bool badCleaning = false;
  for (const auto* jet : *jets) {
    xAOD::Jet *newjet = 0;

    if (m_calibrateJets) m_jetCalibrationTool->calibratedCopy(*jet, newjet);
    else newjet = new xAOD::Jet(*jet);

    // jet cleaning - should be done after lepton overlap removal
    /*
    if (m_cleanJets) {
      if ( (!m_jetCleaningTool->keep(*newjet)) && (newjet->pt() > 20e3) ) {
        delete newjet;
        badCleaning = true;
        return StatusCode::SUCCESS;
        break;
      }
    }
    */

    if (newjet->pt() < m_jetPtCut) {
      delete newjet;
      continue;
    }
    if ( fabs(newjet->eta()) > etaCut ) {
      delete newjet;
      continue;
    }
    v_jet_pt_orig->push_back(jet->pt());
    v_jet_eta_orig->push_back(jet->eta());
    v_jet_phi_orig->push_back(jet->phi());
    v_jet_E_orig ->push_back(jet->e());
    selJets.push_back(newjet);
  }
  if (badCleaning) {
    for (unsigned int j = 0; j < selJets.size(); j++) {
      delete selJets.at(j);
    }
    return StatusCode::SUCCESS;
  }
  std::vector<const xAOD::Jet*> selJetsTMP = selJets;
  std::sort(selJets.begin(), selJets.end(), xaodJetPtSorting);
  std::vector<float> tmpPt = std::vector<float>(*v_jet_pt_orig);
  std::vector<float> tmpEta = std::vector<float>(*v_jet_eta_orig);
  std::vector<float> tmpPhi = std::vector<float>(*v_jet_phi_orig);
  std::vector<float> tmpE = std::vector<float>(*v_jet_E_orig);
  v_jet_pt_orig->clear();
  v_jet_eta_orig->clear();
  v_jet_phi_orig->clear();
  v_jet_E_orig->clear();
  for (unsigned int j = 0; j < selJets.size(); j++) {
    const xAOD::Jet *jet = selJets.at(j);
    int oIndex = -1;
    for (unsigned int j2 = 0; j2 < selJets.size(); j2++) {
      if (selJetsTMP.at(j2)->pt() == jet->pt()) {
	oIndex = j2;
	break;
      }
    }
    v_jet_pt_orig->push_back(tmpPt[oIndex]);
    v_jet_eta_orig->push_back(tmpEta[oIndex]);
    v_jet_phi_orig->push_back(tmpPhi[oIndex]);
    v_jet_E_orig->push_back(tmpE[oIndex]);
  }

  njets = selJets.size();
  ATH_MSG_DEBUG( "Total number of jets is: "<< njets );
  uint8_t getInt(0);   // for accessing summary information

  /////////////////////////////////////////////////////////////////////////////////
  // MAIN JET LOOP
  // Now run over the selected jets and do whatever else needs doing
  for (unsigned int j = 0; j < selJets.size(); j++) {
    const xAOD::Jet *jet = selJets.at(j);

    // addition from Dan: fill clusters
    if (m_dumpCaloInfo) {
      m_cluster_branches.fill(jet->getConstituents());
      m_substructure_moment_branches.fill(*jet);
    }

    // additions by nikola
    const xAOD::Jet *jet_parent = 0;
    if (strcmp(m_jetCollectionName.c_str(), "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets") == 0 || strcmp(m_jetCollectionName.c_str(), "Akt10LCTopoTrmJets") == 0) {
      jet_parent = GetParentJet(jet, "Parent");
    }

    // protection against not properly filled objects ... IT SHOULD NEVER HAPPEN THOUGH
    try {
      const std::vector<ElementLink<xAOD::VertexContainer > > TMPvertices = jet->btagging()->auxdata<std::vector<ElementLink<xAOD::VertexContainer > > >("SV1_vertices");
    } catch(...) {
      std::vector< ElementLink< xAOD::TrackParticleContainer > > TMPassocTracks = jet->btagging()->auxdata<std::vector<ElementLink<xAOD::TrackParticleContainer> > >("BTagTrackToJetAssociator");
      ATH_MSG_WARNING(" THIS JET: " << jet->pt() << " eta: " << jet->eta() << " IS NOT FILLED PROPERLY!!!!!!!!!!!!! and contains: " << TMPassocTracks.size() << " tracks  .... PLEASE CHECK");
      njets-=1;
      continue;
    }

    /////////////////////////////////////////////////////////////////////////////////
    // flagging jets that overlap with electron
    bool iseljetoverlap = false;
    for(unsigned int i = 0; i < truth_electrons.size(); i++) {
      float dr = deltaR(jet->eta(), truth_electrons.at(i).Eta(), jet->phi(), truth_electrons.at(i).Phi());
      if (dr < 0.3) iseljetoverlap = true;
    }
    v_jet_aliveAfterOR->push_back( !iseljetoverlap );

    iseljetoverlap = false;
    for(unsigned int i= 0; i < truth_muons.size(); i++){
      float dr =deltaR(jet->eta(), truth_muons.at(i).Eta(),jet->phi(), truth_muons.at(i).Phi());
      if(dr < 0.3) iseljetoverlap = true;
    }
    v_jet_aliveAfterORmu->push_back( !iseljetoverlap );
    // jet cleaning - should be done after lepton overlap removal
    // if( (!m_jetCleaningTool->keep(*jet)) && (jet->pt() > 20e3) ) return StatusCode::SUCCESS;

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // simple kinematic quantities + PUinfo + JVF/JVT
    v_jet_pt->push_back(jet->pt());
    v_jet_eta->push_back(jet->eta());
    v_jet_phi->push_back(jet->phi());
    v_jet_E->push_back(jet->e());
    v_jet_m->push_back(jet->m());
    v_jet_nConst->push_back( jet->numConstituents() );
    float dRiso = 10;
    for (unsigned int j2 = 0; j2 < selJets.size(); j2++) {
      if (j2 == j) continue;
      const xAOD::Jet *jet2 = selJets.at(j2);
      float dr = deltaR(jet->eta(), jet2->eta(), jet->phi(), jet2->phi());
      if (dr < dRiso) dRiso = dr;
    }
    v_jet_dRiso->push_back(dRiso);

    // matching reco jets to truth jets and recording the truth jet pT
    // picking the highest pT truth jet (with pT > 7GeV) that satisfies dR < 0.3
    // N.B. this assumes that truth jets are pT ordered
    int matchedPt = 0;
    float dRmatch = 100;
    for (const auto* tjet : *truthjets) {
      if (tjet->pt() < 10e3) continue;
      float dr = deltaR(jet->eta(), tjet->eta(), jet->phi(), tjet->phi());

      if (dr < 0.3) {
	dRmatch = dr;
	matchedPt = tjet->pt();
	break;
      }
    }
    if (dRmatch < 0.3) {
      v_jet_truthMatch->push_back(1);
      v_jet_truthPt->push_back(matchedPt);
    }
    else {
      v_jet_truthMatch->push_back(0);
      v_jet_truthPt->push_back(0);
    }

    // flagging as PU jet: no HS jets within 0.6
    bool truthFree = true;
    for (const auto* tjet : *truthjets) {
      if (tjet->pt() < 4e3) continue;
      float dr = deltaR(jet->eta(), tjet->eta(), jet->phi(), tjet->phi());

      if (dr < 0.6) {
	truthFree = false;
	break;
      }
    }
    v_jet_isPU->push_back(truthFree);

    // clean jets
    if (m_cleanJets) {
      const xAOD::Jet *jet_to_clean = jet;
      // additions by nikola
      if (strcmp(m_jetCollectionName.c_str(), "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets") == 0 || strcmp(m_jetCollectionName.c_str(), "Akt10LCTopoTrmJets") == 0) {
        jet_to_clean = jet_parent;
      }
      v_jet_isBadMedium->push_back(!m_jetCleaningTool->keep(*jet_to_clean));
    }
    else v_jet_isBadMedium->push_back(0);

    // JVF
    float jvfV = 0;
    std::vector<float> testjvf;
    bool jetHasJVF = jet->getAttribute<std::vector<float> >(xAOD::JetAttribute::JVF, testjvf);
    if (jetHasJVF && testjvf.size() > m_indexPV) jvfV = testjvf.at(m_indexPV);

    v_jet_JVF->push_back(jvfV);
    // JVT
    float jvtV = 0;
    try {
      jvtV = jet->auxdata<float>("Jvt");
      float tmpJVT = jvtV;
      jvtV = m_jvt->updateJvt(*jet);
      if (tmpJVT != jvtV)  ATH_MSG_DEBUG(" initial: " << tmpJVT << " |  final: " << jvtV );
    } catch (...) {
      ATH_MSG_WARNING(" something went wrong with the JVT recalculation .... please investigate");
    };
    v_jet_JVT->push_back(jvtV);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // all the possible labeling
    int thisJetTruthLabel = -1;
    if (m_rel20) thisJetTruthLabel = jetFlavourLabel(jet, xAOD::ConeFinalParton);
    else jet->getAttribute("TruthLabelID", thisJetTruthLabel);

    v_jet_truthflav->push_back(thisJetTruthLabel);
    if (thisJetTruthLabel == 5) nbjets++;

    int tmpLabel = GAFinalPartonFlavourLabel(jet);
    v_jet_GhostL_q->push_back(tmpLabel);
    if (tmpLabel == 5) nbjets_q++;

    tmpLabel = GAInitialHadronFlavourLabel(jet);
    v_jet_GhostL_HadI->push_back(tmpLabel);
    if (tmpLabel == 5) nbjets_HadI++;

    tmpLabel = GAFinalHadronFlavourLabel(jet);
    v_jet_GhostL_HadF->push_back(tmpLabel);
    if (tmpLabel == 5) nbjets_HadF++;

    tmpLabel = -1;
    try {
      jet->getAttribute("HadronConeExclTruthLabelID", tmpLabel);
    } catch(...) {};
    v_jet_LabDr_HadF->push_back(tmpLabel);

    // requested by P.Berta
    float mindRtoB = 10;
    float mindRtoC = 10;
    float mindRtoT = 10;
    for (unsigned int ip = 0; ip < m_partonB.size(); ip++) {
      float dr = deltaR(jet->eta(), (m_partonB.at(ip))->eta(), jet->phi(), (m_partonB.at(ip))->phi());
      if (dr < mindRtoB) mindRtoB = dr;
    }
    for (unsigned int ip = 0; ip < m_partonC.size(); ip++) {
      float dr = deltaR(jet->eta(), (m_partonC.at(ip))->eta(), jet->phi(), (m_partonC.at(ip))->phi());
      if (dr < mindRtoC) mindRtoC = dr;
    }
    for (unsigned int ip = 0; ip < m_partonT.size(); ip++) {
      float dr = deltaR(jet->eta(), (m_partonT.at(ip))->eta(), jet->phi(), (m_partonT.at(ip))->phi());
      if (dr < mindRtoT) mindRtoT = dr;
    }
    v_jet_dRminToB->push_back(mindRtoB);
    v_jet_dRminToC->push_back(mindRtoC);
    v_jet_dRminToT->push_back(mindRtoT);

    // continue;  // VALERIO !!!!!!!!

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // get truth particles of matched B Hadrons, C Hadrons, and other particles
    // for single b tagging
    const xAOD::TruthParticle *matchedBH = NULL;
    const xAOD::TruthParticle *matchedCH = NULL;

    // additions by nikola
    const xAOD::TruthParticle *matchedBH1 = NULL;
    const xAOD::TruthParticle *matchedBH2 = NULL;
    const xAOD::TruthParticle *matchedCNotFromB1 = NULL;
    const xAOD::TruthParticle *matchedCNotFromB2 = NULL;
    // double b-tagging (on trimmed large-R jets, AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets)
    if (strcmp(m_jetCollectionName.c_str(), "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets") == 0 || strcmp(m_jetCollectionName.c_str(), "Akt10LCTopoTrmJets") == 0) {
      // get ghost B Hadrons from parent jet
      std::vector<const IParticle*> ghostBFromParent; ghostBFromParent.reserve(2);
      jet_parent->getAssociatedObjects<IParticle>("GhostBHadronsFinal", ghostBFromParent);
      v_jet_nGhostBHadrFromParent->push_back(ghostBFromParent.size());    // the number of ghost B Hadrons from parent jet

      // use LEADING 2 ghost B Hadrons from parent jet to later label b-tagging tracks
      if (ghostBFromParent.size() >= 1) {
        matchedBH1 = (const xAOD::TruthParticle*)(ghostBFromParent.at(0));
        if (ghostBFromParent.size() >= 2) {
          matchedBH2=(const xAOD::TruthParticle*)(ghostBFromParent.at(1));
        }
      }

      // get ghost C Hadrons from parent jet
      std::vector<const IParticle*> ghostCFromParent; ghostCFromParent.reserve(2);
      jet_parent->getAssociatedObjects<IParticle>("GhostCHadronsFinal", ghostCFromParent);
      v_jet_nGhostCHadrFromParent->push_back(ghostCFromParent.size());    // the number of ghost C Hadrons from parent jet

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
           if (matchedCNotFromB1 == NULL) matchedCNotFromB1 = cHadron;
           else if (matchedCNotFromB2 == NULL) matchedCNotFromB2 = cHadron;
           else ATH_MSG_INFO ("more than 2 C Hadrons which do not come from a B Hadron have been found...");
         }
      }
      v_jet_nGhostCHadrFromParentNotFromB->push_back(nGhostCHadrFromParentNotFromB);    // the number of ghost C Hadrons from parent jet which are NOT children of ghost B Hadrons from parent jet

      // ghost Tau
      std::vector<const IParticle*> ghostTauFromParent; ghostTauFromParent.reserve(2);
      jet_parent->getAssociatedObjects<IParticle>("GhostTausFinal", ghostTauFromParent);
      v_jet_nGhostTauFromParent->push_back(ghostTauFromParent.size());

      // ghost H
      std::vector<const IParticle*> ghostHFromParent; ghostHFromParent.reserve(2);
      jet_parent->getAssociatedObjects<IParticle>("GhostHBosons", ghostHFromParent);
      v_jet_nGhostHBosoFromParent->push_back(ghostHFromParent.size());
    }
    // single b-tagging
    else {
      // get B hadron quantities
      const std::string labelB = "ConeExclBHadronsFinal";
      std::vector<const IParticle*> ghostB; ghostB.reserve(2);
      jet->getAssociatedObjects<IParticle>(labelB, ghostB);
      if (ghostB.size() >=1 ) {
         matchedBH = (const xAOD::TruthParticle*)(ghostB.at(0));
         // to do: in case of 2, get the closest
      }
      v_jet_nBHadr->push_back(ghostB.size());

      // get C hadron quantities
      const std::string labelC = "ConeExclCHadronsFinal";
      std::vector<const IParticle*> ghostC; ghostC.reserve(2);
      jet->getAssociatedObjects<IParticle>(labelC, ghostC);
      if (ghostC.size() >=1 ) {
         matchedCH = (const xAOD::TruthParticle*)(ghostC.at(0));
         // to do: in case of 2, get the closest
      }
      v_jet_nCHadr->push_back(ghostC.size());
    }

    // get tracks from matched B and C Hadrons
    // additions by nikola
    // double b-tagging
    std::vector<const xAOD::TruthParticle*> tracksFromB1;
    std::vector<const xAOD::TruthParticle*> tracksFromB2;
    std::vector<const xAOD::TruthParticle*> tracksFromC1;
    std::vector<const xAOD::TruthParticle*> tracksFromC2;
    std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB1;
    std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB2;
    if (matchedBH1 != NULL) {
      GetParentTracks(matchedBH1, tracksFromB1, tracksFromC1, false, "  ");
    }
    if (matchedBH2 != NULL) {
      GetParentTracks(matchedBH2, tracksFromB2, tracksFromC2, false, "  ");
    }
    if (matchedCNotFromB1 != NULL) {
      GetParentTracks(matchedCNotFromB1, tracksFromCNotFromB1, tracksFromCNotFromB1, false, "  ");
    }
    if (matchedCNotFromB2 != NULL) {
      GetParentTracks(matchedCNotFromB2, tracksFromCNotFromB2, tracksFromCNotFromB2, false, "  ");
    }

    // single b-tagging
    std::vector<const xAOD::TruthParticle*> tracksFromB;
    std::vector<const xAOD::TruthParticle*> tracksFromC;
    std::vector<const xAOD::TruthParticle*> tracksFromCc;

    // nikola to-do: make this more elegant (maybe loop over all B Hadrons?) maybe add C1 and C2 info
    if (matchedBH1 != NULL) {
      v_bH1_pt->push_back(matchedBH1->pt());
      v_bH1_eta->push_back(matchedBH1->eta());
      v_bH1_phi->push_back(matchedBH1->phi());
      float Lxy = sqrt( pow(matchedBH1->decayVtx()->x(), 2) + pow(matchedBH1->decayVtx()->y(), 2) );
      v_bH1_Lxy->push_back(Lxy);
      v_bH1_x->push_back(matchedBH1->decayVtx()->x());
      v_bH1_y->push_back(matchedBH1->decayVtx()->y());
      v_bH1_z->push_back(matchedBH1->decayVtx()->z());
      float dr = deltaR(jet->eta(), matchedBH1->eta(), jet->phi(), matchedBH1->phi());
      v_bH1_dRjet->push_back(dr);
    }
    else {
      v_bH1_pt->push_back(-999);
      v_bH1_eta->push_back(-999);
      v_bH1_phi->push_back(-999);
      v_bH1_Lxy->push_back(-999);
      v_bH1_dRjet->push_back(-999);
      v_bH1_x->push_back(-999);
      v_bH1_y->push_back(-999);
      v_bH1_z->push_back(-999);
    }
    if (matchedBH2 != NULL) {
      v_bH2_pt->push_back(matchedBH2->pt());
      v_bH2_eta->push_back(matchedBH2->eta());
      v_bH2_phi->push_back(matchedBH2->phi());
      float Lxy = sqrt( pow(matchedBH2->decayVtx()->x(), 2) + pow(matchedBH2->decayVtx()->y(), 2) );
      v_bH2_Lxy->push_back(Lxy);
      v_bH2_x->push_back(matchedBH2->decayVtx()->x());
      v_bH2_y->push_back(matchedBH2->decayVtx()->y());
      v_bH2_z->push_back(matchedBH2->decayVtx()->z());
      float dr = deltaR(jet->eta(), matchedBH2->eta(), jet->phi(), matchedBH2->phi());
      v_bH2_dRjet->push_back(dr);
    }
    else {
      v_bH2_pt->push_back(-999);
      v_bH2_eta->push_back(-999);
      v_bH2_phi->push_back(-999);
      v_bH2_Lxy->push_back(-999);
      v_bH2_dRjet->push_back(-999);
      v_bH2_x->push_back(-999);
      v_bH2_y->push_back(-999);
      v_bH2_z->push_back(-999);
    }
    if (matchedBH != NULL) {
      v_bH_pt->push_back(matchedBH->pt());
      v_bH_eta->push_back(matchedBH->eta());
      v_bH_phi->push_back(matchedBH->phi());
      float Lxy = sqrt( pow(matchedBH->decayVtx()->x(), 2) + pow(matchedBH->decayVtx()->y(), 2) );
      v_bH_Lxy->push_back(Lxy);
      v_bH_x->push_back(matchedBH->decayVtx()->x());
      v_bH_y->push_back(matchedBH->decayVtx()->y());
      v_bH_z->push_back(matchedBH->decayVtx()->z());
      float dr = deltaR(jet->eta(), matchedBH->eta(), jet->phi(), matchedBH->phi());
      v_bH_dRjet->push_back(dr);
      GetParentTracks(matchedBH, tracksFromB, tracksFromC, false, "  ");
    }
    else {
      v_bH_pt->push_back(-999);
      v_bH_eta->push_back(-999);
      v_bH_phi->push_back(-999);
      v_bH_Lxy->push_back(-999);
      v_bH_dRjet->push_back(-999);
      v_bH_x->push_back(-999);
      v_bH_y->push_back(-999);
      v_bH_z->push_back(-999);
    }
    v_bH_nBtracks->push_back(tracksFromB.size() - tracksFromC.size());
    v_bH_nCtracks->push_back(tracksFromC.size());

    if (matchedCH != NULL &&  matchedBH == NULL) {
      v_cH_pt->push_back(matchedCH->pt());
      v_cH_eta->push_back(matchedCH->eta());
      v_cH_phi->push_back(matchedCH->phi());
      float Lxy = sqrt( pow(matchedCH->decayVtx()->x(), 2) + pow(matchedCH->decayVtx()->y(), 2) );
      v_cH_Lxy->push_back(Lxy);
      v_cH_x->push_back(matchedCH->decayVtx()->x());
      v_cH_y->push_back(matchedCH->decayVtx()->y());
      v_cH_z->push_back(matchedCH->decayVtx()->z());
      float dr = deltaR(jet->eta(), matchedCH->eta(), jet->phi(), matchedCH->phi());
      v_cH_dRjet->push_back(dr);
      GetParentTracks(matchedCH, tracksFromCc, tracksFromCc, false, "  ");
    }
    else {
      v_cH_pt->push_back(-999);
      v_cH_eta->push_back(-999);
      v_cH_phi->push_back(-999);
      v_cH_Lxy->push_back(-999);
      v_cH_dRjet->push_back(-999);
      v_cH_x->push_back(-999);
      v_cH_y->push_back(-999);
      v_cH_z->push_back(-999);
    }
    v_cH_nCtracks->push_back(tracksFromCc.size());

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Get b-tag object and fill b-tagging information
    const xAOD::BTagging *bjet = jet->btagging();

    //std::cout << " got Btagging: " << std::endl;

    // IP2D
    std::vector< ElementLink< xAOD::TrackParticleContainer > > IP2DTracks;
    IP2DTracks = bjet->auxdata<std::vector<ElementLink< xAOD::TrackParticleContainer> > >("IP2D_TrackParticleLinks");
    if (IP2DTracks.size() > 0) {
      v_jet_ip2d_pb->push_back(bjet->IP2D_pb());
      v_jet_ip2d_pc->push_back(bjet->IP2D_pc());
      v_jet_ip2d_pu->push_back(bjet->IP2D_pu());
      v_jet_ip2d_llr->push_back(bjet->IP2D_loglikelihoodratio());
    }
    else {
      v_jet_ip2d_pb->push_back(-99);
      v_jet_ip2d_pc->push_back(-99);
      v_jet_ip2d_pu->push_back(-99);
      v_jet_ip2d_llr->push_back(-99);
    }

    // IP3D
    std::vector< ElementLink< xAOD::TrackParticleContainer > > IP3DTracks;
    IP3DTracks = bjet->auxdata<std::vector<ElementLink< xAOD::TrackParticleContainer> > >("IP3D_TrackParticleLinks");
    if (IP3DTracks.size()) {
      v_jet_ip3d_pb->push_back(bjet->IP3D_pb());
      v_jet_ip3d_pc->push_back(bjet->IP3D_pc());
      v_jet_ip3d_pu->push_back(bjet->IP3D_pu());
      v_jet_ip3d_llr->push_back(bjet->IP3D_loglikelihoodratio());
    }
    else {
      v_jet_ip3d_pb->push_back(-99);
      v_jet_ip3d_pc->push_back(-99);
      v_jet_ip3d_pu->push_back(-99);
      v_jet_ip3d_llr->push_back(-99);
    }

    /*
    // this was an attempt to read from the object directly
    try {
      v_jet_sumtrk_pt->push_back(tmpVal2);
      //bjet->variable<float>("IP3D", "trkSum_VPt", tmpVal);
      tmpVal = bjet->auxdata<float>("trkSum_VPt");
      v_jet_sumtrkV_pt->push_back(tmpVal);
      //bjet->variable<float>("IP3D", "trkSum_VEta", tmpVal);
      tmpVal = bjet->auxdata<float>("trkSum_VEta");
      v_jet_sumtrkV_eta->push_back(tmpVal);
    } catch (...) {};
    */

    float tmp_trkSum_SPt = 0;
    float tmp_trkSum_VPt = 0;
    float tmp_trkSum_VEta = 0;
    float tmp_trkSum_VPhi = 0;
    int tmp_trkSum_nTrk = 0;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // getting manual distribution on the number of tracks per jet with the selection
    std::vector< ElementLink< xAOD::TrackParticleContainer > > assocTracks;

    // get tracks from different track<->jet associators for trimmed large-R jets vs other jet collections
    if (strcmp(m_jetCollectionName.c_str(), "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets") == 0 || strcmp(m_jetCollectionName.c_str(), "Akt10LCTopoTrmJets") == 0) {
      assocTracks = bjet->auxdata<std::vector<ElementLink<xAOD::TrackParticleContainer> > >("BTagTrackToJetAssociatorBB");
    }
    else {
      assocTracks = bjet->auxdata<std::vector<ElementLink<xAOD::TrackParticleContainer> > >("BTagTrackToJetAssociator");
    }

    // build vector of tracks to simplify interface
    std::vector<const xAOD::TrackParticle*> associated_tracks;
    // temporary track loop - sums up the 4vectors of all valid b-tag tracks and outputs
    TLorentzVector pseudoTrackJet(0, 0, 0, 0);
    for (unsigned int iT = 0; iT < assocTracks.size(); iT++) {
      if (!assocTracks.at(iT).isValid()) continue;
      const xAOD::TrackParticle *tmpTrk = *(assocTracks.at(iT));
      associated_tracks.push_back(tmpTrk);

      if (m_InDetTrackSelectorTool->accept(*tmpTrk, myVertex) && m_TightTrackVertexAssociationTool->isCompatible(*tmpTrk, *myVertex) ) {
	TLorentzVector tmpTrack(0, 0, 0, 0);
	tmpTrack.SetPtEtaPhiM(tmpTrk->pt(), tmpTrk->eta(), tmpTrk->phi(), 0);
	pseudoTrackJet += tmpTrack;
	tmp_trkSum_SPt += tmpTrk->pt();
	tmp_trkSum_nTrk++;
      }
      else {
	//ATH_MSG_INFO(" .... track failed");
      }
    }
    tmp_trkSum_VPt = pseudoTrackJet.Pt();
    tmp_trkSum_VEta = pseudoTrackJet.Eta();
    tmp_trkSum_VPhi = pseudoTrackJet.Phi();
    v_jet_sumtrkS_pt->push_back(tmp_trkSum_SPt);
    v_jet_sumtrkV_pt->push_back(tmp_trkSum_VPt);
    v_jet_sumtrkV_eta->push_back(tmp_trkSum_VEta);
    v_jet_sumtrkV_phi->push_back(tmp_trkSum_VPhi);
    v_jet_sumtrk_ntrk->push_back(tmp_trkSum_nTrk);

    //std::cout << "  after track loop " << std::endl;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // SV0 // VD: check the existence of the vertex and only then fill the variables
    // this mimics what's done in MV2
    int sv0ntrkj = -1;
    int sv0ntrkv = -1;
    int sv0n2t = -1;
    float sv0m = -99;
    float sv0efc = -99;
    float sv0ndist = -99;
    const std::vector<ElementLink<xAOD::VertexContainer > > SV0vertices = bjet->auxdata<std::vector<ElementLink<xAOD::VertexContainer > > >("SV0_vertices");
    if (SV0vertices.size() != 0) {
      bjet->taggerInfo(sv0ntrkj, xAOD::SV0_NGTinJet);
      bjet->taggerInfo(sv0ntrkv, xAOD::SV0_NGTinSvx);
      bjet->taggerInfo(sv0n2t, xAOD::SV0_N2Tpair);
      bjet->taggerInfo(sv0m, xAOD::SV0_masssvx);
      bjet->taggerInfo(sv0efc, xAOD::SV0_efracsvx);
      bjet->taggerInfo(sv0ndist, xAOD::SV0_normdist);
      v_jet_sv0_sig3d->push_back(bjet->SV0_significance3D());
    }
    else {
      v_jet_sv0_sig3d->push_back(-99);
    }
    v_jet_sv0_ntrkj->push_back(sv0ntrkj);
    v_jet_sv0_ntrkv->push_back(sv0ntrkv);
    v_jet_sv0_n2t->push_back(sv0n2t);
    v_jet_sv0_m->push_back(sv0m);
    v_jet_sv0_efc->push_back(sv0efc);
    v_jet_sv0_normdist->push_back(sv0ndist);

    // SV1 // VD: check the existence of the vertex and only then fill the variables
    // this mimics what's done in MV2
    int sv1ntrkj = -1;
    int sv1ntrkv = -1;
    int sv1n2t = -1;
    float sv1m = -99;
    float sv1efc = -99;
    float sv1ndist = -99;
    float sig3d = -99;
    const std::vector<ElementLink<xAOD::VertexContainer > > SV1vertices = bjet->auxdata<std::vector<ElementLink<xAOD::VertexContainer > > >("SV1_vertices");
    if (SV1vertices.size() != 0) {
      bjet->taggerInfo(sv1ntrkj, xAOD::SV1_NGTinJet);
      bjet->taggerInfo(sv1ntrkv, xAOD::SV1_NGTinSvx);
      bjet->taggerInfo(sv1n2t, xAOD::SV1_N2Tpair);
      bjet->taggerInfo(sv1m, xAOD::SV1_masssvx);
      bjet->taggerInfo(sv1efc, xAOD::SV1_efracsvx);
      bjet->taggerInfo(sv1ndist, xAOD::SV1_normdist);
      try {
	bjet->variable<float>("SV1", "significance3d" , sig3d);
      } catch(...) {}
      v_jet_sv1_pb->push_back(bjet->SV1_pb());
      v_jet_sv1_pc->push_back(bjet->SV1_pc());
      v_jet_sv1_pu->push_back(bjet->SV1_pu());
      v_jet_sv1_llr->push_back(bjet->SV1_loglikelihoodratio());
    }
    else {
      v_jet_sv1_pb->push_back(-99);
      v_jet_sv1_pc->push_back(-99);
      v_jet_sv1_pu->push_back(-99);
      v_jet_sv1_llr->push_back(-99);
    }
    v_jet_sv1_ntrkj->push_back(sv1ntrkj);
    v_jet_sv1_ntrkv->push_back(sv1ntrkv);
    v_jet_sv1_n2t->push_back(sv1n2t);
    v_jet_sv1_m->push_back(sv1m);
    v_jet_sv1_efc->push_back(sv1efc);
    v_jet_sv1_normdist->push_back(sv1ndist);
    v_jet_sv1_sig3d->push_back(sig3d);

    // JetFitter // VD: check the existence of the vertex and then fill the variables
    // this mimics what's done in MV2
    float jfm = -99;
    float jfefc = -99;
    float jfdeta = -99;
    float jfdphi = -99;
    int jfntrkAtVx = -1;
    int jfnvtx = -1;
    float jfsig3d = -99;
    int jfnvtx1t = -1;
    int jfn2t = -1;
    float massUncorr = -99;  // eloi
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
      v_jet_jf_pb->push_back(bjet->JetFitter_pb());
      v_jet_jf_pc->push_back(bjet->JetFitter_pc());
      v_jet_jf_pu->push_back(bjet->JetFitter_pu());
      v_jet_jf_llr->push_back(bjet->JetFitter_loglikelihoodratio());
      bjet->variable<float>("JetFitter", "massUncorr", massUncorr); //eloi
      bjet->variable<float>("JetFitter", "dRFlightDir", dRFlightDir); //eloi
    }
    else {
      v_jet_jf_pb->push_back(-99);
      v_jet_jf_pc->push_back(-99);
      v_jet_jf_pu->push_back(-99);
      v_jet_jf_llr->push_back(-99);
    }
    v_jet_jf_VTXsize->push_back(jfvertices.size());

    v_jet_jf_m->push_back(jfm);
    v_jet_jf_mUncorr->push_back(massUncorr); //eloi
    v_jet_jf_efc->push_back(jfefc);
    v_jet_jf_deta->push_back(jfdeta);
    v_jet_jf_dphi->push_back(jfdphi);
    v_jet_jf_dRFlightDir->push_back(dRFlightDir); //eloi
    v_jet_jf_ntrkAtVx->push_back(jfntrkAtVx);
    v_jet_jf_nvtx->push_back(jfnvtx);
    v_jet_jf_sig3d->push_back(jfsig3d);
    v_jet_jf_nvtx1t->push_back(jfnvtx1t);
    v_jet_jf_n2t->push_back(jfn2t);

    // JetFitterCombNN
    v_jet_jfcombnn_pb->push_back(bjet->JetFitterCombNN_pb());
    v_jet_jfcombnn_pc->push_back(bjet->JetFitterCombNN_pc());
    v_jet_jfcombnn_pu->push_back(bjet->JetFitterCombNN_pu());
    v_jet_jfcombnn_llr->push_back(bjet->JetFitterCombNN_loglikelihoodratio());

    // DL1
    try {
      v_jet_dl1_pb->push_back(bjet->auxdata<double>("DL1_pb"));
      v_jet_dl1_pc->push_back(bjet->auxdata<double>("DL1_pc"));
      v_jet_dl1_pu->push_back(bjet->auxdata<double>("DL1_pu"));
    } catch(...) {}

    // Other
    v_jet_sv1ip3d->push_back(bjet->SV1plusIP3D_discriminant());
    try{ 
      v_jet_mv1    ->push_back(bjet->MV1_discriminant());
      v_jet_mv1c   ->push_back(bjet->auxdata<double>("MV1c_discriminant"));
    } catch(...){ }

    try {
      v_jet_mv2c00->push_back(bjet->auxdata<double>("MV2c00_discriminant"));
      v_jet_mv2c10->push_back(bjet->auxdata<double>("MV2c10_discriminant"));
      v_jet_mv2c20->push_back(bjet->auxdata<double>("MV2c20_discriminant"));
      v_jet_mv2c100->push_back(bjet->auxdata<double>("MV2c100_discriminant"));
      v_jet_mv2cl100->push_back(bjet->auxdata<double>("MV2cl100_discriminant"));
    } catch(...) { }

    try {
      v_jet_mv2m_pu->push_back(bjet->auxdata<double>("MV2m_pu"));
      v_jet_mv2m_pc->push_back(bjet->auxdata<double>("MV2m_pc"));
      v_jet_mv2m_pb->push_back(bjet->auxdata<double>("MV2m_pb"));
    } catch(...) { }

    try {
      v_jet_mvb    ->push_back(bjet->auxdata<double>("MV2c10b_discriminant"));
    } catch(...) { }


    if (m_doMSV){
      // MSV
      // need initial values if no msv vertex is find, fix in MSVvariablesFactory
      double msv1;
      double msv2;
      int msv_n2t;
      float msv_ergtkjet;
      int msv_nvsec;
      float msv_nrdist;

      bjet->variable<double>("MultiSVbb1", "discriminant", msv1);
      v_jet_multisvbb1->push_back(msv1);

      bjet->variable<double>("MultiSVbb2", "discriminant", msv2);
      v_jet_multisvbb2->push_back(msv2);

      bjet->variable<int>("MSV", "N2Tpair", msv_n2t);
      v_jet_msv_N2Tpair->push_back(msv_n2t);

      bjet->variable<float>("MSV", "energyTrkInJet", msv_ergtkjet);
      v_jet_msv_energyTrkInJet->push_back(msv_ergtkjet);

      bjet->variable<int>("MSV", "nvsec", msv_nvsec);
      v_jet_msv_nvsec->push_back(msv_nvsec);

      bjet->variable<float>("MSV", "normdist", msv_nrdist);
      v_jet_msv_normdist->push_back(msv_nrdist);

      std::vector< ElementLink< xAOD::VertexContainer > > msvVertices;
      bjet->variable<std::vector<ElementLink<xAOD::VertexContainer> > >("MSV", "vertices", msvVertices);

      //tmp vectors
      std::vector<float> j_msv_cov0 = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_cov1 = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_cov2 = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_cov3 = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_cov4 = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_cov5 = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_mass = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_efrc = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_ntrk = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_pt = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_eta = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_phi = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_dls = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_xp = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_yp = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_zp = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_chi = std::vector<float>(msv_nvsec, 0);
      std::vector<float> j_msv_ndf = std::vector<float>(msv_nvsec, 0);

      // loop over vertices
      int ivtx = 0;
      if (msvVertices.size() > 0) {
        const std::vector<ElementLink<xAOD::VertexContainer> >::const_iterator verticesEnd = msvVertices.end();
        for (std::vector<ElementLink<xAOD::VertexContainer> >::const_iterator vtxIter = msvVertices.begin(); vtxIter != verticesEnd; ++vtxIter){
          if (msvVertices.size() >= 10) continue;
          float mass = xAOD::SecVtxHelper::VertexMass(**vtxIter);
          float efrc = xAOD::SecVtxHelper::EnergyFraction(**vtxIter);
          int ntrk = xAOD::SecVtxHelper::VtxNtrk(**vtxIter);
          float pt = xAOD::SecVtxHelper::Vtxpt(**vtxIter);
          float eta = xAOD::SecVtxHelper::Vtxeta(**vtxIter);
          float phi = xAOD::SecVtxHelper::Vtxphi(**vtxIter);
          float dls = xAOD::SecVtxHelper::VtxnormDist(**vtxIter);
          float xp = (**vtxIter)->x();
          float yp = (**vtxIter)->y();
          float zp = (**vtxIter)->z();
          float chi = (**vtxIter)->chiSquared();
          float ndf = (**vtxIter)->numberDoF();
          std::vector<float> covariantMatrix = (**vtxIter)->covariance();

          if (ivtx < 10) {
            j_msv_cov0[ivtx] = covariantMatrix.at(0);
            j_msv_cov1[ivtx] = covariantMatrix.at(1);
            j_msv_cov2[ivtx] = covariantMatrix.at(2);
            j_msv_cov3[ivtx] = covariantMatrix.at(3);
            j_msv_cov4[ivtx] = covariantMatrix.at(4);
            j_msv_cov5[ivtx] = covariantMatrix.at(5);
            j_msv_mass[ivtx] = mass;
            j_msv_efrc[ivtx] = efrc;
            j_msv_ntrk[ivtx] = ntrk;
            j_msv_pt[ivtx] = pt;
            j_msv_eta[ivtx] = eta;
            j_msv_phi[ivtx] = phi;
            j_msv_dls[ivtx] = dls;
            j_msv_xp[ivtx] = xp;
            j_msv_yp[ivtx] = yp;
            j_msv_zp[ivtx] = zp;
            j_msv_chi[ivtx] = chi;
            j_msv_ndf[ivtx] = ndf;
            ivtx++;
          }
        }
      }

      // fill info per vertex
      v_jet_msv_vtx_cov0->push_back(j_msv_cov0);
      v_jet_msv_vtx_cov1->push_back(j_msv_cov1);
      v_jet_msv_vtx_cov2->push_back(j_msv_cov2);
      v_jet_msv_vtx_cov3->push_back(j_msv_cov3);
      v_jet_msv_vtx_cov4->push_back(j_msv_cov4);
      v_jet_msv_vtx_cov5->push_back(j_msv_cov5);
      v_jet_msv_vtx_mass->push_back(j_msv_mass);
      v_jet_msv_vtx_efrc->push_back(j_msv_efrc);
      v_jet_msv_vtx_ntrk->push_back(j_msv_ntrk);
      v_jet_msv_vtx_pt->push_back(j_msv_pt);
      v_jet_msv_vtx_eta->push_back(j_msv_eta);
      v_jet_msv_vtx_phi->push_back(j_msv_phi);
      v_jet_msv_vtx_dls->push_back(j_msv_dls);
      v_jet_msv_vtx_x->push_back(j_msv_xp);
      v_jet_msv_vtx_y->push_back(j_msv_yp);
      v_jet_msv_vtx_z->push_back(j_msv_eta);
      v_jet_msv_vtx_chi->push_back(j_msv_chi);
      v_jet_msv_vtx_ndf->push_back(j_msv_ndf);

    } // end m_doMSV

    // Generating MVb variables (as in MV2Tag.cxx) 
    bool trksOK=IP3DTracks.size();

    std::vector<float> vectD0, vectD0Signi, vectZ0, vectZ0Signi;    vectD0.clear(), vectD0Signi.clear(), vectZ0.clear(), vectZ0Signi.clear();
    trksOK &= bjet->variable< std::vector<float> > ("IP3D", "valD0wrtPVofTracks", vectD0     );
    trksOK &= bjet->variable< std::vector<float> > ("IP3D", "sigD0wrtPVofTracks", vectD0Signi);
    trksOK &= bjet->variable< std::vector<float> > ("IP3D", "valZ0wrtPVofTracks", vectZ0     );
    trksOK &= bjet->variable< std::vector<float> > ("IP3D", "sigZ0wrtPVofTracks", vectZ0Signi);
    if (vectD0.size() and vectD0Signi.size() and vectZ0.size() and vectZ0Signi.size()) {
      trksOK &= IP3DTracks.size() == vectD0.size();
      trksOK &= IP3DTracks.size() == vectZ0.size();
      trksOK &= IP3DTracks.size() == vectD0Signi.size();
      trksOK &= IP3DTracks.size() == vectZ0Signi.size();
    }
    //std::cout<<"debug: "<<IP3DTracks.size()<<" "<<vectD0.size()<<" "<<vectZ0.size()<<" "<<vectD0Signi.size()<<" "<<vectZ0Signi.size()<<std::endl;

    int ntrks = IP3DTracks.size();
    float width = 0;
    int   n_trk_d0cut = 0;
    float trk3_d0sig = -100;
    float trk3_z0sig = -100;
    float sv_scaled_efc = -1;
    float jf_scaled_efc = -1;
    if (trksOK) {
      ATH_MSG_VERBOSE("#BTAG# MV2: calculating MVb inputs.");

      float sum_pt = 0., sum_pt_dr = 0.;

      std::vector<std::pair<float, float> > trk_d0_z0;
      trk_d0_z0.reserve(IP3DTracks.size());

      unsigned trkIndex=0;
      for(auto trkIter = IP3DTracks.begin(); trkIter != IP3DTracks.end(); ++trkIter) {
        const xAOD::TrackParticle* aTemp = **trkIter;
        TLorentzVector trk;
        trk.SetPtEtaPhiM(aTemp->pt(), aTemp->eta(), aTemp->phi(), 0.);

        // no need for a dedicated selection here, the tracks are already
        // selected by the IP3D algorithm
        const float d0sig = vectD0Signi.at(trkIndex);
        const float z0sig = vectZ0Signi.at(trkIndex);
        trkIndex++;

        if (std::fabs(d0sig) > 1.8)
          n_trk_d0cut++;

        // track width components
        sum_pt += trk.Pt();
        const float dRtoJet = trk.DeltaR(jet->p4());
        sum_pt_dr += dRtoJet * trk.Pt();

        // for 3rd higest d0/z0 significance
        trk_d0_z0.push_back(std::make_pair(d0sig, z0sig));
      } //end of trk loop
    
      // sort by highest signed d0 sig
      std::sort(trk_d0_z0.begin(), trk_d0_z0.end(), [](const std::pair<float, float>& a, const std::pair<float, float>& b) { 
        return a.first > b.first; 
      } );

      //Assign MVb variables
      if (sum_pt > 0) width = sum_pt_dr / sum_pt;
      if (trk_d0_z0.size() > 2) trk3_d0sig = trk_d0_z0[2].first;
      if (trk_d0_z0.size() > 2) trk3_z0sig = trk_d0_z0[2].second;
      if (sv1ntrkv>0) sv_scaled_efc  =  sv1efc * (static_cast<float>(ntrks) / sv1ntrkv);
      if (jfntrkAtVx + jfnvtx1t>0) jf_scaled_efc  =  jfefc * (static_cast<float>(ntrks) / (jfntrkAtVx + jfnvtx1t));
    }

    v_jet_width->push_back(width);
    v_jet_n_trk_sigd0cut->push_back(n_trk_d0cut);
    v_jet_trk3_d0sig->push_back(trk3_d0sig);
    v_jet_trk3_z0sig->push_back(trk3_z0sig);
    v_jet_sv_scaled_efc->push_back(sv_scaled_efc);
    v_jet_jf_scaled_efc->push_back(jf_scaled_efc);

    // ExKtbbTag
    if (bjet->isAvailable<double>("ExKtbb_Hbb_DoubleMV2c20")) {
      std::vector<const xAOD::Jet*> exKtJets;
      jet->getAssociatedObjects<xAOD::Jet>("ExKt2SubJets", exKtJets);
      if (exKtJets.size() == 2) {
        m_exkt_branches.fill(exKtJets);
      }

      v_jet_ExKtbb_Hbb_DoubleMV2c20->push_back(bjet->auxdata<double>("ExKtbb_Hbb_DoubleMV2c20"));

      if (bjet->isAvailable<double>("ExKtbb_Hbb_SingleMV2c20")) {
        v_jet_ExKtbb_Hbb_SingleMV2c20->push_back(bjet->auxdata<double>("ExKtbb_Hbb_SingleMV2c20"));
      }
      if (bjet->isAvailable<double>("ExKtbb_Hbb_MV2Only")) {
        v_jet_ExKtbb_Hbb_MV2Only->push_back(bjet->auxdata<double>("ExKtbb_Hbb_MV2Only"));
      }
      if (bjet->isAvailable<double>("ExKtbb_Hbb_MV2andJFDRSig")) {
        v_jet_ExKtbb_Hbb_MV2andJFDRSig->push_back(bjet->auxdata<double>("ExKtbb_Hbb_MV2andJFDRSig"));
      }
      if (bjet->isAvailable<double>("ExKtbb_Hbb_MV2andTopos")) {
        v_jet_ExKtbb_Hbb_MV2andTopos->push_back(bjet->auxdata<double>("ExKtbb_Hbb_MV2andTopos"));
      }
    }
    else {
      ATH_MSG_DEBUG("WARNING! No ExKtbbTag run on " << m_jetCollectionName.c_str());
    }

    // additions by nikola for ghost associated track jets
    if (strcmp(m_jetCollectionName.c_str(), "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets") == 0 || strcmp(m_jetCollectionName.c_str(), "Akt10LCTopoTrmJets") == 0) {
      // ATH_MSG_INFO("this is a trimmed large-R jet collection, adding information (pt and mv2c00) of track jets associated to parent untrimmed jet collection");

      std::vector<const xAOD::Jet*> ghostTrackJet2;

//      jet_parent->getAssociatedObjects<xAOD::Jet>("GhostAntiKt2TrackJet", ghostTrackJet2);
      jet->getAssociatedObjects<xAOD::Jet>("GhostAntiKt2TrackJet", ghostTrackJet2);
      if (ghostTrackJet2.size() >= 2) {
        m_unclustered_vertices.fill(ghostTrackJet2);
      }

      std::vector<const xAOD::Jet*> ghostVRTrackJet;
      jet->getAssociatedObjects<xAOD::Jet>("GhostVR50Rmax4Rmin0TrackJet", ghostVRTrackJet);

      m_vrtrkjet_branches.fill(ghostVRTrackJet);
    }

    // now the tracking part: prepare all the tmpVectors
    int j_btag_ntrk = 0;
    int j_sv1_ntrk = 0;
    int j_ip3d_ntrk = 0;
    int j_jf_ntrk = 0;

    std::vector<float> j_trk_pt;
    std::vector<float> j_trk_eta;
    std::vector<float> j_trk_theta;
    std::vector<float> j_trk_phi;
    std::vector<float> j_trk_qoverp;
    std::vector<float> j_trk_dr; // mod nikola
    std::vector<int> j_trk_assoc_msv; // mod nikola
    std::vector<float> j_trk_chi2;
    std::vector<float> j_trk_ndf;
    std::vector<int> j_trk_algo;
    std::vector<int> j_trk_orig;
    std::vector<int> j_trk_nInnHits;
    std::vector<int> j_trk_nNextToInnHits;
    std::vector<int> j_trk_nBLHits;
    std::vector<int> j_trk_nsharedBLHits;
    std::vector<int> j_trk_nsplitBLHits;
    std::vector<int> j_trk_nPixHits;
    std::vector<int> j_trk_nsharedPixHits;
    std::vector<int> j_trk_nsplitPixHits;
    std::vector<int> j_trk_nSCTHits;
    std::vector<int> j_trk_nsharedSCTHits;
    std::vector<int> j_trk_expectBLayerHit;
    std::vector<float> j_trk_vtx_X;
    std::vector<float> j_trk_vtx_Y;
    std::vector<float> j_trk_vtx_dx;
    std::vector<float>  j_trk_vtx_dy;
    std::vector<float> j_trk_d0;
    std::vector<float> j_trk_z0;
    std::vector<float> j_trk_d0_truth;
    std::vector<float> j_trk_z0_truth;
    std::vector<int> j_trk_ip3d_grade;
    std::vector<float> j_trk_ip3d_d0;
    std::vector<float> j_trk_ip3d_d02D;
    std::vector<float> j_trk_ip3d_z0;
    std::vector<float> j_trk_ip3d_d0sig;
    std::vector<float> j_trk_ip3d_z0sig;
    std::vector<float> j_trk_ip2d_llr;
    std::vector<float> j_trk_ip3d_llr;

    std::vector<float> j_trk_ip_d0;
    std::vector<float> j_trk_ip_z0;

    std::vector<float> j_sv0_vtxx;
    std::vector<float> j_sv0_vtxy;
    std::vector<float> j_sv0_vtxz;
    std::vector<float> j_sv1_vtxx;
    std::vector<float> j_sv1_vtxy;
    std::vector<float> j_sv1_vtxz;
    std::vector<float> j_jf_vtx_chi2; // mod Remco
    std::vector<float> j_jf_vtx_ndf; // mod Remco
    std::vector<int> j_jf_vtx_ntrk; // mod Remco
    std::vector<float> j_jf_vtx_L3d; // mod Remco
    std::vector<float> j_jf_vtx_sig3d; // mod Remco
    std::vector<int> j_jf_vtx_nvtx; // mod Remco
    std::vector<int> j_trk_jf_Vertex; // mod Remco
    // if (m_reduceInfo) continue;

    bool is8TeV = true;
    if (bjet->isAvailable<std::vector<ElementLink<xAOD::BTagVertexContainer> > >("JetFitter_JFvertices")) is8TeV = false;

    // std::vector< ElementLink< xAOD::TrackParticleContainer > > assocTracks = bjet->auxdata<std::vector<ElementLink<xAOD::TrackParticleContainer> > >("BTagTrackToJetAssociator");
    std::vector< ElementLink< xAOD::TrackParticleContainer > > SV0Tracks ;
    std::vector< ElementLink< xAOD::TrackParticleContainer > > SV1Tracks ;
    std::vector< ElementLink< xAOD::TrackParticleContainer > > JFTracks;

    if (!is8TeV) {
      // IP2DTracks = bjet->auxdata<std::vector<ElementLink< xAOD::TrackParticleContainer> > >("IP2D_TrackParticleLinks");
      // IP3DTracks = bjet->auxdata<std::vector<ElementLink< xAOD::TrackParticleContainer> > >("IP3D_TrackParticleLinks");
      SV0Tracks = bjet->SV0_TrackParticleLinks();
      SV1Tracks = bjet->SV1_TrackParticleLinks();
    }  // bjet->IP3D_TrackParticleLinks();

    v_jet_sv0_Nvtx->push_back(SV0vertices.size());

    float jet_mu_dRmin_pt=999,jet_mu_dRmin_dR=999,jet_mu_dRmin_truthflav=999,jet_mu_dRmin_eta=999,jet_mu_dRmin_phi=999,jet_mu_dRmin_assJet_pt=999,jet_mu_dRmin_qOverPratio=999,jet_mu_dRmin_mombalsignif=999,jet_mu_dRmin_scatneighsignif=999,jet_mu_dRmin_pTrel=999,jet_mu_dRmin_VtxTyp=999,jet_mu_dRmin_d0=999,jet_mu_dRmin_z0=999,jet_mu_dRmin_parent_pdgid=999,jet_mu_dRmin_ID_qOverP_var=999,jet_mu_dRmin_muonType=999;    
    float jet_mu_fatjet_nMu = 0, jet_mu_fatjet_pTmax_pT = 999, jet_mu_fatjet_pTmax_pTrel = 999, jet_mu_fatjet_pTmax_pTrelFrac = 999;
    if (m_SMT) {
      // additions by nikola
      try {
	std::vector<ElementLink<xAOD::MuonContainer> > assocMuons;
        assocMuons = bjet->auxdata<std::vector<ElementLink<xAOD::MuonContainer> > >("Muons");
        if (assocMuons.size() != 0) {
          for (unsigned int iT = 0; iT < assocMuons.size(); iT++) {
            if (!assocMuons.at(iT).isValid()) continue;
            const xAOD::Muon *tmpMuon = *(assocMuons.at(iT));
            float dr = deltaR(tmpMuon->eta(), jet->eta(), tmpMuon->phi(), jet->phi());
            const ElementLink< xAOD::TrackParticleContainer >& pMuIDTrack = tmpMuon->inDetTrackParticleLink();
            const ElementLink< xAOD::TrackParticleContainer >& pMuMSTrack = tmpMuon->muonSpectrometerTrackParticleLink();
            const xAOD::Vertex *pVtx = (*pMuIDTrack)->vertex();
            const std::vector<float>&cov = (*pMuIDTrack)->definingParametersCovMatrixVec();
            float momBalSignif0 = 999.;
            tmpMuon->parameter(momBalSignif0, xAOD::Muon::momentumBalanceSignificance);
            if (momBalSignif0 == 0) continue;
            if ((*pMuMSTrack)->qOverP() == 0) continue;
            if (dr >= 1.0) continue;
            jet_mu_fatjet_nMu += 1;
            if (tmpMuon->pt() > jet_mu_fatjet_pTmax_pT) {
              jet_mu_fatjet_pTmax_pT = tmpMuon->pt();
              TLorentzVector myjet, mymu;
              myjet.SetPtEtaPhiM(jet->pt(), jet->eta(), jet->phi(), 0);
              mymu.SetPtEtaPhiM(tmpMuon->pt(), tmpMuon->eta(), tmpMuon->phi(), 0);
              jet_mu_fatjet_pTmax_pTrel = myjet.Vect().Perp(mymu.Vect()) / 1000;
              jet_mu_fatjet_pTmax_pTrelFrac = jet_mu_fatjet_pTmax_pTrel / jet->pt();
            }
          }
        }
      } catch(...) {
        ATH_MSG_INFO("NO Muons found!");
      }

      /*
      std::cout << std::endl;
      for( SG::auxid_t auxid : bjet->getAuxIDs() ) {
	static SG::AuxTypeRegistry& reg = SG::AuxTypeRegistry::instance();
	std::cout << reg.getName( auxid ) << " , " << reg.getTypeName( auxid ) << std::endl;
      }
      */

      // new from Valerio: if the variables are already available, do not calculate them
      if ( bjet->isAvailable<float>("SMT_mu_pt") ) {
	//std::cout << "SMT info already available, will get them from there ... " << std::endl;
	jet_mu_dRmin_dR             = bjet->auxdata<float>("SMT_dR");
	jet_mu_dRmin_pt             = bjet->auxdata<float>("SMT_mu_pt");
	jet_mu_dRmin_qOverPratio    = bjet->auxdata<float>("SMT_qOverPratio");
	jet_mu_dRmin_mombalsignif   = bjet->auxdata<float>("SMT_mombalsignif");
	jet_mu_dRmin_scatneighsignif= bjet->auxdata<float>("SMT_scatneighsignif");
	jet_mu_dRmin_pTrel          = bjet->auxdata<float>("SMT_pTrel");
	jet_mu_dRmin_d0             = bjet->auxdata<float>("SMT_mu_d0");
	jet_mu_dRmin_z0             = bjet->auxdata<float>("SMT_mu_z0");
	jet_mu_dRmin_ID_qOverP_var  = bjet->auxdata<float>("SMT_ID_qOverP");
	jet_mu_dRmin_assJet_pt=jet->pt()/1000; // ?? why is this variable needed?
	jet_mu_dRmin_truthflav=thisJetTruthLabel;

	ElementLink<xAOD::MuonContainer> tmpMuonLink= bjet->auxdata<ElementLink<xAOD::MuonContainer> >("SMT_mu_link");
	if ( tmpMuonLink.isValid() ) {
	  const xAOD::Muon* tmpMuon=(*tmpMuonLink);
	  //std::cout << " link is: " << tmpMuon << std::endl;
	  if ( tmpMuon!=0 ) {
	    jet_mu_dRmin_eta      =tmpMuon->eta();
	    jet_mu_dRmin_phi      =tmpMuon->phi();
	    jet_mu_dRmin_muonType =tmpMuon->muonType();
	    //std::cout << " .... after eta and friends" << std::endl;
	    const ElementLink< xAOD::TrackParticleContainer >& pMuIDTrack=tmpMuon->inDetTrackParticleLink();
	    //std::cout << "   the link is: " << pMuIDTrack << std::endl;
	    const xAOD::Vertex * pVtx=(*pMuIDTrack)->vertex();
	    if(pVtx!=NULL) {
	      jet_mu_dRmin_VtxTyp=pVtx->vertexType();
	    } else {jet_mu_dRmin_VtxTyp=999.;}
	    const xAOD::TruthParticle* matched_truth_muon=0;
	    if(tmpMuon->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
	      ElementLink<xAOD::TruthParticleContainer> link = tmpMuon->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
	      if(link.isValid()) {
		matched_truth_muon = *link;
		int pdgid = parent_classify(matched_truth_muon);
		jet_mu_dRmin_parent_pdgid=pdgid;
	      } else {jet_mu_dRmin_parent_pdgid=999.;}
	    }
	  }
	}

      } else {
	try{
	  std::vector<ElementLink<xAOD::MuonContainer> > assocMuons;
	  assocMuons= bjet->auxdata<std::vector<ElementLink<xAOD::MuonContainer> > >("Muons");
	  if(assocMuons.size()!=0){
	    for (unsigned int iT=0; iT<assocMuons.size(); iT++) {
	      if (!assocMuons.at(iT).isValid()) continue;
	      const xAOD::Muon* tmpMuon= *(assocMuons.at(iT));
	      float dr = deltaR(tmpMuon->eta(),jet->eta(),tmpMuon->phi(),jet->phi());
	      if(dr>=0.4) continue;
	      const ElementLink< xAOD::TrackParticleContainer >& pMuIDTrack=tmpMuon->inDetTrackParticleLink();
	      const ElementLink< xAOD::TrackParticleContainer >& pMuMSTrack=tmpMuon->muonSpectrometerTrackParticleLink();
	      const xAOD::Vertex * pVtx=(*pMuIDTrack)->vertex();
	      const std::vector<float>&cov= (*pMuIDTrack)->definingParametersCovMatrixVec();
	      float momBalSignif0=999.;
	      tmpMuon->parameter(momBalSignif0, xAOD::Muon::momentumBalanceSignificance);
	      if(momBalSignif0==0) continue;
	      if((*pMuMSTrack)->qOverP()==0) continue;
	      if(dr<jet_mu_dRmin_dR){
		jet_mu_dRmin_dR=dr;
		jet_mu_dRmin_pt=tmpMuon->pt()/1000;
		jet_mu_dRmin_truthflav=thisJetTruthLabel;
		jet_mu_dRmin_eta=tmpMuon->eta();
		jet_mu_dRmin_phi=tmpMuon->phi();
		jet_mu_dRmin_assJet_pt=jet->pt()/1000;
		jet_mu_dRmin_qOverPratio=(*pMuIDTrack)->qOverP()/(*pMuMSTrack)->qOverP();
		float momBalSignif=999.;
		if(tmpMuon->parameter(momBalSignif, xAOD::Muon::momentumBalanceSignificance)) {
		  jet_mu_dRmin_mombalsignif=momBalSignif;
		} else {jet_mu_dRmin_mombalsignif=momBalSignif;}
		float scatNeighSignif=999.;
		if(tmpMuon->parameter(scatNeighSignif, xAOD::Muon::scatteringNeighbourSignificance)) {
		  jet_mu_dRmin_scatneighsignif=scatNeighSignif;
		} else {jet_mu_dRmin_scatneighsignif=scatNeighSignif;}
		TLorentzVector myjet, mymu;
		myjet.SetPtEtaPhiM(jet->pt(),jet->eta(),jet->phi(),0);
		mymu.SetPtEtaPhiM(tmpMuon->pt(),tmpMuon->eta(),tmpMuon->phi(),0);
		jet_mu_dRmin_pTrel=myjet.Vect().Perp(mymu.Vect())/1000;
		if(pVtx!=NULL) {
		  jet_mu_dRmin_VtxTyp=pVtx->vertexType();
		} else {jet_mu_dRmin_VtxTyp=999.;}
		jet_mu_dRmin_d0=tmpMuon->primaryTrackParticle()->d0();
		jet_mu_dRmin_z0=tmpMuon->primaryTrackParticle()->z0();

		const xAOD::TruthParticle* matched_truth_muon=0;
		if(tmpMuon->isAvailable<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink")) {
		  ElementLink<xAOD::TruthParticleContainer> link = tmpMuon->auxdata<ElementLink<xAOD::TruthParticleContainer> >("truthParticleLink");
		  if(link.isValid()) {
		    matched_truth_muon = *link;
		    int pdgid = parent_classify(matched_truth_muon);
		    jet_mu_dRmin_parent_pdgid=pdgid;
		  } else {jet_mu_dRmin_parent_pdgid=999.;}
		}
		jet_mu_dRmin_ID_qOverP_var=cov[14];
		jet_mu_dRmin_muonType=tmpMuon->muonType();
	      }
	    }
	  }
	} catch(...) {
	  //std::cout << "NO Muons found!"<<std::endl;
	  //todo: write out some warning here but don't want to clog logfiles for now
	}
      }
     }

    for (unsigned int sv0V = 0; sv0V < SV0vertices.size(); sv0V++) {
      if (!SV0vertices.at(sv0V).isValid()) {
        // std::cout << "INVALID sv0 vertex: " << std::endl;
        continue;
      }
      const xAOD::Vertex *tmpVertex = *(SV0vertices.at(sv0V));
      j_sv0_vtxx.push_back(tmpVertex->x());
      j_sv0_vtxy.push_back(tmpVertex->y());
      j_sv0_vtxz.push_back(tmpVertex->z());
    }
    v_jet_sv0_vtxx->push_back(j_sv0_vtxx);
    v_jet_sv0_vtxy->push_back(j_sv0_vtxy);
    v_jet_sv0_vtxz->push_back(j_sv0_vtxz);

    v_jet_sv1_Nvtx->push_back(SV1vertices.size());
    for (unsigned int sv1V = 0; sv1V < SV1vertices.size(); sv1V++) {
      if (!SV1vertices.at(sv1V).isValid()) continue;
      const xAOD::Vertex *tmpVertex = *(SV1vertices.at(sv1V));
      j_sv1_vtxx.push_back(tmpVertex->x());
      j_sv1_vtxy.push_back(tmpVertex->y());
      j_sv1_vtxz.push_back(tmpVertex->z());
    }
    v_jet_sv1_vtxx->push_back(j_sv1_vtxx);
    v_jet_sv1_vtxy->push_back(j_sv1_vtxy);
    v_jet_sv1_vtxz->push_back(j_sv1_vtxz);


    if (m_SMT) {
      v_jet_mu_assJet_pt->push_back(jet_mu_dRmin_assJet_pt);
      v_jet_mu_truthflav->push_back(jet_mu_dRmin_truthflav);
      v_jet_mu_pt->push_back(jet_mu_dRmin_pt);
      v_jet_mu_eta->push_back(jet_mu_dRmin_eta);
      v_jet_mu_phi->push_back(jet_mu_dRmin_phi);
      v_jet_mu_qOverPratio->push_back(jet_mu_dRmin_qOverPratio);
      v_jet_mu_mombalsignif->push_back(jet_mu_dRmin_mombalsignif);
      v_jet_mu_scatneighsignif->push_back(jet_mu_dRmin_scatneighsignif);
      v_jet_mu_dR->push_back(jet_mu_dRmin_dR);
      v_jet_mu_pTrel->push_back(jet_mu_dRmin_pTrel);
      v_jet_mu_VtxTyp->push_back(jet_mu_dRmin_VtxTyp);
      v_jet_mu_d0->push_back(jet_mu_dRmin_d0);
      v_jet_mu_z0->push_back(jet_mu_dRmin_z0);
      v_jet_mu_parent_pdgid->push_back(jet_mu_dRmin_parent_pdgid);
      v_jet_mu_ID_qOverP_var->push_back(jet_mu_dRmin_ID_qOverP_var);
      v_jet_mu_muonType->push_back(jet_mu_dRmin_muonType);
      v_jet_mu_fatjet_nMu->push_back(jet_mu_fatjet_nMu);
      v_jet_mu_fatjet_pTmax_pT->push_back(jet_mu_fatjet_pTmax_pT);
      v_jet_mu_fatjet_pTmax_pTrel->push_back(jet_mu_fatjet_pTmax_pTrel);
      v_jet_mu_fatjet_pTmax_pTrelFrac->push_back(jet_mu_fatjet_pTmax_pTrelFrac);
    }

    if (m_reduceInfo) continue;

    std::vector<float> fittedPosition = bjet->auxdata<std::vector<float> >("JetFitter_fittedPosition"); // mod Remco
    std::vector<float> fittedCov = bjet->auxdata<std::vector<float> >("JetFitter_fittedCov"); // mod Remco

    // if (PV_jf_x != fittedPosition[0]) {std::cout << "PV_jf_x is set for the first time (this may happen only once!)" << std::endl;} // mod Remco
    // if (PV_jf_y != fittedPosition[0]) {std::cout << "PV_jf_y is set for the first time (this may happen only once!)" << std::endl;} // mod Remco
    // if (PV_jf_z != fittedPosition[0]) {std::cout << "PV_jf_z is set for the first time (this may happen only once!)" << std::endl;} // mod Remco
    if (fittedPosition.size() > 0) {
      PV_jf_x = fittedPosition[0]; // mod Remco
      PV_jf_y = fittedPosition[1]; // mod Remco
      PV_jf_z = fittedPosition[2]; // mod Remco
      v_jet_jf_phi->push_back(fittedPosition[3]); // mod Remco
      v_jet_jf_theta->push_back(fittedPosition[4]); // mod Remco
    }
    else {
      v_jet_jf_phi->push_back(-999); // mod Remco
      v_jet_jf_theta->push_back(-999); // mod Remco
    }

    // std::cout << " VALERIO: " << jfvertices.size() << " , " << jfnvtx << " , " << jfnvtx1t << " ..... and: " << fittedPosition.size() << std::endl;
    for (unsigned int jfv = 0; jfv < jfvertices.size(); jfv++) {
      if (!jfvertices.at(jfv).isValid()) continue;
      const xAOD::BTagVertex *tmpVertex = *(jfvertices.at(jfv));
      const std::vector< ElementLink<xAOD::TrackParticleContainer> > tmpVect = tmpVertex->track_links(); // mod Remco
      JFTracks.insert(JFTracks.end(), tmpVect.begin(), tmpVect.end()); // mod Remco

      j_jf_vtx_chi2.push_back(tmpVertex->chi2()); // mod Remco
      j_jf_vtx_ndf.push_back(tmpVertex->NDF()); // mod Remco
      j_jf_vtx_ntrk.push_back(tmpVect.size()); // mod Remco
      if (jfv < 3) {
        j_jf_vtx_L3d.push_back(fittedPosition[jfv + 5]); // mod Remco
        j_jf_vtx_sig3d.push_back(sqrt(fittedCov[jfv + 5])); // mod Remco
      }
      else {
        j_jf_vtx_L3d.push_back(-999); // mod Remco
        j_jf_vtx_sig3d.push_back(-999); // mod Remco
      }
      // j_jf_vtx_nvtx.push_back(jfvertices.size()); // mod Remco
    }
    v_jet_jf_vtx_chi2->push_back(j_jf_vtx_chi2); // mod Remco
    v_jet_jf_vtx_ndf->push_back(j_jf_vtx_ndf); // mod Remco
    v_jet_jf_vtx_ntrk->push_back(j_jf_vtx_ntrk); // mod Remco
    v_jet_jf_vtx_L3d->push_back(j_jf_vtx_L3d); // mod Remco
    v_jet_jf_vtx_sig3d->push_back(j_jf_vtx_sig3d); // mod Remco
    // v_jet_jf_vtx_nvtx->push_back(j_jf_vtx_nvtx); // mod Remco

    j_btag_ntrk = 0; // assocTracks.size();
    j_sv1_ntrk = SV1Tracks.size();
    j_ip3d_ntrk = IP3DTracks.size();
    j_jf_ntrk = JFTracks.size();

    std::vector<int> tmpGrading = bjet->auxdata<std::vector<int> >("IP3D_gradeOfTracks");
    std::vector<float> tmpD0 = bjet->auxdata<std::vector<float> >("IP3D_valD0wrtPVofTracks");
    std::vector<float> tmpZ0 = bjet->auxdata<std::vector<float> >("IP3D_valZ0wrtPVofTracks");
    std::vector<float> tmpD0sig= bjet->auxdata<std::vector<float> >("IP3D_sigD0wrtPVofTracks");
    std::vector<float> tmpZ0sig= bjet->auxdata<std::vector<float> >("IP3D_sigZ0wrtPVofTracks");

    std::vector<float> tmpIP3DBwgt= bjet->auxdata<std::vector<float> >("IP3D_weightBofTracks");
    std::vector<float> tmpIP3DUwgt= bjet->auxdata<std::vector<float> >("IP3D_weightUofTracks");
    std::vector<float> tmpIP2DBwgt= bjet->auxdata<std::vector<float> >("IP2D_weightBofTracks");
    std::vector<float> tmpIP2DUwgt= bjet->auxdata<std::vector<float> >("IP2D_weightUofTracks");

    float ip2d_llr = -999;
    float ip3d_llr = -999;

    j_ip3d_ntrk = tmpGrading.size();

    // std::cout << "TOT tracks: " << assocTracks.size() << " IP3D: " << j_ip3d_ntrk << " ... grade: " << tmpGrading.size() << std::endl;

    if (m_reduceInfo) continue; // if using reduceInfo, don't output track information
    // if (is8TeV) continue;

    // jet direction:
    TLorentzVector jetDir;
    jetDir.SetPtEtaPhiE(v_jet_pt_orig->at(j), v_jet_eta_orig->at(j), v_jet_phi_orig->at(j), v_jet_E_orig->at(j));
    // Amg::Vector3D jetDirection(jet->px(),jet->py(),jet->pz());
    Amg::Vector3D jetDirection(jetDir.Px(),jetDir.Py(),jetDir.Pz());
    Amg::Vector3D unit = jetDirection.unit();

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    // Addition from Dan: first fill my track branches
    m_track_branches.fill(associated_tracks);
    // build track to vertex maps (from track_to_vertex_associators)
    auto msv_vtx_map = trkvx::get_msv_map(*bjet);
    // MAIN TRACK LOOP
    for (unsigned int iT = 0; iT < assocTracks.size(); iT++) {
      // std::cout << " .... trk link: " << iT << std::endl;
      if (!assocTracks.at(iT).isValid()) continue;
      const xAOD::TrackParticle *tmpTrk = *(assocTracks.at(iT));
      tmpTrk->summaryValue(getInt, xAOD::numberOfPixelHits);
      int nSi = getInt;
      tmpTrk->summaryValue(getInt, xAOD::numberOfSCTHits);
      nSi += getInt;
      if (nSi < 2) continue;

      j_btag_ntrk++;
      j_trk_pt.push_back(tmpTrk->pt());
      j_trk_eta.push_back(tmpTrk->eta());
      j_trk_theta.push_back(tmpTrk->theta());
      j_trk_phi.push_back(tmpTrk->phi());
      j_trk_qoverp.push_back(tmpTrk->qOverP());
      j_trk_chi2.push_back(tmpTrk->chiSquared());
      j_trk_ndf.push_back(tmpTrk->numberDoF());

      // addition by nikola
      TLorentzVector jet4vector, trk4vector;
      jet4vector.SetPtEtaPhiM(jet->pt(), jet->eta(), jet->phi(), jet->m());
      trk4vector.SetPtEtaPhiM(tmpTrk->pt(), tmpTrk->eta(), tmpTrk->phi(), 0);
      j_trk_dr.push_back(jet4vector.DeltaR(trk4vector));

      // if doing MSV, keep track of which MSV this track belongs to
      if (m_doMSV) {
        int trk_assoc_msv = msv_vtx_map.count(tmpTrk) ?
          msv_vtx_map.at(tmpTrk) : -1;
        j_trk_assoc_msv.push_back(trk_assoc_msv);
      }

      // algo
      unsigned int trackAlgo = 0;
      int index = -1;

      for (unsigned int iT = 0; iT < IP3DTracks.size(); iT++) {
        if (tmpTrk == *(IP3DTracks.at(iT))) {
          trackAlgo += 1 << IP3D;
          index = iT;
          break;
	}
      }

      if (index!=-1) {
	j_trk_ip3d_grade.push_back(tmpGrading.at(index));
	ip3d_llr=-999;
	ip2d_llr=-999;
	if (tmpIP3DUwgt.at(index)!=0) ip3d_llr = log(tmpIP3DBwgt.at(index)/tmpIP3DUwgt.at(index));
	if (tmpIP2DUwgt.at(index)!=0) ip2d_llr = log(tmpIP2DBwgt.at(index)/tmpIP2DUwgt.at(index));
	j_trk_ip3d_llr.push_back(ip3d_llr);
	j_trk_ip2d_llr.push_back(ip2d_llr);
      } else {
	j_trk_ip3d_grade.push_back(-10);
	j_trk_ip3d_llr.push_back(-999);
	j_trk_ip2d_llr.push_back(-999);
      }
      if (particleInCollection(tmpTrk, IP2DTracks)) trackAlgo += 1 << IP2D;
      if (particleInCollection(tmpTrk, SV0Tracks)) trackAlgo +=1 << SV0;
      if (particleInCollection(tmpTrk, SV1Tracks)) trackAlgo +=1 << SV1;
      if (particleInCollection(tmpTrk, JFTracks)) trackAlgo +=1 << JF; // mod Remco
      j_trk_algo.push_back(trackAlgo);

      int myVtx = -1; // mod Remco
      for (unsigned int jfv = 0; jfv < jfvertices.size(); jfv++) { // mod Remco
        if (!jfvertices.at(jfv).isValid()) continue;
        const xAOD::BTagVertex *tmpVertex = *(jfvertices.at(jfv)); // mod Remco
        const std::vector< ElementLink<xAOD::TrackParticleContainer> > tmpVect = tmpVertex->track_links(); // mod Remco
        if (particleInCollection(tmpTrk, tmpVect)) myVtx = jfv; // mod Remco
      } // mod Remco
      j_trk_jf_Vertex.push_back(myVtx); // mod Remco
      // std::cout << " ..... after Remco" << std::endl;

      // origin
      int origin = getTrackOrigin(tmpTrk,
                                  tracksFromB,
                                  tracksFromC,
                                  tracksFromCc,
                                  tracksFromB1,
                                  tracksFromB2,
                                  tracksFromC1,
                                  tracksFromC2,
                                  tracksFromCNotFromB1,
                                  tracksFromCNotFromB2);

      const xAOD::TruthParticle *truth = truthParticle(tmpTrk);

      if (truth) {
        if (truth->prodVtx()) {
          j_trk_vtx_X.push_back(truth->prodVtx()->x());
          j_trk_vtx_Y.push_back(truth->prodVtx()->y());
	}
        else {
          j_trk_vtx_X.push_back(-666);
          j_trk_vtx_Y.push_back(-666);
	}
      }
      else{
        j_trk_vtx_X.push_back(-999);
        j_trk_vtx_Y.push_back(-999);
      }
      j_trk_orig.push_back(origin);
      // std::cout << " ..... after origin" << std::endl;

      // hit content
      // Blayer
      tmpTrk->summaryValue(getInt, xAOD::numberOfInnermostPixelLayerHits);
      j_trk_nInnHits.push_back(getInt);
      tmpTrk->summaryValue(getInt, xAOD::numberOfNextToInnermostPixelLayerHits);
      j_trk_nNextToInnHits.push_back(getInt);

      tmpTrk->summaryValue(getInt, xAOD::numberOfBLayerHits);
      j_trk_nBLHits.push_back(getInt);
      getInt = 0;
      tmpTrk->summaryValue(getInt, xAOD::numberOfBLayerSharedHits);
      j_trk_nsharedBLHits.push_back(getInt);
      getInt = 0;
      tmpTrk->summaryValue(getInt, xAOD::numberOfBLayerSplitHits);
      j_trk_nsplitBLHits.push_back(getInt);
      getInt = 0;
      tmpTrk->summaryValue(getInt, xAOD::expectBLayerHit);
      j_trk_expectBLayerHit.push_back(getInt);
      getInt = 0;
      // Pixel
      tmpTrk->summaryValue(getInt, xAOD::numberOfPixelHits);
      j_trk_nPixHits.push_back(getInt);
      getInt = 0;
      tmpTrk->summaryValue(getInt, xAOD::numberOfPixelSharedHits);
      j_trk_nsharedPixHits.push_back(getInt);
      getInt = 0;
      tmpTrk->summaryValue(getInt, xAOD::numberOfPixelSplitHits);
      j_trk_nsplitPixHits.push_back(getInt);
      getInt = 0;
      // SCT
      tmpTrk->summaryValue(getInt, xAOD::numberOfSCTHits);
      j_trk_nSCTHits.push_back(getInt);
      getInt = 0;
      tmpTrk->summaryValue(getInt, xAOD::numberOfSCTSharedHits);
      j_trk_nsharedSCTHits.push_back(getInt);
      getInt = 0;

      // default spatial coordinates
      j_trk_d0.push_back(tmpTrk->d0());
      j_trk_z0.push_back(tmpTrk->z0() + tmpTrk->vz() - myVertex->z());

      // spatial coordinates: now from the tool:
      float d0wrtPriVtx = -999;
      float d0ErrwrtPriVtx = -999;
      float z0wrtPriVtx = -999;
      float z0ErrwrtPriVtx = -999;
      const Trk::ImpactParametersAndSigma *myIPandSigma(0);
      myIPandSigma = m_trackToVertexIPEstimator->estimate(tmpTrk, myVertex, false);
      if (0 == myIPandSigma) {
        ATH_MSG_WARNING("#NTUPDUMPER: trackToVertexIPEstimator failed !");
      }
      else {
        d0wrtPriVtx = myIPandSigma->IPd0;
        d0ErrwrtPriVtx = myIPandSigma->sigmad0;
        z0wrtPriVtx = myIPandSigma->IPz0SinTheta;
        z0ErrwrtPriVtx = myIPandSigma->sigmaz0SinTheta;
        delete myIPandSigma;
        myIPandSigma = 0;
      }

      // sign of the impact parameter
      double signOfIP = m_trackToVertexIPEstimator->get3DLifetimeSignOfTrack(tmpTrk->perigeeParameters(), unit, *myVertex);
      double signOfIP2D = m_trackToVertexIPEstimator->get2DLifetimeSignOfTrack(tmpTrk->perigeeParameters(), unit, *myVertex);
      double signOfZIP = m_trackToVertexIPEstimator->getZLifetimeSignOfTrack(tmpTrk->perigeeParameters(), unit, *myVertex);
      // significances
      double sIP = signOfIP * fabs(d0wrtPriVtx);
      double significance = signOfIP * fabs(d0wrtPriVtx / d0ErrwrtPriVtx);
      double szIP = signOfZIP * fabs(z0wrtPriVtx);
      double z0Sig = signOfZIP * fabs(z0wrtPriVtx / z0ErrwrtPriVtx);

      j_trk_ip3d_d0.push_back(sIP);
      j_trk_ip3d_d02D.push_back(signOfIP2D*fabs(d0wrtPriVtx));
      j_trk_ip3d_z0.push_back(szIP);
      j_trk_ip3d_d0sig.push_back( significance );
      j_trk_ip3d_z0sig.push_back(z0Sig);

      // unsigned parameters
      j_trk_ip_d0.push_back(d0wrtPriVtx);
      j_trk_ip_z0.push_back(z0wrtPriVtx);

      // TRUTH track info ......
      if (origin == PUFAKE) {
        j_trk_d0_truth.push_back(-999);
        j_trk_z0_truth.push_back(-999);
      }
      else {
        float tmpd0T = -999;
        float tmpz0T = -999;
        try {
          tmpd0T = truth->auxdata< float >("d0");
          tmpz0T = truth->auxdata< float >("z0");
        } catch(...) {
          // todo: write out some warning here but don't want to clog logfiles for now
        }
        j_trk_d0_truth.push_back(tmpd0T);
        j_trk_z0_truth.push_back(tmpz0T);
      }
    } // end track loop

    v_jet_btag_ntrk->push_back(j_btag_ntrk);
    v_jet_sv1_ntrk->push_back(j_sv1_ntrk);
    v_jet_ip3d_ntrk->push_back(j_ip3d_ntrk);
    v_jet_jf_ntrk->push_back(j_jf_ntrk);
    v_jet_trk_pt->push_back(j_trk_pt);
    v_jet_trk_eta->push_back(j_trk_eta);
    v_jet_trk_theta->push_back(j_trk_theta);
    v_jet_trk_phi->push_back(j_trk_phi);
    v_jet_trk_qoverp->push_back(j_trk_qoverp);
    v_jet_trk_dr->push_back(j_trk_dr);
    v_jet_trk_assoc_msv->push_back(j_trk_assoc_msv);
    v_jet_trk_chi2->push_back(j_trk_chi2);
    v_jet_trk_ndf->push_back(j_trk_ndf);
    v_jet_trk_algo->push_back(j_trk_algo);
    v_jet_trk_orig->push_back(j_trk_orig);
    v_jet_trk_vtx_X->push_back(j_trk_vtx_X);
    v_jet_trk_vtx_Y->push_back(j_trk_vtx_Y);
    v_jet_trk_nInnHits->push_back(j_trk_nInnHits);
    v_jet_trk_nNextToInnHits->push_back(j_trk_nNextToInnHits);
    v_jet_trk_nBLHits->push_back(j_trk_nBLHits);
    v_jet_trk_nsharedBLHits->push_back(j_trk_nsharedBLHits);
    v_jet_trk_nsplitBLHits->push_back(j_trk_nsplitBLHits);
    v_jet_trk_nPixHits->push_back(j_trk_nPixHits);
    v_jet_trk_nsharedPixHits->push_back(j_trk_nsharedPixHits);
    v_jet_trk_nsplitPixHits->push_back(j_trk_nsplitPixHits);
    v_jet_trk_nSCTHits->push_back(j_trk_nSCTHits);
    v_jet_trk_nsharedSCTHits->push_back(j_trk_nsharedSCTHits);
    v_jet_trk_expectBLayerHit->push_back(j_trk_expectBLayerHit);
    v_jet_trk_d0->push_back(j_trk_d0);
    v_jet_trk_z0->push_back(j_trk_z0);
    v_jet_trk_d0_truth->push_back(j_trk_d0_truth);
    v_jet_trk_z0_truth->push_back(j_trk_z0_truth);
    v_jet_trk_IP3D_grade->push_back(j_trk_ip3d_grade);
    v_jet_trk_IP3D_d0->push_back(j_trk_ip3d_d0);
    v_jet_trk_IP3D_d02D->push_back(j_trk_ip3d_d02D);
    v_jet_trk_IP3D_z0->push_back(j_trk_ip3d_z0);
    v_jet_trk_IP3D_d0sig->push_back(j_trk_ip3d_d0sig);
    v_jet_trk_IP3D_z0sig->push_back(j_trk_ip3d_z0sig);

    v_jet_trk_ip_d0->  push_back(j_trk_ip_d0);
    v_jet_trk_ip_z0->  push_back(j_trk_ip_z0);

    v_jet_trk_IP2D_llr->push_back(j_trk_ip2d_llr);
    v_jet_trk_IP3D_llr->push_back(j_trk_ip3d_llr);

    v_jet_trk_jf_Vertex->push_back(j_trk_jf_Vertex); // mod Remco

  } // end jet loop

  for (unsigned int j = 0; j < selJets.size(); j++) {
    delete selJets.at(j);
  }

  if (!isData) tree->Fill();
  else {
    if (njets == 0) return StatusCode::SUCCESS;
    if (v_jet_pt->at(0) < 30e3) return StatusCode::SUCCESS;
    tree->Fill();
  }

  // clear all the things that need clearing
  truth_electrons.clear();
  truth_muons.clear();
  selJets.clear();

  // addition from Dan: clear branch collections
  m_cluster_branches.clear();
  m_substructure_moment_branches.clear();
  m_exkt_branches.clear();
  m_vrtrkjet_branches.clear();
  m_track_branches.clear();
  m_unclustered_vertices.clear();

  return StatusCode::SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// User defined functions
/////////////////////////
float btagIBLAnalysisAlg :: deltaR(float eta1, float eta2, float phi1, float phi2) {
  float DEta = fabs(eta1 - eta2);
  float DPhi = acos(cos(fabs(phi1 - phi2)));
  return sqrt(pow(DEta, 2) + pow(DPhi, 2));
}

const xAOD::Jet *btagIBLAnalysisAlg :: GetParentJet(const xAOD::Jet *Jet, std::string Keyname) {
  ElementLink<xAOD::JetContainer> el = Jet->auxdata<ElementLink<xAOD::JetContainer> >(Keyname);

  if(el.isValid()) {
    return *el;
  }
  else {
    ATH_MSG_WARNING("GetParentJet(): Unable to get parent link %s ! Null ptr is returned.");
    return 0;
  }
}


const xAOD::TruthParticle *btagIBLAnalysisAlg :: truthParticle(const xAOD::TrackParticle *trkPart) const {
  typedef ElementLink< xAOD::TruthParticleContainer > Link_t;
  static const char *NAME = "truthParticleLink";
  if( ! trkPart->isAvailable< Link_t >( NAME ) ) {
    return 0;
  }
  const Link_t& link = trkPart->auxdata< Link_t >( NAME );
  if (!link.isValid()) {
    return 0;
  }
  return *link;
}

bool GoesIntoC(const xAOD::TruthParticle *part) {
  if (!part) return false;
  if (!part->hasDecayVtx()) return false;
  const xAOD::TruthVertex *decayVtx = part->decayVtx();
  for (unsigned int ch = 0; ch < decayVtx->nOutgoingParticles(); ch++) {
    const xAOD::TruthParticle *tmpPart = decayVtx->outgoingParticle(ch);
    if (tmpPart->isCharmHadron()) return true;
  }
  return false;
}

// saving recursively only charged particle
void btagIBLAnalysisAlg :: GetParentTracks(const xAOD::TruthParticle *particle, std::vector<const xAOD::TruthParticle*> &tracksFromB, std::vector<const xAOD::TruthParticle*> &tracksFromC, bool isFromC, std::string indent) {
  if (!particle->hasDecayVtx()) return;
  const xAOD::TruthVertex *decayVtx = particle->decayVtx();
  indent += "  ";
  for (unsigned int ch = 0; ch < decayVtx->nOutgoingParticles(); ch++) {
    const xAOD::TruthParticle *tmpParticle = decayVtx->outgoingParticle(ch);
    if (tmpParticle->barcode() > 200e3) continue;
    if (!tmpParticle->isCharmHadron() && !(const_cast< xAOD::TruthParticle* >(tmpParticle))->isBottomHadron()) {
      if (tmpParticle->isCharged()) tracksFromB.push_back(tmpParticle);
      if (isFromC && tmpParticle->isCharged()) tracksFromC.push_back(tmpParticle);
    }

    if (isFromC) GetParentTracks(tmpParticle, tracksFromB, tracksFromC, true, indent);
    else GetParentTracks(tmpParticle, tracksFromB, tracksFromC, tmpParticle->isCharmHadron() && !GoesIntoC(tmpParticle), indent);
  }
}

void btagIBLAnalysisAlg :: clearvectors() {
  PV_x = -999;
  PV_y = -999;
  PV_z = -999;
  truth_PV_x = -999;
  truth_PV_y = -999;
  truth_PV_z = -999;
  truth_LeadJet_pt = 0;

  PV_jf_x = -999;
  PV_jf_y = -999;
  PV_jf_z = -999;

  v_jet_pt->clear();
  v_jet_eta->clear();
  v_jet_phi->clear();
  v_jet_pt_orig->clear();
  v_jet_eta_orig->clear();
  v_jet_phi_orig->clear();
  v_jet_E_orig  ->clear();
  v_jet_sumtrkS_pt->clear();
  v_jet_sumtrkV_pt->clear();
  v_jet_sumtrkV_eta->clear();
  v_jet_sumtrkV_phi->clear();
  v_jet_sumtrk_ntrk->clear();
  v_jet_E->clear();
  v_jet_m->clear();
  v_jet_nConst->clear();
  v_jet_truthflav->clear();
  v_jet_nBHadr->clear();
  v_jet_nCHadr->clear();
  v_jet_nGhostBHadrFromParent->clear(); // mod nikola
  v_jet_nGhostCHadrFromParent->clear(); // mod nikola
  v_jet_nGhostCHadrFromParentNotFromB->clear(); // mod nikola
  v_jet_nGhostTauFromParent->clear();
  v_jet_nGhostHBosoFromParent->clear();

  v_jet_GhostL_q->clear();
  v_jet_GhostL_HadI->clear();
  v_jet_GhostL_HadF->clear();
  v_jet_LabDr_HadF->clear();
  v_jet_aliveAfterOR->clear();
  v_jet_aliveAfterORmu->clear();
  v_jet_truthMatch->clear();
  v_jet_isBadMedium->clear();
  v_jet_isPU->clear();
  v_jet_truthPt->clear();
  v_jet_dRiso->clear();
  v_jet_JVT->clear();
  v_jet_JVF->clear();
  v_jet_dRminToB->clear();
  v_jet_dRminToC->clear();
  v_jet_dRminToT->clear();

  v_jet_ip2d_pb->clear();
  v_jet_ip2d_pc->clear();
  v_jet_ip2d_pu->clear();
  v_jet_ip2d_llr->clear();

  v_jet_ip3d_pb->clear();
  v_jet_ip3d_pc->clear();
  v_jet_ip3d_pu->clear();
  v_jet_ip3d_llr->clear();

  v_jet_sv0_sig3d->clear();
  v_jet_sv0_ntrkj->clear();
  v_jet_sv0_ntrkv->clear();
  v_jet_sv0_n2t->clear();
  v_jet_sv0_m->clear();
  v_jet_sv0_efc->clear();
  v_jet_sv0_normdist->clear();
  v_jet_sv0_Nvtx->clear();
  v_jet_sv0_vtxx->clear();
  v_jet_sv0_vtxy->clear();
  v_jet_sv0_vtxz->clear();

  v_jet_sv1_ntrkj->clear();
  v_jet_sv1_ntrkv->clear();
  v_jet_sv1_n2t->clear();
  v_jet_sv1_m->clear();
  v_jet_sv1_efc->clear();
  v_jet_sv1_normdist->clear();
  v_jet_sv1_Nvtx->clear();
  v_jet_sv1_pb->clear();
  v_jet_sv1_pc->clear();
  v_jet_sv1_pu->clear();
  v_jet_sv1_llr->clear();
  v_jet_sv1_sig3d->clear();
  v_jet_sv1_vtxx->clear();
  v_jet_sv1_vtxy->clear();
  v_jet_sv1_vtxz->clear();

  v_jet_jf_pb->clear();
  v_jet_jf_pc->clear();
  v_jet_jf_pu->clear();
  v_jet_jf_llr->clear();
  v_jet_jf_m->clear();
  v_jet_jf_mUncorr->clear(); //eloi
  v_jet_jf_efc->clear();
  v_jet_jf_deta->clear();
  v_jet_jf_dphi->clear();
  v_jet_jf_dRFlightDir->clear(); //eloi
  v_jet_jf_ntrkAtVx->clear();
  v_jet_jf_nvtx->clear();
  v_jet_jf_sig3d->clear();
  v_jet_jf_nvtx1t->clear();
  v_jet_jf_n2t->clear();
  v_jet_jf_VTXsize->clear();
  v_jet_jf_vtx_chi2->clear(); // mod Remco
  v_jet_jf_vtx_ndf->clear(); // mod Remco
  v_jet_jf_vtx_ntrk->clear(); // mod Remco
  v_jet_jf_vtx_L3d->clear(); // mod Remco
  v_jet_jf_vtx_sig3d->clear(); // mod Remco
  v_jet_jf_vtx_nvtx->clear(); // mod Remco
  v_jet_jf_phi->clear(); // mod Remco
  v_jet_jf_theta->clear(); // mod Remco

  v_jet_jfcombnn_pb->clear();
  v_jet_jfcombnn_pc->clear();
  v_jet_jfcombnn_pu->clear();
  v_jet_jfcombnn_llr->clear();

  v_jet_dl1_pb->clear();
  v_jet_dl1_pc->clear();
  v_jet_dl1_pu->clear();

  v_jet_sv1ip3d->clear();
  v_jet_mv1->clear();
  v_jet_mv1c->clear();
  v_jet_mv2c00->clear();
  v_jet_mv2c10->clear();
  v_jet_mv2c20->clear();
  v_jet_mv2c100->clear();
  v_jet_mv2cl100->clear();
  v_jet_mv2m_pu->clear();
  v_jet_mv2m_pb->clear();
  v_jet_mv2m_pc->clear();
  v_jet_mvb->clear();

  v_jet_ip2dNT_llr->clear();
  v_jet_ip3dNT_llr->clear();
  v_jet_sv1flip_llr->clear();
  v_jet_jfflip_llr->clear();
  v_jet_mv2c20flip->clear();

  v_jet_multisvbb1->clear();
  v_jet_multisvbb2->clear();
  v_jet_msv_N2Tpair->clear();
  v_jet_msv_energyTrkInJet->clear();
  v_jet_msv_nvsec->clear();
  v_jet_msv_normdist->clear();
  v_jet_msv_vtx_cov0->clear();
  v_jet_msv_vtx_cov1->clear();
  v_jet_msv_vtx_cov2->clear();
  v_jet_msv_vtx_cov3->clear();
  v_jet_msv_vtx_cov4->clear();
  v_jet_msv_vtx_cov5->clear();
  v_jet_msv_vtx_mass->clear();
  v_jet_msv_vtx_efrc->clear();
  v_jet_msv_vtx_ntrk->clear();
  v_jet_msv_vtx_pt->clear();
  v_jet_msv_vtx_eta->clear();
  v_jet_msv_vtx_phi->clear();
  v_jet_msv_vtx_dls->clear();
  v_jet_msv_vtx_x->clear();
  v_jet_msv_vtx_y->clear();
  v_jet_msv_vtx_z->clear();
  v_jet_msv_vtx_chi->clear();
  v_jet_msv_vtx_ndf->clear();

  v_jet_ExKtbb_Hbb_DoubleMV2c20->clear();
  v_jet_ExKtbb_Hbb_SingleMV2c20->clear();
  v_jet_ExKtbb_Hbb_MV2Only->clear();
  v_jet_ExKtbb_Hbb_MV2andJFDRSig->clear();
  v_jet_ExKtbb_Hbb_MV2andTopos->clear();

  v_bH1_pt->clear();
  v_bH1_eta->clear();
  v_bH1_phi->clear();
  v_bH1_Lxy->clear();
  v_bH1_dRjet->clear();
  v_bH1_x->clear();
  v_bH1_y->clear();
  v_bH1_z->clear();

  v_bH2_pt->clear();
  v_bH2_eta->clear();
  v_bH2_phi->clear();
  v_bH2_Lxy->clear();
  v_bH2_dRjet->clear();
  v_bH2_x->clear();
  v_bH2_y->clear();
  v_bH2_z->clear();

  v_bH_pt->clear();
  v_bH_eta->clear();
  v_bH_phi->clear();
  v_bH_Lxy->clear();
  v_bH_dRjet->clear();
  v_bH_x->clear();
  v_bH_y->clear();
  v_bH_z->clear();
  v_bH_nBtracks->clear();
  v_bH_nCtracks->clear();

  v_cH_pt->clear();
  v_cH_eta->clear();
  v_cH_phi->clear();
  v_cH_Lxy->clear();
  v_cH_dRjet->clear();
  v_cH_x->clear();
  v_cH_y->clear();
  v_cH_z->clear();
  v_cH_nCtracks->clear();

  v_jet_btag_ntrk->clear();
  v_jet_trk_pt->clear();
  v_jet_trk_eta->clear();
  v_jet_trk_theta->clear();
  v_jet_trk_phi->clear();
  v_jet_trk_qoverp->clear();
  v_jet_trk_dr->clear();
  v_jet_trk_assoc_msv->clear();
  v_jet_trk_chi2->clear();
  v_jet_trk_ndf->clear();
  v_jet_trk_algo->clear();
  v_jet_trk_orig->clear();
  v_jet_trk_nInnHits->clear();
  v_jet_trk_nNextToInnHits->clear();
  v_jet_trk_nBLHits->clear();
  v_jet_trk_nsharedBLHits->clear();
  v_jet_trk_nsplitBLHits->clear();
  v_jet_trk_nPixHits->clear();
  v_jet_trk_nsharedPixHits->clear();
  v_jet_trk_nsplitPixHits->clear();
  v_jet_trk_nSCTHits->clear();
  v_jet_trk_nsharedSCTHits->clear();
  v_jet_trk_expectBLayerHit->clear();
  v_jet_trk_d0->clear();
  v_jet_trk_z0->clear();
  v_jet_trk_d0_truth->clear();
  v_jet_trk_z0_truth->clear();
  v_jet_trk_IP3D_grade->clear();
  v_jet_trk_IP3D_d0->clear();
  v_jet_trk_IP3D_d02D->clear();
  v_jet_trk_IP3D_z0->clear();
  v_jet_trk_IP3D_d0sig->clear();
  v_jet_trk_IP3D_z0sig->clear();

  v_jet_trk_ip_d0->clear();
  v_jet_trk_ip_z0->clear();


  v_jet_trk_vtx_X->clear();
  v_jet_trk_vtx_Y->clear();
  v_jet_trk_vtx_dx->clear();
  v_jet_trk_vtx_dy->clear();

  v_jet_trk_IP2D_llr->clear();
  v_jet_trk_IP3D_llr->clear();

  v_jet_trk_jf_Vertex->clear(); // mod Remco

  v_jet_sv1_ntrk->clear();
  v_jet_ip3d_ntrk->clear();
  v_jet_jf_ntrk->clear();

  // MVb variables
  v_jet_width->clear();
  v_jet_n_trk_sigd0cut->clear();
  v_jet_trk3_d0sig->clear();
  v_jet_trk3_z0sig->clear();
  v_jet_sv_scaled_efc->clear();
  v_jet_jf_scaled_efc->clear();

  // additions by andrea
  v_jet_mu_pt->clear();
  v_jet_mu_eta->clear();
  v_jet_mu_phi->clear();
  v_jet_mu_qOverPratio->clear();
  v_jet_mu_dR->clear();
  v_jet_mu_d0->clear();
  v_jet_mu_z0->clear();
  v_jet_mu_VtxTyp->clear();
  v_jet_mu_mombalsignif->clear();
  v_jet_mu_scatneighsignif->clear();
  v_jet_mu_pTrel->clear();
  v_jet_mu_truthflav->clear();
  v_jet_mu_parent_pdgid->clear();
  v_jet_mu_ID_qOverP_var->clear();
  v_jet_mu_muonType->clear();
  v_jet_mu_assJet_pt->clear();
  // additions by nikola
  v_jet_mu_fatjet_nMu->clear();
  v_jet_mu_fatjet_pTmax_pT->clear();
  v_jet_mu_fatjet_pTmax_pTrel->clear();
  v_jet_mu_fatjet_pTmax_pTrelFrac->clear();
}

int btagIBLAnalysisAlg :: parent_classify(const xAOD::TruthParticle *theParticle) {
  const xAOD::TruthParticle *parent = 0; // the parent object
  Int_t particle_id = 999;
  Int_t parent_id = 999;

  if (theParticle == NULL) return parent_id;

  particle_id = theParticle->pdgId();
  parent = theParticle->parent(0);
  if (parent) parent_id = parent->pdgId();
  else return parent_id;

  while (fabs(parent_id) == fabs(particle_id) && fabs(parent_id) < 400 && fabs(parent_id) != 0) {
    parent = parent->parent(0);
    if (parent) parent_id = parent->pdgId();
    else break;
  }
  return parent_id;
}

int btagIBLAnalysisAlg :: getTrackOrigin(const xAOD::TrackParticle *tmpTrk,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCc,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB1,
                                         std::vector<const xAOD::TruthParticle*> tracksFromB2,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC1,
                                         std::vector<const xAOD::TruthParticle*> tracksFromC2,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB1,
                                         std::vector<const xAOD::TruthParticle*> tracksFromCNotFromB2) {
      // origin
      int origin = PUFAKE;
      const xAOD::TruthParticle *truth = truthParticle(tmpTrk);
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
          // additions by nikola
          for (unsigned int iT = 0; iT < tracksFromB1.size(); iT++) {
            if (truth == tracksFromB1.at(iT)) {
              origin = 10;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromB2.size(); iT++) {
            if (truth == tracksFromB2.at(iT)) {
              origin = 11;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromC1.size(); iT++) {
            if (truth == tracksFromC1.at(iT)) {
              origin = 12;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromC2.size(); iT++) {
            if (truth == tracksFromC2.at(iT)) {
              origin = 13;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromCNotFromB1.size(); iT++) {
            if (truth == tracksFromCNotFromB1.at(iT)) {
              origin = 14;
              break;
            }
          }
          for (unsigned int iT = 0; iT < tracksFromCNotFromB2.size(); iT++) {
            if (truth == tracksFromCNotFromB2.at(iT)) {
              origin = 15;
              break;
            }
          }
        }
      }
  return origin;
}

