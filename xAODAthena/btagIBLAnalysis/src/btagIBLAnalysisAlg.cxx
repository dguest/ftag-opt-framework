///////////////////////////////////////
// btagIBLAnalysisAlg.cxx
///////////////////////////////////////
// Author(s): marx@cern.ch
// Description: athena-based code to process xAODs 
///////////////////////////////////////
#include "GaudiKernel/ITHistSvc.h"
#include "GaudiKernel/ServiceHandle.h"

#include "btagIBLAnalysisAlg.h"
#include "GAFlavourLabel.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODJet/JetContainer.h"

#include "JetInterface/IJetSelector.h"
#include "JetCalibTools/IJetCalibrationTool.h"

using xAOD::IParticle;


///////////////////////////////////////////////////////////////////////////////////
btagIBLAnalysisAlg::btagIBLAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthHistogramAlgorithm( name, pSvcLocator )
  ,  m_jetCalibrationTool("JetCalibrationTool/JetCalibrationTool",this) ,  m_jetCleaningTool("JetCleaningTool/JetCleaningTool",this) {

  declareProperty( "JetCleaningTool", m_jetCleaningTool );
  declareProperty( "JetCalibrationTool", m_jetCalibrationTool );

}


btagIBLAnalysisAlg::~btagIBLAnalysisAlg() {}

