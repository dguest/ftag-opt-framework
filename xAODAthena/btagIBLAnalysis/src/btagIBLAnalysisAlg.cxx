///////////////////////////////////////
// btagIBLAnalysisAlg.cxx
///////////////////////////////////////
// Author(s): marx@cern.ch
// Description: athena-based code to process xAODs 
///////////////////////////////////////

#include "btagIBLAnalysisAlg.h"
#include "GAFlavourLabel.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODJet/JetContainer.h"

#include "JetInterface/IJetSelector.h"

using xAOD::IParticle;

/////////////////////////////////////////////////////////////////////////////////////
float deltaR(float eta1, float eta2, float phi1, float phi2) {
  float DEta=fabs( eta1-eta2 );
  float DPhi=acos(cos( fabs( phi1-phi2 ) ) );
  return sqrt( pow(DEta,2)+pow(DPhi,2) );
}


///////////////////////////////////////////////////////////////////////////////////
btagIBLAnalysisAlg::btagIBLAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthHistogramAlgorithm( name, pSvcLocator ),  m_jetCleaningTool("JetCleaningTool/JetCleaningTool",this) {

  declareProperty( "JetCleaningTool", m_jetCleaningTool );

}


btagIBLAnalysisAlg::~btagIBLAnalysisAlg() {}


StatusCode btagIBLAnalysisAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  //ATH_CHECK( book( TH1F("hist_Lxy_denom", "Lxy", 200, 0.0, 100.0) ) );
  //ATH_CHECK( book( TH1F("hist_Lxy", "Lxy", 200, 0.0, 100.0) ) );
 
  // define output file for ntuple, tree and branches
  output = new TFile("flavntuple_110401_ttbar.root","recreate");
  tree = new TTree("bTag","bTag");

  // Retrieve the jet cleaning tool
  CHECK( m_jetCleaningTool.retrieve() );

  // setup branches
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
  tree->Branch("jet_truthMatch" ,&v_jet_truthMatch);
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

  // Write tree into ntuple and close file
  output->cd();
  tree->Write();
  output->Close();

  // Clean up
  CHECK( m_jetCleaningTool.release() );

  return StatusCode::SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////
