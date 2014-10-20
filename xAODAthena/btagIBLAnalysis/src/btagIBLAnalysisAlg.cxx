///////////////////////////////////////
// btagIBLAnalysisAlg.cxx
///////////////////////////////////////
// Author(s): marx@cern.ch
// Description: athena-based code to process xAODs 
///////////////////////////////////////

#include "btagIBLAnalysisAlg.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"
#include "xAODTruth/TruthEventContainer.h"


btagIBLAnalysisAlg::btagIBLAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator ) : AthHistogramAlgorithm( name, pSvcLocator ){

  //declareProperty( "Property", m_nProperty ); //example property declaration

}


btagIBLAnalysisAlg::~btagIBLAnalysisAlg() {}


StatusCode btagIBLAnalysisAlg::initialize() {
  ATH_MSG_INFO ("Initializing " << name() << "...");

  ATH_CHECK( book( TH1F("hist_Lxy_denom", "Lxy", 200, 0.0, 100.0) ) );
  ATH_CHECK( book( TH1F("hist_Lxy", "Lxy", 200, 0.0, 100.0) ) );

  return StatusCode::SUCCESS;
}

StatusCode btagIBLAnalysisAlg::finalize() {
  ATH_MSG_INFO ("Finalizing " << name() << "...");

  return StatusCode::SUCCESS;
}

StatusCode btagIBLAnalysisAlg::execute() {  
  ATH_MSG_DEBUG ("Executing " << name() << "...");

  //---------------------------
  // Event information
  //--------------------------- 
  /*const xAOD::EventInfo* eventInfo = 0;
  //CHECK( evtStore()->retrieve(eventInfo, "EventInfo") );  // with key name
  CHECK( evtStore()->retrieve(eventInfo) ); // without key name
  
  // check if data or MC
  if(!eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION ) ){
    ATH_MSG_DEBUG( "DATA. Will stop processing this algorithm for the current event.");
    return StatusCode::SUCCESS;
    }*/

  //  some more event-level information
  //int datasetID =  eventInfo->mcChannelNumber();
  //double eventWeight = eventInfo->mcEventWeight() ;

  //---------------------------
  // Truth stuff
  //--------------------------- 

  // find truth B hadrons in this event, store their 4-vector and index
  std::vector<std::pair<TLorentzVector, int> > truth_bhadrons;

  const xAOD::TruthEventContainer* xTruthEventContainer = NULL;
  CHECK( evtStore()->retrieve( xTruthEventContainer, "TruthEvent") );

  for ( const auto* truth : *xTruthEventContainer ) {
    // loop over all truth particles in event
    for(unsigned int i = 0; i < truth->nTruthParticles(); i++){
      const xAOD::TruthParticle* particle = truth->truthParticle(i);
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


  //---------------------------
  // Jets
  //--------------------------- 
  const xAOD::JetContainer* jets = 0;
  CHECK( evtStore()->retrieve( jets, "AntiKt4LCTopoJets") ); // what about BTagging_AntiKt4Truth?
  //std::cout << "jet size = " << jets->size() << std::endl;
  
  // find reconstructed b jets and see if there is a delta R match with a truth B hadron
  std::vector<TLorentzVector> bjets;
  std::vector<TLorentzVector> bjets_matched;
  std::vector<int> matched_index;

  for ( const auto* jet : *jets ) {
    
    if(jet->pt() > 25000 && fabs( jet->eta() ) < 2.5){

      // still need to figure out how to compute official Lxy for denominator since here haven't truth matched yet 
      //float Lxy_denom = sqrt( pow( event->jet_AntiKt4LCTopo[i].flavor_truth_vx_x(),2)+ pow( event->jet_AntiKt4LCTopo[i].flavor_truth_vx_y(),2) );
      //hist("hist_Lxy_denom")->Fill(Lxy_denom, 1.0);

      // get b-tag object
      const xAOD::BTagging* bjet = jet->btagging();
      //std::cout << "MV1 = " << bjet->MV1_discriminant() << std::endl;
      if(bjet->MV1_discriminant() > 0.983){
      
	TLorentzVector reco_bjet;
	reco_bjet.SetPtEtaPhiM(jet->pt(), jet->eta(), jet->phi(), jet->m());
	bjets.push_back( reco_bjet );
	
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
	
	if(dR < 0.3){
	  //std::cout << "bjet matched to a B hadron with delta R = " << dR << std::endl;
	  bjets_matched.push_back( reco_bjet );
	  matched_index.push_back(k);
	  //h_bjet_pt->Fill(reco_bjet.Pt()/1000);
	  
	  // now figure out transverse decay length (N.B. make some plots here for all bjets and only those truth matched)
	  for ( const auto* truth : *xTruthEventContainer ) {
	    const xAOD::TruthVertex* thisvtx = (truth->truthParticle(k))->decayVtx();
	    float myLxy = sqrt( pow(thisvtx->x(),2)+ pow(thisvtx->y(),2) );
	    //std::cout << " myLxy = " << myLxy << std::endl;
	    hist("hist_Lxy")->Fill(myLxy, 1.0);
	  }
	}
      }
    }

    
    // Get flavour truth label
    //int thisJetTruthLabel;
    //jet->getAttribute("TruthLabelID",thisJetTruthLabel);
    //ATH_MSG_DEBUG( "  truth label = " << thisJetTruthLabel );

    // Get b-tag object and print out some things
    //const xAOD::BTagging* bjet = jet->btagging();
    //std::cout << "IP3D: pb = " << bjet->IP3D_pb() << ", LL ratio = " << bjet->IP3D_loglikelihoodratio() << std::endl;
    //int ntrk;
    //std::cout << bjet->taggerInfo(ntrk,xAOD::IP3D_ntrk) << std::endl; // this currently always returns false
    //std::cout << ntrk << std::endl;

  
  } // jet loop
 

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
bool btagIBLAnalysisAlg :: isBHadron(int pdgid){

  const int allBHadrons[75]={511,513,515,521,523,525,531,533,535,541,543,545,551,553,555,
			     5101,5103,5112,5114,5122,5132,5142,5201,5203,5212,5214,5222,5224,
			     5232,5242,5301,5303,5312,5314,5322,5324,5332,5334,5342,5401,5403,
			     5412,5414,5422,5424,5432,5434,5442,5444,5503,5512,5514,5522,5524,
			     5532,5534,5542,5544,5554,10511,10513,10521,10523,10531,10533,10541,
			     10543,10551,10553,20513,20523,20533,20543,20553,100553};
  
  bool isB = false;
  for(int i = 0; i < 75; i++){
    if(fabs(pdgid) == allBHadrons[i]) isB = true;
  }
  //std::cout << "pdgid = " << pdgid << ", decision = " << isB << std::endl;
  return isB;
}