///////////////////////////////////////////////////////////////////////////////////
StatusCode btagIBLAnalysisAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  // Register histograms
  //ATH_CHECK( book( TH1F("hist_Lxy_denom", "Lxy", 200, 0.0, 100.0) ) );
  //ATH_CHECK( book( TH1F("hist_Lxy", "Lxy", 200, 0.0, 100.0) ) );
 
  // Register output tree
  ServiceHandle<ITHistSvc> histSvc("THistSvc",name());
  CHECK( histSvc.retrieve() );
  tree = new TTree("bTag","bTag");
  CHECK( histSvc->regTree("/BTAGSTREAM/tree",tree) );

  // Retrieve the jet cleaning tool
  CHECK( m_jetCleaningTool.retrieve() );

  // Retrieve the jet calibration tool
  CHECK( m_jetCalibrationTool.retrieve() );

  // Setup branches
  v_jet_pt =new std::vector<float>(); v_jet_pt->reserve(15);
  v_jet_eta=new std::vector<float>(); v_jet_eta->reserve(15);
  v_jet_phi=new std::vector<float>(); v_jet_phi->reserve(15);
  v_jet_E  =new std::vector<float>(); v_jet_E->reserve(15);
  v_jet_m  =new std::vector<float>(); v_jet_m->reserve(15);
  v_jet_truthflav  =new std::vector<int>();
  v_jet_GhostL_q   =new std::vector<int>();
  v_jet_GhostL_HadI=new std::vector<int>();
  v_jet_GhostL_HadF=new std::vector<int>();
  v_jet_aliveAfterOR =new std::vector<int>();
  v_jet_truthMatch =new std::vector<int>();
  v_jet_truthPt =new std::vector<float>();
  v_jet_dRiso   =new std::vector<float>();

  v_jet_ip2d_pb   =new std::vector<float>();
  v_jet_ip2d_pc   =new std::vector<float>();
  v_jet_ip2d_pu   =new std::vector<float>();
  v_jet_ip2d_llr  =new std::vector<float>();
  v_jet_ip3d_pb   =new std::vector<float>();
  v_jet_ip3d_pc   =new std::vector<float>();
  v_jet_ip3d_pu   =new std::vector<float>();
  v_jet_ip3d_llr  =new std::vector<float>();
  v_jet_sv0_sig3d  =new std::vector<float>();
  v_jet_sv0_ntrkv  =new std::vector<float>();
  v_jet_sv0_m      =new std::vector<float>();
  v_jet_sv0_efc    =new std::vector<float>();
  v_jet_sv0_n2t    =new std::vector<float>();
  v_jet_sv1_pb   =new std::vector<float>();
  v_jet_sv1_pc   =new std::vector<float>();
  v_jet_sv1_pu   =new std::vector<float>();
  v_jet_sv1_llr  =new std::vector<float>();
  
  v_jet_jf_pb=new std::vector<float>();
  v_jet_jf_pc=new std::vector<float>();
  v_jet_jf_pu=new std::vector<float>();
  v_jet_jf_llr=new std::vector<float>();
  v_jet_jf_m=new std::vector<float>();
  v_jet_jf_efc=new std::vector<float>();
  v_jet_jf_deta=new std::vector<float>();
  v_jet_jf_dphi=new std::vector<float>();
  v_jet_jf_ntrkAtVx=new std::vector<float>();
  v_jet_jf_nvtx=new std::vector<int>();
  v_jet_jf_sig3d=new std::vector<int>();
  v_jet_jf_nvtx1t=new std::vector<int>();
  v_jet_jf_n2t=new std::vector<int>();

  v_jet_jfcombnn_pb=new std::vector<float>();
  v_jet_jfcombnn_pc=new std::vector<float>();
  v_jet_jfcombnn_pu=new std::vector<float>();
  v_jet_jfcombnn_llr=new std::vector<float>();
  
  v_jet_sv1ip3d=new std::vector<double>();
  v_jet_mv1=new std::vector<double>();
  v_jet_mv1c=new std::vector<double>();
  v_jet_mv2c00=new std::vector<double>();
  v_jet_mv2c10=new std::vector<double>();
  v_jet_mv2c20=new std::vector<double>();
  v_jet_mvb=new std::vector<double>();

  v_bH_pt=new std::vector<float>();
  v_bH_eta=new std::vector<float>();
  v_bH_phi=new std::vector<float>();
  v_bH_Lxy=new std::vector<float>();
  v_bH_dRjet=new std::vector<float>();


  tree->Branch("runnb",&runnumber);
  tree->Branch("eventnb",&eventnumber);
  tree->Branch("mcchan",&mcchannel);
  tree->Branch("mcwg",&mcweight);

  tree->Branch("njets",&njets);
  tree->Branch("nbjets"     ,&nbjets);
  tree->Branch("nbjets_q"   ,&nbjets_q);
  tree->Branch("nbjets_HadI",&nbjets_HadI);
  tree->Branch("nbjets_HadF",&nbjets_HadF);
  tree->Branch("jet_pt",&v_jet_pt);
  tree->Branch("jet_eta",&v_jet_eta);
  tree->Branch("jet_phi",&v_jet_phi);
  tree->Branch("jet_E",&v_jet_E);
  tree->Branch("jet_m",&v_jet_m);
  tree->Branch("jet_truthflav"  ,&v_jet_truthflav);
  tree->Branch("jet_GhostL_q"   ,&v_jet_GhostL_q);
  tree->Branch("jet_GhostL_HadI",&v_jet_GhostL_HadI);
  tree->Branch("jet_GhostL_HadF",&v_jet_GhostL_HadF);
  tree->Branch("jet_aliveAfterOR" ,&v_jet_aliveAfterOR);
  tree->Branch("jet_truthMatch" ,&v_jet_truthMatch);
  tree->Branch("jet_truthPt" ,&v_jet_truthPt);
  tree->Branch("jet_dRiso" ,&v_jet_dRiso);

  tree->Branch("jet_ip2d_pb",&v_jet_ip2d_pb);
  tree->Branch("jet_ip2d_pc",&v_jet_ip2d_pc);
  tree->Branch("jet_ip2d_pu",&v_jet_ip2d_pu);
  tree->Branch("jet_ip2d_llr",&v_jet_ip2d_llr);

  tree->Branch("jet_ip3d_pb",&v_jet_ip3d_pb);
  tree->Branch("jet_ip3d_pc",&v_jet_ip3d_pc);
  tree->Branch("jet_ip3d_pu",&v_jet_ip3d_pu);
  tree->Branch("jet_ip3d_llr",&v_jet_ip3d_llr);

  tree->Branch("jet_sv0_sig3d",&v_jet_sv0_sig3d);
  tree->Branch("jet_sv0_ntrkv",&v_jet_sv0_ntrkv);
  tree->Branch("jet_sv0_m",&v_jet_sv0_m);
  tree->Branch("jet_sv0_efc",&v_jet_sv0_efc);
  tree->Branch("jet_sv0_n2t",&v_jet_sv0_n2t);

  tree->Branch("jet_sv1_pb",&v_jet_sv1_pb);
  tree->Branch("jet_sv1_pc",&v_jet_sv1_pc);
  tree->Branch("jet_sv1_pu",&v_jet_sv1_pu);
  tree->Branch("jet_sv1_llr",&v_jet_sv1_llr);

  tree->Branch("jet_jf_pb",&v_jet_jf_pb);
  tree->Branch("jet_jf_pc",&v_jet_jf_pc);
  tree->Branch("jet_jf_pu",&v_jet_jf_pu);
  tree->Branch("jet_jf_llr",&v_jet_jf_llr);
  tree->Branch("jet_jf_m",&v_jet_jf_m);
  tree->Branch("jet_jf_efc",&v_jet_jf_efc);
  tree->Branch("jet_jf_deta",&v_jet_jf_deta);
  tree->Branch("jet_jf_dphi",&v_jet_jf_dphi);
  tree->Branch("jet_jf_ntrkAtVx",&v_jet_jf_ntrkAtVx);
  tree->Branch("jet_jf_nvtx",&v_jet_jf_nvtx);
  tree->Branch("jet_jf_sig3d",&v_jet_jf_sig3d);
  tree->Branch("jet_jf_nvtx1t",&v_jet_jf_nvtx1t);
  tree->Branch("jet_jf_n2t",&v_jet_jf_n2t);

  tree->Branch("jet_jfcombnn_pb",&v_jet_jfcombnn_pb);
  tree->Branch("jet_jfcombnn_pc",&v_jet_jfcombnn_pc);
  tree->Branch("jet_jfcombnn_pu",&v_jet_jfcombnn_pu);
  tree->Branch("jet_jfcombnn_llr",&v_jet_jfcombnn_llr);

  tree->Branch("jet_sv1ip3d",&v_jet_sv1ip3d);
  tree->Branch("jet_mv1",&v_jet_mv1);
  tree->Branch("jet_mv1c",&v_jet_mv1c);
  tree->Branch("jet_mv2c00",&v_jet_mv2c00);
  tree->Branch("jet_mv2c10",&v_jet_mv2c10);
  tree->Branch("jet_mv2c20",&v_jet_mv2c20);
  tree->Branch("jet_mvb",&v_jet_mvb);

  tree->Branch("bH_pt",&v_bH_pt);
  tree->Branch("bH_eta",&v_bH_eta);
  tree->Branch("bH_phi",&v_bH_phi);
  tree->Branch("bH_Lxy",&v_bH_Lxy);
  tree->Branch("bH_dRjet",&v_bH_dRjet);
  
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

  clearvectors();
  //-------------------------
  // Event information
  //--------------------------- 

  const xAOD::EventInfo* eventInfo = 0;
  CHECK( evtStore()->retrieve(eventInfo, "EventInfo") );  // with key name
  
  // check if data or MC
  if(!eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION ) ){
    ATH_MSG_DEBUG( "DATA. Will stop processing this algorithm for the current event.");
    return StatusCode::SUCCESS;
  }

  runnumber   = eventInfo->runNumber();
  eventnumber = eventInfo->eventNumber();
  mcchannel   = eventInfo->mcChannelNumber();
  mcweight    = eventInfo->mcEventWeight();


  //---------------------------
  // Truth stuff
  //--------------------------- 

  const xAOD::TruthEventContainer* xTruthEventContainer = NULL;
  CHECK( evtStore()->retrieve( xTruthEventContainer, "TruthEvent") );
  
  // select truth electrons for electron-jet overlap removal
  std::vector<TLorentzVector> truth_electrons;
  for ( const auto* truth : *xTruthEventContainer ) {
    for(unsigned int i = 0; i < truth->nTruthParticles(); i++){
      const xAOD::TruthParticle* particle = truth->truthParticle(i);
      if (particle->pt() < 15e3) continue;
      if (particle->status() != 1) continue;
      if (particle->barcode() > 2e5) continue;
      if (fabs(particle->pdgId()) != 11) continue;
      // see if this electron is coming from a W boson decay
      bool isfromW = false;
      const xAOD::TruthVertex* prodvtx = particle->prodVtx();
      for(unsigned j = 0; j < prodvtx->nIncomingParticles(); j++){
	if( fabs( (prodvtx->incomingParticle(j))->pdgId() ) == 24 ) isfromW = true;
      }
      if(!isfromW) continue;
      TLorentzVector telec;
      telec.SetPtEtaPhiM(particle->pt(), particle->eta(), particle->phi(), particle->m());
      truth_electrons.push_back(telec);
    }
  }

  //---------------------------
  // Jets
  //--------------------------- 
 
  const xAOD::JetContainer* jets = 0;
  CHECK( evtStore()->retrieve( jets, "AntiKt4LCTopoJets") ); // what about BTagging_AntiKt4Truth?

  const xAOD::JetContainer* truthjets = 0;
  CHECK( evtStore()->retrieve( truthjets, "AntiKt4TruthJets") );

  njets=0;
  nbjets=0;
  nbjets_q=0;
  nbjets_HadI=0;
  nbjets_HadF=0;

  // Loop over jets, apply calibration and only keep the ones with pT > 20GeV, eta < 2.5
  std::vector<const xAOD::Jet*> selJets; selJets.reserve(10);

  for ( const auto* jet : *jets ) {
    xAOD::Jet * newjet = 0;

    m_jetCalibrationTool->calibratedCopy(*jet, newjet);

    if ( newjet->pt() < 20e3 )         continue;
    if ( fabs( newjet->eta() ) > 2.5)  continue;

    selJets.push_back(newjet);
  }

  njets = selJets.size();
  ATH_MSG_DEBUG( "Total number of jets is: "<< njets );

  // Now run over the selected jets and do whatever else needs doing
  for (unsigned int j=0; j<selJets.size(); j++) {
    const xAOD::Jet* jet=selJets.at(j);

    // flagging jets that overlap with electron, eventually these should just be removed
    bool iseljetoverlap = false;

    for(unsigned int i= 0; i < truth_electrons.size(); i++){
      float dr =deltaR(jet->eta(), truth_electrons.at(i).Eta(),jet->phi(), truth_electrons.at(i).Phi());
      if(dr < 0.3) iseljetoverlap = true;
    }

    if ( iseljetoverlap ) v_jet_aliveAfterOR->push_back(0);
    else v_jet_aliveAfterOR->push_back(1);

    // jet cleaning - should be done after lepton overlap removal
    if( (!m_jetCleaningTool->keep( *jet )) && (jet->pt() > 20e3) ) return StatusCode::SUCCESS;

    v_jet_pt ->push_back(jet->pt()  );
    v_jet_eta->push_back(jet->eta() );
    v_jet_phi->push_back(jet->phi() );
    v_jet_E  ->push_back(jet->e()   );
    v_jet_m  ->push_back(jet->m()   );
    
    float dRiso=10;
    for (unsigned int j2=0; j2<selJets.size(); j2++) {
      if (j2==j) continue;
      const xAOD::Jet* jet2=selJets.at(j2);
      float dr =deltaR(jet->eta(), jet2->eta(),
		       jet->phi(), jet2->phi());
      if (dr<dRiso) dRiso=dr;
    }
    v_jet_dRiso  ->push_back(dRiso);

    // matching reco jets to truth jets
    // picking the highest pT truth jet (with pT > 7GeV) that satisfies dR < 0.3
    // N.B. this assumes that truth jets are pT ordered
    int matchedPt=0;
    float dRmatch=100;
    for ( const auto* tjet : *truthjets ) {
      if(tjet->pt() < 7e3) continue;
      float dr =deltaR(jet->eta(), tjet->eta(),
		       jet->phi(), tjet->phi());

      if(dr < 0.3){
	dRmatch=dr;
	matchedPt=tjet->pt();
	break;
      }

      // LM: commenting out the old way which used closest dR
      /*if (dr<dRmatch) {
	dRmatch=dr;
	matchedPt=tjet->pt();
	}*/
    }
    if (dRmatch<0.3) {
      v_jet_truthMatch->push_back(1);
      v_jet_truthPt   ->push_back(matchedPt);
    } else {
      v_jet_truthMatch->push_back(0);
      v_jet_truthPt   ->push_back(0);
    }

    //std::vector<float> testjvf = jet->auxdata<std::vector<float> >("JVF"); //todo: pick the right vertex
    
    // Get flavour truth label
    int thisJetTruthLabel;
    jet->getAttribute("TruthLabelID",thisJetTruthLabel);
    v_jet_truthflav->push_back(thisJetTruthLabel);
    if(thisJetTruthLabel == 5) nbjets++;
    
    int tmpLabel=  GAFinalPartonFlavourLabel(jet);
    v_jet_GhostL_q->push_back(tmpLabel);
    if(tmpLabel == 5)  nbjets_q++;
    
    tmpLabel=  GAInitialHadronFlavourLabel(jet);
    v_jet_GhostL_HadI->push_back(tmpLabel);
    if(tmpLabel == 5)  nbjets_HadI++;
    
    tmpLabel=  GAFinalHadronFlavourLabel(jet);
    v_jet_GhostL_HadF->push_back(tmpLabel);
    if(tmpLabel == 5)  nbjets_HadF++;
    
    // get B hadron quantities
    const xAOD::TruthParticle* matchedBH=NULL;
    const std::string labelB = "GhostBHadronsFinal";
    std::vector<const IParticle*> ghostB; ghostB.reserve(2);
    jet->getAssociatedObjects<IParticle>(labelB, ghostB);
    if (ghostB.size() >=1 ) {
       matchedBH=(const xAOD::TruthParticle*)(ghostB.at(0));
       // to do: in case of 2, get the closest
    } 

    if ( matchedBH!=NULL ) {
      v_bH_pt   ->push_back(matchedBH->pt());
      v_bH_eta  ->push_back(matchedBH->eta());
      v_bH_phi  ->push_back(matchedBH->phi());
      float Lxy=sqrt( pow(matchedBH->decayVtx()->x(),2) + pow(matchedBH->decayVtx()->y(),2) );
      v_bH_Lxy  ->push_back(Lxy);
      float dr =deltaR(jet->eta(), matchedBH->eta(),
		       jet->phi(), matchedBH->phi());
      v_bH_dRjet->push_back(dr);
    } else {
      v_bH_pt   ->push_back(-999);
      v_bH_eta  ->push_back(-999);
      v_bH_phi  ->push_back(-999);
      v_bH_Lxy  ->push_back(-999);
      v_bH_dRjet->push_back(-999);
    }

    // Get b-tag object
    const xAOD::BTagging* bjet = jet->btagging();
    
    // IP2D
    v_jet_ip2d_pb->push_back(bjet->IP2D_pb());
    v_jet_ip2d_pc->push_back(bjet->IP2D_pc());
    v_jet_ip2d_pu->push_back(bjet->IP2D_pu());
    v_jet_ip2d_llr->push_back(bjet->IP2D_loglikelihoodratio());
    
    // IP3D
    v_jet_ip3d_pb->push_back(bjet->IP3D_pb());
    v_jet_ip3d_pc->push_back(bjet->IP3D_pc());
    v_jet_ip3d_pu->push_back(bjet->IP3D_pu());
    v_jet_ip3d_llr->push_back(bjet->IP3D_loglikelihoodratio());
    
    // SV0
    v_jet_sv0_sig3d->push_back(bjet->SV0_significance3D());
    int sv0ntrkv;
    bjet->taggerInfo(sv0ntrkv, xAOD::SV0_NGTinSvx);
    v_jet_sv0_ntrkv->push_back(sv0ntrkv);
    float sv0m;
    bjet->taggerInfo(sv0m, xAOD::SV0_masssvx);
    v_jet_sv0_m->push_back(sv0m);
    float sv0efc;
    bjet->taggerInfo(sv0efc, xAOD::SV0_efracsvx);
    v_jet_sv0_efc->push_back(sv0efc);
    int sv0n2t;
    bjet->taggerInfo(sv0n2t, xAOD::SV0_N2Tpair);
    v_jet_sv0_n2t->push_back(sv0n2t);
    
    // SV1
    v_jet_sv1_pb->push_back(bjet->SV1_pb());
    v_jet_sv1_pc->push_back(bjet->SV1_pc());
    v_jet_sv1_pu->push_back(bjet->SV1_pu());
    v_jet_sv1_llr->push_back(bjet->SV1_loglikelihoodratio());
    
    // JetFitter
    v_jet_jf_pb->push_back(bjet->JetFitter_pb());
    v_jet_jf_pc->push_back(bjet->JetFitter_pc());
    v_jet_jf_pu->push_back(bjet->JetFitter_pu());
    v_jet_jf_llr->push_back(bjet->JetFitter_loglikelihoodratio());
    float jfm;
    bjet->taggerInfo(jfm, xAOD::JetFitter_mass);
    v_jet_jf_m->push_back(jfm);
    float jfefc;
    bjet->taggerInfo(jfefc, xAOD::JetFitter_energyFraction);
    v_jet_jf_efc->push_back(jfefc);
    float jfdeta;
    bjet->taggerInfo(jfdeta, xAOD::JetFitter_deltaeta);
    v_jet_jf_deta->push_back(jfdeta);
    float jfdphi;
    bjet->taggerInfo(jfdphi, xAOD::JetFitter_deltaphi);
    v_jet_jf_dphi->push_back(jfdphi);
    int jfntrkAtVx;
    bjet->taggerInfo(jfntrkAtVx, xAOD::JetFitter_nTracksAtVtx);
    v_jet_jf_ntrkAtVx->push_back(jfntrkAtVx);
    int jfnvtx;
    bjet->taggerInfo(jfnvtx, xAOD::JetFitter_nVTX);
    v_jet_jf_nvtx->push_back(jfnvtx);
    float jfsig3d;
    bjet->taggerInfo(jfsig3d, xAOD::JetFitter_significance3d);
    v_jet_jf_sig3d->push_back(jfsig3d);
    int jfnvtx1t;
    bjet->taggerInfo(jfnvtx1t, xAOD::JetFitter_nSingleTracks);
    v_jet_jf_nvtx1t->push_back(jfnvtx1t);
    int jfn2t;
    bjet->taggerInfo(jfn2t, xAOD::JetFitter_N2Tpair);
    v_jet_jf_n2t->push_back(jfn2t);
    
    // JetFitterCombNN
    v_jet_jfcombnn_pb->push_back(bjet->JetFitterCombNN_pb());
    v_jet_jfcombnn_pc->push_back(bjet->JetFitterCombNN_pc());
    v_jet_jfcombnn_pu->push_back(bjet->JetFitterCombNN_pu());
    v_jet_jfcombnn_llr->push_back(bjet->JetFitterCombNN_loglikelihoodratio());
    
    // Other
    v_jet_sv1ip3d->push_back(bjet->SV1plusIP3D_discriminant());
    v_jet_mv1    ->push_back(bjet->MV1_discriminant());
    try{
      v_jet_mv1c   ->push_back(bjet->auxdata<double>("MV1c_discriminant"));
      v_jet_mv2c00 ->push_back(bjet->auxdata<double>("MV2c00_discriminant"));
      v_jet_mv2c10 ->push_back(bjet->auxdata<double>("MV2c10_discriminant"));
      v_jet_mv2c20 ->push_back(bjet->auxdata<double>("MV2c20_discriminant"));
      v_jet_mvb    ->push_back(bjet->auxdata<double>("MVb_discriminant"));
    }
    catch(...){
      //todo: write out some warning here but don't want to clog logfiles for now
    }
    
  } // jet loop
 
  tree->Fill();

  // clear all the things that need clearing
  truth_electrons.clear();
  selJets.clear();
  
  return StatusCode::SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// User defined functions