StatusCode btagIBLAnalysisAlg::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");

  clearvectors();

  //---------------------------
  // Jet cleaning
  //--------------------------- 
  const xAOD::JetContainer* jets = 0;
  CHECK( evtStore()->retrieve( jets, "AntiKt4LCTopoJets") ); // what about BTagging_AntiKt4Truth?

  const xAOD::JetContainer* truthjets = 0;
  CHECK( evtStore()->retrieve( truthjets, "AntiKt4TruthJets") );

  for ( const auto* jet : *jets ) {
    if( (!m_jetCleaningTool->keep( *jet )) && (jet->pt() > 20000) ) return StatusCode::SUCCESS;
  }


  //---------------------------
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
  // find truth B hadrons in this event, store their 4-vector and index
  std::vector<std::pair<TLorentzVector, int> > truth_bhadrons;
  const xAOD::TruthEventContainer* xTruthEventContainer = NULL;
  CHECK( evtStore()->retrieve( xTruthEventContainer, "TruthEvent") );

  /* // VD: trying to get the info directly from the jet using ghost-matched b-hadrons
  for ( const auto* truth : *xTruthEventContainer ) {
    // loop over all truth particles in event
    for(unsigned int i = 0; i < truth->nTruthParticles(); i++){
      const xAOD::TruthParticle* particle = truth->truthParticle(i);
      // VD: trying to speed it up:
      if (particle->barcode()>2e5) break;
      if (particle->status()!=2) continue;

      // see if it is a B hadron
      if( isBHadron(particle->pdgId()) ) {
	// then see if it has any decay products that are also B hadrons, if not keep it
	const xAOD::TruthVertex* decayvtx = particle->decayVtx();
	for(unsigned int j = 0; j < decayvtx->nOutgoingParticles(); j++){
	  const xAOD::TruthParticle* child = decayvtx->outgoingParticle(j);
	  if( !isBHadron(child->pdgId()) )  {
	    TLorentzVector truth_bhadron;
	    truth_bhadron.SetPtEtaPhiM( particle->pt(), particle->eta(), particle->phi(), particle->m());
	    truth_bhadrons.push_back( std::make_pair(truth_bhadron,i) );
	  }
	}	
      }
    }
  }
  */


  //---------------------------
  // Jets
  //--------------------------- 
  // find reconstructed b jets and see if there is a delta R match with a truth B hadron
  std::vector<TLorentzVector> bjets;
  std::vector<TLorentzVector> bjets_matched;
  std::vector<int> matched_index;

  int numj = 0;
  int numbj= 0;
  nbjets_q=0;
  nbjets_HadI=0;
  nbjets_HadF=0;

  /// VD: first loop over the jets to find the selected one (so that I can do dR)
  std::vector<const xAOD::Jet*> selJets; selJets.reserve(10);
  for ( const auto* jet : *jets ) {
    if ( jet->pt() < 20e3 )         continue;
    if ( fabs( jet->eta() ) > 2.5)  continue;
    selJets.push_back(jet);
  }
  
  ATH_MSG_DEBUG( "Total numer of jets is: "<< selJets.size() );

  for (unsigned int j=0; j<selJets.size(); j++) {
    const xAOD::Jet* jet=selJets.at(j);

    numj++;     
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

    // matching reco jetc to truth jets
    int matched=0;
    for ( const auto* tjet : *truthjets ) {
      float dr =deltaR(jet->eta(), tjet->eta(),
		       jet->phi(), tjet->phi());
      if (dr<0.4) {
	matched=1;
	break;
      }
    }      
    v_jet_truthMatch  ->push_back(matched);
    

    //std::vector<float> testjvf = jet->auxdata<std::vector<float> >("JVF"); //todo: pick the right vertex
    
    // Get flavour truth label
    int thisJetTruthLabel;
    jet->getAttribute("TruthLabelID",thisJetTruthLabel);
    v_jet_truthflav->push_back(thisJetTruthLabel);
    if(thisJetTruthLabel == 5) numbj++;
    
    int tmpLabel=  GAFinalPartonFlavourLabel(jet);
    v_jet_GhostL_q->push_back(tmpLabel);
    if(tmpLabel == 5)  nbjets_q++;
    
    tmpLabel=  GAInitialHadronFlavourLabel(jet);
    v_jet_GhostL_HadI->push_back(tmpLabel);
    if(tmpLabel == 5)  nbjets_HadI++;
    
    tmpLabel=  GAFinalHadronFlavourLabel(jet);
    v_jet_GhostL_HadF->push_back(tmpLabel);
    if(tmpLabel == 5)  nbjets_HadF++;
    
    // b-tagging
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

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    
    // matching      
    TLorentzVector reco_bjet;
    reco_bjet.SetPtEtaPhiM(jet->pt(), jet->eta(), jet->phi(), jet->m());
    bjets.push_back( reco_bjet );
    
      /*
      float dR = 100000;
      int k = 0;
      for (unsigned int j = 0; j < truth_bhadrons.size(); j++){
	float dR_tmp = reco_bjet.DeltaR(truth_bhadrons.at(j).first);
	if(dR_tmp < 0.3){
	  if(dR > dR_tmp){
	    dR = dR_tmp;
	    k = truth_bhadrons.at(j).second;
	  }
	}
      }
      
      if(dR < 0.3 && thisJetTruthLabel == 5){
	//std::cout << "bjet matched to a B hadron with delta R = " << dR << std::endl;
	bjets_matched.push_back( reco_bjet );
	matched_index.push_back(k);
	//h_bjet_pt->Fill(reco_bjet.Pt()/1000);
	
	// now figure out transverse decay length (N.B. make some plots here for all bjets and only those truth matched)
	for ( const auto* truth : *xTruthEventContainer ) {
	  const xAOD::TruthVertex* thisvtx = (truth->truthParticle(k))->decayVtx();
	  float myLxy = sqrt( pow(thisvtx->x(),2)+ pow(thisvtx->y(),2) );
	  //std::cout << " myLxy = " << myLxy << std::endl;
	  //hist("hist_Lxy_denom")->Fill(myLxy, 1.0);
	  //hist("hist_jet_pt_denom")->Fill(jet->pt()/1000., 1.0);
	  //v_jet_Lxy_denom->push_back(myLxy);
	  //if(bjet->MV1_discriminant() > 0.983){
	  v_bH_pt->push_back(truth->truthParticle(k)->pt());
	  v_bH_eta->push_back(truth->truthParticle(k)->eta());
	  v_bH_phi->push_back(truth->truthParticle(k)->phi());
	  v_bH_Lxy->push_back(myLxy);
	    //hist("hist_Lxy")->Fill(myLxy, 1.0);
	    //hist("hist_jet_pt")->Fill(jet->pt()/1000., 1.0);
	  //}
	}
      }
      else{
	v_bH_pt->push_back(-999);
	v_bH_eta->push_back(-999);
	v_bH_phi->push_back(-999);
	v_bH_Lxy->push_back(-999);
	//std::cout << "this happened" << std::endl;
      }
      */
  } // jet loop

  njets = numj;
  nbjets = numbj;
 
  tree->Fill();

  // clear all the things that need clearing
  truth_bhadrons.clear();
  bjets.clear();
  bjets_matched.clear();
  matched_index.clear();
  
  return StatusCode::SUCCESS;
}

///////////////////////////////////////
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


void btagIBLAnalysisAlg :: clearvectors(){
  v_jet_pt->clear();
  v_jet_eta->clear();
  v_jet_phi->clear();
  v_jet_E->clear();
  v_jet_m->clear();
  v_jet_truthflav->clear();
  v_jet_GhostL_q->clear();
  v_jet_GhostL_HadI->clear();
  v_jet_GhostL_HadF->clear();;
  v_jet_truthMatch->clear();;
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
  //v_jet_Lxy_denom->clear();
}