///////////////////////////////////////
const int allBHadrons[75]={511,513,515,521,523,525,531,533,535,541,543,545,551,553,555,
			   5101,5103,5112,5114,5122,5132,5142,5201,5203,5212,5214,5222,5224,
			   5232,5242,5301,5303,5312,5314,5322,5324,5332,5334,5342,5401,5403,
			   5412,5414,5422,5424,5432,5434,5442,5444,5503,5512,5514,5522,5524,
			   5532,5534,5542,5544,5554,10511,10513,10521,10523,10531,10533,10541,
			   10543,10551,10553,20513,20523,20533,20543,20553,100553};
  

bool btagIBLAnalysisAlg :: isBHadron(int pdgid){
  
  bool isB = false;
  for(int i = 0; i < 75; i++){
    if(fabs(pdgid) == allBHadrons[i]) return true;
  }
  return isB;
}

float btagIBLAnalysisAlg :: deltaR(float eta1, float eta2, float phi1, float phi2) {
  float DEta=fabs( eta1-eta2 );
  float DPhi=acos(cos( fabs( phi1-phi2 ) ) );
  return sqrt( pow(DEta,2)+pow(DPhi,2) );
}

void btagIBLAnalysisAlg :: clearvectors(){
  v_jet_pt->clear();
  v_jet_eta->clear();
  v_jet_phi->clear();
  v_jet_E->clear();
  v_jet_m->clear();
  v_jet_truthflav->clear();
  v_jet_GhostL_q->clear();
  v_jet_GhostL_HadI->clear();
  v_jet_GhostL_HadF->clear();
  v_jet_aliveAfterOR->clear();
  v_jet_truthMatch->clear();
  v_jet_truthPt->clear();
  v_jet_dRiso->clear();;

  v_jet_ip2d_pb->clear();
  v_jet_ip2d_pc->clear();
  v_jet_ip2d_pu->clear();
  v_jet_ip2d_llr->clear();

  v_jet_ip3d_pb->clear();
  v_jet_ip3d_pc->clear();
  v_jet_ip3d_pu->clear();
  v_jet_ip3d_llr->clear();

  v_jet_sv0_sig3d->clear();
  v_jet_sv0_ntrkv->clear();
  v_jet_sv0_m->clear();
  v_jet_sv0_efc->clear();
  v_jet_sv0_n2t->clear();

  v_jet_sv1_pb->clear();
  v_jet_sv1_pc->clear();
  v_jet_sv1_pu->clear();
  v_jet_sv1_llr->clear();

  v_jet_jf_pb->clear();
  v_jet_jf_pc->clear();
  v_jet_jf_pu->clear();
  v_jet_jf_llr->clear();
  v_jet_jf_m->clear();
  v_jet_jf_efc->clear();
  v_jet_jf_deta->clear();
  v_jet_jf_dphi->clear();
  v_jet_jf_ntrkAtVx->clear();
  v_jet_jf_nvtx->clear();
  v_jet_jf_sig3d->clear();
  v_jet_jf_nvtx1t->clear();
  v_jet_jf_n2t->clear();

  v_jet_jfcombnn_pb->clear();
  v_jet_jfcombnn_pc->clear();
  v_jet_jfcombnn_pu->clear();
  v_jet_jfcombnn_llr->clear();

  v_jet_sv1ip3d->clear();
  v_jet_mv1->clear();
  v_jet_mv1c->clear();
  v_jet_mv2c00->clear();
  v_jet_mv2c10->clear();
  v_jet_mv2c20->clear();
  v_jet_mvb->clear();

  v_bH_pt->clear();
  v_bH_eta->clear();
  v_bH_phi->clear();
  v_bH_Lxy->clear();
  v_bH_dRjet->clear();

}

