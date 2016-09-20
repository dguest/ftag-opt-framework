#include "BHadronBranches.hh"
#include "BHadronBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include "xAODJet/Jet.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthVertex.h"

#include <string>
#include <stdexcept>

using xAOD::IParticle;

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::BHadronBranches():
  m_branches(new BHadronBranchBuffer)
{
  // instantiate all the vectors here ...
  m_branches->nBHadr = new std::vector<int>();
  m_branches->nCHadr = new std::vector<int>();

  m_branches->bH_pdgId        = new std::vector<std::vector< int> >();
  m_branches->bH_pt           = new std::vector<std::vector< float> >();
  m_branches->bH_eta          = new std::vector<std::vector< float> >();
  m_branches->bH_phi          = new std::vector<std::vector< float> >();
  m_branches->bH_E            = new std::vector<std::vector< float> >();
  m_branches->bH_Lxy          = new std::vector<std::vector< float> >();
  m_branches->bH_x            = new std::vector<std::vector< float> >();
  m_branches->bH_y            = new std::vector<std::vector< float> >();
  m_branches->bH_z            = new std::vector<std::vector< float> >();
  m_branches->bH_dRjet        = new std::vector<std::vector< float> >();
  m_branches->bH_PtTrk        = new std::vector<std::vector< float> >();
  m_branches->bH_MTrk         = new std::vector<std::vector< float> >();
  m_branches->bH_nBtracks     = new std::vector<std::vector< int> >();
  m_branches->bH_nCtracks     = new std::vector<std::vector< int> >();
  m_branches->bH_nBtracks_400 = new std::vector<std::vector< int> >();
  m_branches->bH_nCtracks_400 = new std::vector<std::vector< int> >();

  m_branches->bH_child_hadron_idx         = new std::vector<std::vector<int> > ();
  m_branches->bH_child_pdg_id             = new std::vector<std::vector<int> >();
  m_branches->bH_child_parent_pdg_id      = new std::vector<std::vector<int> >();
  m_branches->bH_child_barcode            = new std::vector<std::vector<int> >();
  m_branches->bH_child_charge             = new std::vector<std::vector<float> >();
  m_branches->bH_child_px                 = new std::vector<std::vector<float> >();
  m_branches->bH_child_py                 = new std::vector<std::vector<float> >();
  m_branches->bH_child_pz                 = new std::vector<std::vector<float> >();
  m_branches->bH_child_E                  = new std::vector<std::vector<float> >();
  m_branches->bH_child_prod_x             = new std::vector<std::vector<float> >();
  m_branches->bH_child_prod_y             = new std::vector<std::vector<float> >();
  m_branches->bH_child_prod_z             = new std::vector<std::vector<float> >();
  m_branches->bH_child_decay_x            = new std::vector<std::vector<float> >();
  m_branches->bH_child_decay_y            = new std::vector<std::vector<float> >();
  m_branches->bH_child_decay_z            = new std::vector<std::vector<float> >();

  m_branches->cH_pdgId        = new std::vector<std::vector< int> >();
  m_branches->cH_pt           = new std::vector<std::vector< float> >();
  m_branches->cH_eta          = new std::vector<std::vector< float> >();
  m_branches->cH_phi          = new std::vector<std::vector< float> >();
  m_branches->cH_E            = new std::vector<std::vector< float> >();
  m_branches->cH_Lxy          = new std::vector<std::vector< float> >();
  m_branches->cH_x            = new std::vector<std::vector< float> >();
  m_branches->cH_y            = new std::vector<std::vector< float> >();
  m_branches->cH_z            = new std::vector<std::vector< float> >();
  m_branches->cH_dRjet        = new std::vector<std::vector< float> >();
  m_branches->cH_PtTrk        = new std::vector<std::vector< float> >();
  m_branches->cH_MTrk         = new std::vector<std::vector< float> >();
  m_branches->cH_nCtracks     = new std::vector<std::vector< int> >();
  m_branches->cH_nCtracks_400 = new std::vector<std::vector< int> >();

  m_branches->cH_child_hadron_idx         = new std::vector<std::vector<int> > ();
  m_branches->cH_child_pdg_id             = new std::vector<std::vector<int> >();
  m_branches->cH_child_parent_pdg_id      = new std::vector<std::vector<int> >();
  m_branches->cH_child_barcode            = new std::vector<std::vector<int> >();
  m_branches->cH_child_charge             = new std::vector<std::vector<float> >();
  m_branches->cH_child_px                 = new std::vector<std::vector<float> >();
  m_branches->cH_child_py                 = new std::vector<std::vector<float> >();
  m_branches->cH_child_pz                 = new std::vector<std::vector<float> >();
  m_branches->cH_child_E                  = new std::vector<std::vector<float> >();
  m_branches->cH_child_prod_x             = new std::vector<std::vector<float> >();
  m_branches->cH_child_prod_y             = new std::vector<std::vector<float> >();
  m_branches->cH_child_prod_z             = new std::vector<std::vector<float> >();
  m_branches->cH_child_decay_x            = new std::vector<std::vector<float> >();
  m_branches->cH_child_decay_y            = new std::vector<std::vector<float> >();
  m_branches->cH_child_decay_z            = new std::vector<std::vector<float> >();


}

//!-----------------------------------------------------------------------------------------------------------------------------!//
BHadronBranches::~BHadronBranches() {
  // delete all the vectors here ...
  delete m_branches->nBHadr;
  delete m_branches->nCHadr;

  delete m_branches->bH_pdgId;
  delete m_branches->bH_pt;
  delete m_branches->bH_eta;
  delete m_branches->bH_phi;
  delete m_branches->bH_E;
  delete m_branches->bH_Lxy;
  delete m_branches->bH_x;
  delete m_branches->bH_y;
  delete m_branches->bH_z;
  delete m_branches->bH_dRjet;
  delete m_branches->bH_PtTrk;
  delete m_branches->bH_MTrk;
  delete m_branches->bH_nBtracks;
  delete m_branches->bH_nCtracks;
  delete m_branches->bH_nBtracks_400;
  delete m_branches->bH_nCtracks_400;
  delete m_branches->bH_child_hadron_idx;
  delete m_branches->bH_child_pdg_id;
  delete m_branches->bH_child_parent_pdg_id;
  delete m_branches->bH_child_barcode;
  delete m_branches->bH_child_charge;
  delete m_branches->bH_child_px;
  delete m_branches->bH_child_py;
  delete m_branches->bH_child_pz;
  delete m_branches->bH_child_E;
  delete m_branches->bH_child_prod_x;
  delete m_branches->bH_child_prod_y;
  delete m_branches->bH_child_prod_z;
  delete m_branches->bH_child_decay_x;
  delete m_branches->bH_child_decay_y;
  delete m_branches->bH_child_decay_z;

  delete m_branches->cH_pdgId;
  delete m_branches->cH_pt;
  delete m_branches->cH_eta;
  delete m_branches->cH_phi;
  delete m_branches->cH_E;
  delete m_branches->cH_Lxy;
  delete m_branches->cH_x;
  delete m_branches->cH_y;
  delete m_branches->cH_z;
  delete m_branches->cH_dRjet;
  delete m_branches->cH_PtTrk;
  delete m_branches->cH_MTrk;
  delete m_branches->cH_nCtracks;
  delete m_branches->cH_nCtracks_400;
  delete m_branches->cH_child_hadron_idx;
  delete m_branches->cH_child_pdg_id;
  delete m_branches->cH_child_parent_pdg_id;
  delete m_branches->cH_child_barcode;
  delete m_branches->cH_child_charge;
  delete m_branches->cH_child_px;
  delete m_branches->cH_child_py;
  delete m_branches->cH_child_pz;
  delete m_branches->cH_child_E;
  delete m_branches->cH_child_prod_x;
  delete m_branches->cH_child_prod_y;
  delete m_branches->cH_child_prod_z;
  delete m_branches->cH_child_decay_x;
  delete m_branches->cH_child_decay_y;
  delete m_branches->cH_child_decay_z;

  delete m_branches;
}

void BHadronBranches::set_tree(TTree& output_tree) const {
  //std::string prefix = "jet_bH_";
  output_tree.Branch( "jet_nBHadr"       , &m_branches->nBHadr);
  output_tree.Branch( "jet_nCHadr"       , &m_branches->nCHadr);

  output_tree.Branch( "jet_bH_pdgId"       , &m_branches->bH_pdgId);
  output_tree.Branch( "jet_bH_pt"          , &m_branches->bH_pt);
  output_tree.Branch( "jet_bH_eta"         , &m_branches->bH_eta);
  output_tree.Branch( "jet_bH_phi"         , &m_branches->bH_phi);
  output_tree.Branch( "jet_bH_E"           , &m_branches->bH_E);
  output_tree.Branch( "jet_bH_Lxy"         , &m_branches->bH_Lxy);
  output_tree.Branch( "jet_bH_x"           , &m_branches->bH_x);
  output_tree.Branch( "jet_bH_y"           , &m_branches->bH_y);
  output_tree.Branch( "jet_bH_z"           , &m_branches->bH_z);
  output_tree.Branch( "jet_bH_dRjet"       , &m_branches->bH_dRjet);
  output_tree.Branch( "jet_bH_PtTrk"       , &m_branches->bH_PtTrk);
  output_tree.Branch( "jet_bH_MTrk"        , &m_branches->bH_MTrk);
  output_tree.Branch( "jet_bH_nBtracks"    , &m_branches->bH_nBtracks);
  output_tree.Branch( "jet_bH_nCtracks"    , &m_branches->bH_nCtracks);
  output_tree.Branch( "jet_bH_nBtracks_400", &m_branches->bH_nBtracks_400);
  output_tree.Branch( "jet_bH_nCtracks_400", &m_branches->bH_nCtracks_400);

  output_tree.Branch( "jet_bH_child_hadron_idx"   , &m_branches->bH_child_hadron_idx);
  output_tree.Branch( "jet_bH_child_pdg_id"       , &m_branches->bH_child_pdg_id);
  output_tree.Branch( "jet_bH_child_parent_pdg_id", &m_branches->bH_child_parent_pdg_id);
  output_tree.Branch( "jet_bH_child_barcode"      , &m_branches->bH_child_barcode);
  output_tree.Branch( "jet_bH_child_charge"       , &m_branches->bH_child_charge);
  output_tree.Branch( "jet_bH_child_px"           , &m_branches->bH_child_px);
  output_tree.Branch( "jet_bH_child_py"           , &m_branches->bH_child_py);
  output_tree.Branch( "jet_bH_child_pz"           , &m_branches->bH_child_pz);
  output_tree.Branch( "jet_bH_child_E"            , &m_branches->bH_child_E);
  output_tree.Branch( "jet_bH_child_prod_x"       , &m_branches->bH_child_prod_x);
  output_tree.Branch( "jet_bH_child_prod_y"       , &m_branches->bH_child_prod_y);
  output_tree.Branch( "jet_bH_child_prod_z"       , &m_branches->bH_child_prod_z);
  output_tree.Branch( "jet_bH_child_decay_x"      , &m_branches->bH_child_decay_x);
  output_tree.Branch( "jet_bH_child_decay_y"      , &m_branches->bH_child_decay_y);
  output_tree.Branch( "jet_bH_child_decay_z"      , &m_branches->bH_child_decay_z);


  output_tree.Branch( "jet_cH_pdgId"       , &m_branches->cH_pdgId);
  output_tree.Branch( "jet_cH_pt"          , &m_branches->cH_pt);
  output_tree.Branch( "jet_cH_eta"         , &m_branches->cH_eta);
  output_tree.Branch( "jet_cH_phi"         , &m_branches->cH_phi);
  output_tree.Branch( "jet_cH_E"           , &m_branches->cH_E);
  output_tree.Branch( "jet_cH_Lxy"         , &m_branches->cH_Lxy);
  output_tree.Branch( "jet_cH_x"           , &m_branches->cH_x);
  output_tree.Branch( "jet_cH_y"           , &m_branches->cH_y);
  output_tree.Branch( "jet_cH_z"           , &m_branches->cH_z);
  output_tree.Branch( "jet_cH_dRjet"       , &m_branches->cH_dRjet);
  output_tree.Branch( "jet_cH_PtTrk"       , &m_branches->cH_PtTrk);
  output_tree.Branch( "jet_cH_MTrk"        , &m_branches->cH_MTrk);
  output_tree.Branch( "jet_cH_nCtracks"    , &m_branches->cH_nCtracks);
  output_tree.Branch( "jet_cH_nCtracks_400", &m_branches->cH_nCtracks_400);

  output_tree.Branch( "jet_cH_child_hadron_idx"   , &m_branches->cH_child_hadron_idx);
  output_tree.Branch( "jet_cH_child_pdg_id"       , &m_branches->cH_child_pdg_id);
  output_tree.Branch( "jet_cH_child_parent_pdg_id", &m_branches->cH_child_parent_pdg_id);
  output_tree.Branch( "jet_cH_child_barcode"      , &m_branches->cH_child_barcode);
  output_tree.Branch( "jet_cH_child_charge"       , &m_branches->cH_child_charge);
  output_tree.Branch( "jet_cH_child_px"           , &m_branches->cH_child_px);
  output_tree.Branch( "jet_cH_child_py"           , &m_branches->cH_child_py);
  output_tree.Branch( "jet_cH_child_pz"           , &m_branches->cH_child_pz);
  output_tree.Branch( "jet_cH_child_E"            , &m_branches->cH_child_E);
  output_tree.Branch( "jet_cH_child_prod_x"       , &m_branches->cH_child_prod_x);
  output_tree.Branch( "jet_cH_child_prod_y"       , &m_branches->cH_child_prod_y);
  output_tree.Branch( "jet_cH_child_prod_z"       , &m_branches->cH_child_prod_z);
  output_tree.Branch( "jet_cH_child_decay_x"      , &m_branches->cH_child_decay_x);
  output_tree.Branch( "jet_cH_child_decay_y"      , &m_branches->cH_child_decay_y);
  output_tree.Branch( "jet_cH_child_decay_z"      , &m_branches->cH_child_decay_z);

}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::fill(const xAOD::Jet& jet) {

  std::vector<const IParticle*> ghostB;
  const std::string labelB = "ConeExclBHadronsFinal";
  jet.getAssociatedObjects<IParticle>(labelB, ghostB);

  std::vector<const IParticle*> ghostC;
  const std::string labelC = "ConeExclCHadronsFinal";
  jet.getAssociatedObjects<IParticle>(labelC, ghostC);

  std::vector<int>     j_bH_pdgId;
  std::vector<float>   j_bH_pt;
  std::vector<float>   j_bH_eta;
  std::vector<float>   j_bH_phi;
  std::vector<float>   j_bH_E;
  std::vector<float>   j_bH_Lxy;
  std::vector<float>   j_bH_x;
  std::vector<float>   j_bH_y;
  std::vector<float>   j_bH_z;
  std::vector<float>   j_bH_dRjet;
  std::vector<float>   j_bH_PtTrk;
  std::vector<float>   j_bH_MTrk;
  std::vector<int>     j_bH_nBtracks;
  std::vector<int>     j_bH_nCtracks;
  std::vector<int>     j_bH_nBtracks_400;
  std::vector<int>     j_bH_nCtracks_400;
  std::vector<int>     j_bH_child_hadron_idx;
  std::vector<int>     j_bH_child_pdg_id;
  std::vector<int>     j_bH_child_parent_pdg_id;
  std::vector<int>     j_bH_child_barcode;
  std::vector<float>   j_bH_child_charge;
  std::vector<float>   j_bH_child_px;
  std::vector<float>   j_bH_child_py;
  std::vector<float>   j_bH_child_pz;
  std::vector<float>   j_bH_child_E;
  std::vector<float>   j_bH_child_prod_x;
  std::vector<float>   j_bH_child_prod_y;
  std::vector<float>   j_bH_child_prod_z;
  std::vector<float>   j_bH_child_decay_x;
  std::vector<float>   j_bH_child_decay_y;
  std::vector<float>   j_bH_child_decay_z;

  std::vector<int>     j_cH_pdgId;
  std::vector<float>   j_cH_pt;
  std::vector<float>   j_cH_eta;
  std::vector<float>   j_cH_phi;
  std::vector<float>   j_cH_E;
  std::vector<float>   j_cH_Lxy;
  std::vector<float>   j_cH_x;
  std::vector<float>   j_cH_y;
  std::vector<float>   j_cH_z;
  std::vector<float>   j_cH_dRjet;
  std::vector<float>   j_cH_PtTrk;
  std::vector<float>   j_cH_MTrk;
  std::vector<int>     j_cH_nCtracks;
  std::vector<int>     j_cH_nCtracks_400;
  std::vector<int>     j_cH_child_hadron_idx;
  std::vector<int>     j_cH_child_pdg_id;
  std::vector<int>     j_cH_child_parent_pdg_id;
  std::vector<int>     j_cH_child_barcode;
  std::vector<float>   j_cH_child_charge;
  std::vector<float>   j_cH_child_px;
  std::vector<float>   j_cH_child_py;
  std::vector<float>   j_cH_child_pz;
  std::vector<float>   j_cH_child_E;
  std::vector<float>   j_cH_child_prod_x;
  std::vector<float>   j_cH_child_prod_y;
  std::vector<float>   j_cH_child_prod_z;
  std::vector<float>   j_cH_child_decay_x;
  std::vector<float>   j_cH_child_decay_y;
  std::vector<float>   j_cH_child_decay_z;

  if ( ghostB.size()>0 ) {

    //sort hadrons by dR to jet
    std::vector<int> BhadIndices = getDRSortedIndices(ghostB,&jet);


    for ( unsigned int iB=0; iB< ghostB.size(); iB++) {

      const xAOD::TruthParticle * myB=(const xAOD::TruthParticle*)(ghostB.at(BhadIndices[iB]));

      j_bH_pdgId.push_back( myB->pdgId()  );
      j_bH_pt.push_back( myB->pt()  );
      j_bH_eta.push_back( myB->eta()  );
      j_bH_phi.push_back( myB->phi()  );
      j_bH_E.push_back( myB->e()  );
      j_bH_Lxy.push_back( sqrt( pow(myB->decayVtx()->x(), 2) + pow(myB->decayVtx()->y(), 2) )  );
      j_bH_x.push_back( myB->decayVtx()->x()  );
      j_bH_y.push_back( myB->decayVtx()->y() );
      j_bH_z.push_back( myB->decayVtx()->z()  );
      float dEta = (myB->eta()) - (jet.eta()) ;
      float dPhi = acos(cos( fabs( myB->phi()-jet.phi() ) ) );
      j_bH_dRjet.push_back( sqrt(pow(dEta, 2) + pow(dPhi, 2))  );



      //loop over decay products, save charged tracks that are not c or b hadrons:
      std::vector<const xAOD::TruthParticle*> tracksFromB;
      std::vector<const xAOD::TruthParticle*> tracksFromC;

      GetAllChildren(myB, tracksFromB, tracksFromC, false ); //tracksFromB contains also tracksFromC

      int nBtrk_400=0;
      int nCtrk_400=0;
      TLorentzVector tracks_p4(0,0,0,0);



      for(unsigned i=0; i< tracksFromB.size(); i++){
        const xAOD::TruthParticle* trk =  tracksFromB.at(i);

        TLorentzVector trkp4;
        trkp4.SetPxPyPzE(trk->px(), trk->py(), trk->pz(), trk->e());

        tracks_p4 = tracks_p4+trkp4;

        if( trk->pt() > 400 && fabs(trk->eta()) < 2.5 ){ nBtrk_400++; }

        j_bH_child_hadron_idx.push_back(iB);
        j_bH_child_pdg_id.push_back( trk->pdgId() );
        j_bH_child_parent_pdg_id.push_back( trk->parent(0)->pdgId()  );
        j_bH_child_barcode.push_back( trk->barcode()  );
        j_bH_child_charge.push_back( trk->charge()  );
        j_bH_child_px.push_back( trk->px()  );
        j_bH_child_py.push_back( trk->py()  );
        j_bH_child_pz.push_back( trk->pz()  );
        j_bH_child_E.push_back( trk->e()  );
        j_bH_child_prod_x.push_back( trk->prodVtx()->x()  );
        j_bH_child_prod_y.push_back( trk->prodVtx()->y()  );
        j_bH_child_prod_z.push_back( trk->prodVtx()->z()  );

        if(trk->hasDecayVtx() ){
          j_bH_child_decay_x.push_back( trk->decayVtx()->x()  );
          j_bH_child_decay_y.push_back( trk->decayVtx()->y()  );
          j_bH_child_decay_z.push_back( trk->decayVtx()->z()  );
        }else{
          j_bH_child_decay_x.push_back( -999  );
          j_bH_child_decay_y.push_back( -999  );
          j_bH_child_decay_z.push_back( -999  );
        }
      }

      for(unsigned i=0; i< tracksFromC.size(); i++){
        const xAOD::TruthParticle* trk =  tracksFromC.at(i);

        TLorentzVector trkp4;
        trkp4.SetPxPyPzE(trk->px(), trk->py(), trk->pz(), trk->e());
        if( trkp4.Pt() > 400 && fabs(trkp4.Eta()) < 2.5  ){ nCtrk_400++; }
      }


      j_bH_nBtracks.push_back(tracksFromB.size()-tracksFromC.size());
      j_bH_nCtracks.push_back(tracksFromC.size());
      j_bH_nBtracks_400.push_back(nBtrk_400-nCtrk_400);
      j_bH_nCtracks_400.push_back(nCtrk_400);

      j_bH_PtTrk.push_back(tracks_p4.Pt());
      j_bH_MTrk.push_back(tracks_p4.M());


    }
  } // ghost B size >0
  else{
    j_bH_pdgId.push_back(-99);
    j_bH_pt.push_back(-99);
    j_bH_eta.push_back(-99);
    j_bH_phi.push_back(-99);
    j_bH_E.push_back(-99);
    j_bH_Lxy.push_back(-99);
    j_bH_x.push_back(-99);
    j_bH_y.push_back(-99);
    j_bH_z.push_back(-99);
    j_bH_dRjet.push_back(-99);
    j_bH_PtTrk.push_back(-99);
    j_bH_MTrk.push_back(-99);
    j_bH_nBtracks.push_back(-99);
    j_bH_nCtracks.push_back(-99);
    j_bH_nBtracks_400.push_back(-99);
    j_bH_nCtracks_400.push_back(-99);
    j_bH_child_hadron_idx.push_back(-99);
    j_bH_child_pdg_id.push_back(-99);
    j_bH_child_parent_pdg_id.push_back(-99);
    j_bH_child_barcode.push_back(-99);
    j_bH_child_charge.push_back(-99);
    j_bH_child_px.push_back(-99);
    j_bH_child_py.push_back(-99);
    j_bH_child_pz.push_back(-99);
    j_bH_child_E.push_back(-99);
    j_bH_child_prod_x.push_back(-99);
    j_bH_child_prod_y.push_back(-99);
    j_bH_child_prod_z.push_back(-99);
    j_bH_child_decay_x.push_back(-99);
    j_bH_child_decay_y.push_back(-99);
    j_bH_child_decay_z.push_back(-99);
  }

  if ( ghostC.size()>0 ) {

    std::vector<int> ChadIndices = getDRSortedIndices(ghostC,&jet);

    for ( unsigned int iC=0; iC< ghostC.size(); iC++) {

      const xAOD::TruthParticle * myC=(const xAOD::TruthParticle*)(ghostC.at(ChadIndices[iC]));

      j_cH_pdgId.push_back( myC->pdgId()  );
      j_cH_pt.push_back( myC->pt()  );
      j_cH_eta.push_back( myC->eta()  );
      j_cH_phi.push_back( myC->phi()  );
      j_cH_E.push_back( myC->e()  );
      j_cH_Lxy.push_back( sqrt( pow(myC->decayVtx()->x(), 2) + pow(myC->decayVtx()->y(), 2) )  );
      j_cH_x.push_back( myC->decayVtx()->x()  );
      j_cH_y.push_back( myC->decayVtx()->y() );
      j_cH_z.push_back( myC->decayVtx()->z()  );
      float dEta = (myC->eta()) - (jet.eta()) ;
      float dPhi = acos(cos( fabs( myC->phi()-jet.phi() ) ) );
      j_cH_dRjet.push_back( sqrt(pow(dEta, 2) + pow(dPhi, 2))  );



      //loop over decay products, save charged tracks that are not c hadrons:
      std::vector<const xAOD::TruthParticle*> tracksFromC;

      GetAllChildren(myC, tracksFromC, tracksFromC, false ); //tracksFromB contains also tracksFromC

      int nCtrk_400=0;
      TLorentzVector tracks_p4(0,0,0,0);

      for(unsigned i=0; i< tracksFromC.size(); i++){
        const xAOD::TruthParticle* trk =  tracksFromC.at(i);

        TLorentzVector trkp4;
        trkp4.SetPxPyPzE(trk->px(), trk->py(), trk->pz(), trk->e());

        tracks_p4 = tracks_p4+trkp4;

        if( trkp4.Pt() > 400 && fabs(trkp4.Eta()) < 2.5 ){ nCtrk_400++; }

        j_cH_child_hadron_idx.push_back(iC);
        j_cH_child_pdg_id.push_back( trk->pdgId() );
        j_cH_child_parent_pdg_id.push_back( trk->parent(0)->pdgId()  );
        j_cH_child_barcode.push_back( trk->barcode()  );
        j_cH_child_charge.push_back( trk->charge()  );
        j_cH_child_px.push_back( trk->px()  );
        j_cH_child_py.push_back( trk->py()  );
        j_cH_child_pz.push_back( trk->pz()  );
        j_cH_child_E.push_back( trk->e()  );
        j_cH_child_prod_x.push_back( trk->prodVtx()->x()  );
        j_cH_child_prod_y.push_back( trk->prodVtx()->y()  );
        j_cH_child_prod_z.push_back( trk->prodVtx()->z()  );

        if(trk->hasDecayVtx() ){
          j_cH_child_decay_x.push_back( trk->decayVtx()->x()  );
          j_cH_child_decay_y.push_back( trk->decayVtx()->y()  );
          j_cH_child_decay_z.push_back( trk->decayVtx()->z()  );
        }else{
          j_cH_child_decay_x.push_back( -999  );
          j_cH_child_decay_y.push_back( -999  );
          j_cH_child_decay_z.push_back( -999  );
        }
      }


      j_cH_nCtracks.push_back(tracksFromC.size());
      j_cH_nCtracks_400.push_back(nCtrk_400);

      j_cH_PtTrk.push_back(tracks_p4.Pt());
      j_cH_MTrk.push_back(tracks_p4.M());
    }

  } // ghost C size >0
  else{
    j_cH_pdgId.push_back(-99);
    j_cH_pt.push_back(-99);
    j_cH_eta.push_back(-99);
    j_cH_phi.push_back(-99);
    j_cH_E.push_back(-99);
    j_cH_Lxy.push_back(-99);
    j_cH_x.push_back(-99);
    j_cH_y.push_back(-99);
    j_cH_z.push_back(-99);
    j_cH_dRjet.push_back(-99);
    j_cH_PtTrk.push_back(-99);
    j_cH_MTrk.push_back(-99);
    j_cH_nCtracks.push_back(-99);
    j_cH_nCtracks_400.push_back(-99);
    j_cH_child_hadron_idx.push_back(-99);
    j_cH_child_pdg_id.push_back(-99);
    j_cH_child_parent_pdg_id.push_back(-99);
    j_cH_child_barcode.push_back(-99);
    j_cH_child_charge.push_back(-99);
    j_cH_child_px.push_back(-99);
    j_cH_child_py.push_back(-99);
    j_cH_child_pz.push_back(-99);
    j_cH_child_E.push_back(-99);
    j_cH_child_prod_x.push_back(-99);
    j_cH_child_prod_y.push_back(-99);
    j_cH_child_prod_z.push_back(-99);
    j_cH_child_decay_x.push_back(-99);
    j_cH_child_decay_y.push_back(-99);
    j_cH_child_decay_z.push_back(-99);
  }




  m_branches->nBHadr->push_back(ghostB.size());
  m_branches->nCHadr->push_back(ghostC.size());

  m_branches->bH_pt->push_back(j_bH_pt);
  m_branches->bH_pdgId->push_back(j_bH_pdgId);
  m_branches->bH_pt->push_back(j_bH_pt);
  m_branches->bH_eta->push_back(j_bH_eta);
  m_branches->bH_phi->push_back(j_bH_phi);
  m_branches->bH_E->push_back(j_bH_E);
  m_branches->bH_Lxy->push_back(j_bH_Lxy);
  m_branches->bH_x->push_back(j_bH_x);
  m_branches->bH_y->push_back(j_bH_y);
  m_branches->bH_z->push_back(j_bH_z);
  m_branches->bH_dRjet->push_back(j_bH_dRjet);
  m_branches->bH_PtTrk->push_back(j_bH_PtTrk);
  m_branches->bH_MTrk->push_back(j_bH_MTrk);
  m_branches->bH_nBtracks->push_back(j_bH_nBtracks);
  m_branches->bH_nCtracks->push_back(j_bH_nCtracks);
  m_branches->bH_nBtracks_400->push_back(j_bH_nBtracks_400);
  m_branches->bH_nCtracks_400->push_back(j_bH_nCtracks_400);

  m_branches->bH_child_hadron_idx->push_back(j_bH_child_hadron_idx);
  m_branches->bH_child_pdg_id->push_back(j_bH_child_pdg_id);
  m_branches->bH_child_parent_pdg_id->push_back(j_bH_child_parent_pdg_id);
  m_branches->bH_child_barcode->push_back(j_bH_child_barcode);
  m_branches->bH_child_charge->push_back(j_bH_child_charge);
  m_branches->bH_child_px->push_back(j_bH_child_px);
  m_branches->bH_child_py->push_back(j_bH_child_py);
  m_branches->bH_child_pz->push_back(j_bH_child_pz);
  m_branches->bH_child_E->push_back(j_bH_child_E);
  m_branches->bH_child_prod_x->push_back(j_bH_child_prod_x);
  m_branches->bH_child_prod_y->push_back(j_bH_child_prod_y);
  m_branches->bH_child_prod_z->push_back(j_bH_child_prod_z);
  m_branches->bH_child_decay_x->push_back(j_bH_child_decay_x);
  m_branches->bH_child_decay_y->push_back(j_bH_child_decay_y);
  m_branches->bH_child_decay_z->push_back(j_bH_child_decay_z);


  m_branches->cH_pt->push_back(j_cH_pt);
  m_branches->cH_pdgId->push_back(j_cH_pdgId);
  m_branches->cH_pt->push_back(j_cH_pt);
  m_branches->cH_eta->push_back(j_cH_eta);
  m_branches->cH_phi->push_back(j_cH_phi);
  m_branches->cH_E->push_back(j_cH_E);
  m_branches->cH_Lxy->push_back(j_cH_Lxy);
  m_branches->cH_x->push_back(j_cH_x);
  m_branches->cH_y->push_back(j_cH_y);
  m_branches->cH_z->push_back(j_cH_z);
  m_branches->cH_dRjet->push_back(j_cH_dRjet);
  m_branches->cH_PtTrk->push_back(j_cH_PtTrk);
  m_branches->cH_MTrk->push_back(j_cH_MTrk);
  m_branches->cH_nCtracks->push_back(j_cH_nCtracks);
  m_branches->cH_nCtracks_400->push_back(j_cH_nCtracks_400);

  m_branches->cH_child_hadron_idx->push_back(j_cH_child_hadron_idx);
  m_branches->cH_child_pdg_id->push_back(j_cH_child_pdg_id);
  m_branches->cH_child_parent_pdg_id->push_back(j_cH_child_parent_pdg_id);
  m_branches->cH_child_barcode->push_back(j_cH_child_barcode);
  m_branches->cH_child_charge->push_back(j_cH_child_charge);
  m_branches->cH_child_px->push_back(j_cH_child_px);
  m_branches->cH_child_py->push_back(j_cH_child_py);
  m_branches->cH_child_pz->push_back(j_cH_child_pz);
  m_branches->cH_child_E->push_back(j_cH_child_E);
  m_branches->cH_child_prod_x->push_back(j_cH_child_prod_x);
  m_branches->cH_child_prod_y->push_back(j_cH_child_prod_y);
  m_branches->cH_child_prod_z->push_back(j_cH_child_prod_z);
  m_branches->cH_child_decay_x->push_back(j_cH_child_decay_x);
  m_branches->cH_child_decay_y->push_back(j_cH_child_decay_y);
  m_branches->cH_child_decay_z->push_back(j_cH_child_decay_z);

}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void BHadronBranches::clear() {
  // clear vectors
  m_branches->nBHadr->clear();
  m_branches->nCHadr->clear();
  m_branches->bH_pdgId->clear();
  m_branches->bH_pt->clear();
  m_branches->bH_eta->clear();
  m_branches->bH_phi->clear();
  m_branches->bH_E->clear();
  m_branches->bH_Lxy->clear();
  m_branches->bH_x->clear();
  m_branches->bH_y->clear();
  m_branches->bH_z->clear();
  m_branches->bH_dRjet->clear();
  m_branches->bH_PtTrk->clear();
  m_branches->bH_MTrk->clear();
  m_branches->bH_nBtracks->clear();
  m_branches->bH_nCtracks->clear();
  m_branches->bH_nBtracks_400->clear();
  m_branches->bH_nCtracks_400->clear();
  m_branches->bH_child_hadron_idx->clear();
  m_branches->bH_child_pdg_id->clear();
  m_branches->bH_child_parent_pdg_id->clear();
  m_branches->bH_child_barcode->clear();
  m_branches->bH_child_charge->clear();
  m_branches->bH_child_px->clear();
  m_branches->bH_child_py->clear();
  m_branches->bH_child_pz->clear();
  m_branches->bH_child_E->clear();
  m_branches->bH_child_prod_x->clear();
  m_branches->bH_child_prod_y->clear();
  m_branches->bH_child_prod_z->clear();
  m_branches->bH_child_decay_x->clear();
  m_branches->bH_child_decay_y->clear();
  m_branches->bH_child_decay_z->clear();

  m_branches->cH_pdgId->clear();
  m_branches->cH_pt->clear();
  m_branches->cH_eta->clear();
  m_branches->cH_phi->clear();
  m_branches->cH_E->clear();
  m_branches->cH_Lxy->clear();
  m_branches->cH_x->clear();
  m_branches->cH_y->clear();
  m_branches->cH_z->clear();
  m_branches->cH_dRjet->clear();
  m_branches->cH_PtTrk->clear();
  m_branches->cH_MTrk->clear();
  m_branches->cH_nCtracks->clear();
  m_branches->cH_nCtracks_400->clear();
  m_branches->cH_child_hadron_idx->clear();
  m_branches->cH_child_pdg_id->clear();
  m_branches->cH_child_parent_pdg_id->clear();
  m_branches->cH_child_barcode->clear();
  m_branches->cH_child_charge->clear();
  m_branches->cH_child_px->clear();
  m_branches->cH_child_py->clear();
  m_branches->cH_child_pz->clear();
  m_branches->cH_child_E->clear();
  m_branches->cH_child_prod_x->clear();
  m_branches->cH_child_prod_y->clear();
  m_branches->cH_child_prod_z->clear();
  m_branches->cH_child_decay_x->clear();
  m_branches->cH_child_decay_y->clear();
  m_branches->cH_child_decay_z->clear();

}

void BHadronBranches :: GetAllChildren(const xAOD::TruthParticle* particle,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromB,
                                           std::vector<const xAOD::TruthParticle*> &tracksFromC,
                                           bool isFromC){


  if(!particle->hasDecayVtx()) return;

  const xAOD::TruthVertex* decayvtx = particle->decayVtx();

  for(unsigned i=0; i< decayvtx->nOutgoingParticles(); i++){

     const xAOD::TruthParticle* child = decayvtx->outgoingParticle(i);

     if (child->barcode() > 200e3) continue;
     if ( !child->isCharmHadron() && !child->isBottomHadron() ){

        if(child->isCharged()){ tracksFromB.push_back(child);}
        if(child->isCharged() && isFromC){
          tracksFromC.push_back(child);
        }
     }

     if (isFromC) GetAllChildren(child, tracksFromB, tracksFromC, true);
     else GetAllChildren(child, tracksFromB, tracksFromC, child->isCharmHadron() );


  }

}


std::vector<int> BHadronBranches :: getDRSortedIndices(std::vector<const xAOD::IParticle*> ghostHads, const xAOD::Jet *jet){
    std::vector<float> dRofhadrons;

    for(unsigned int ip = 0; ip < ghostHads.size(); ip++){
      float dEta = (ghostHads.at(ip))->eta() - (jet->eta()) ;
      float dPhi = acos(cos( fabs( (ghostHads.at(ip))->phi()-jet->phi() ) ) );
      float dr = sqrt(pow(dEta, 2) + pow(dPhi, 2));
      dRofhadrons.push_back(dr);
    }

    std::vector<int> y(dRofhadrons.size());
    std::size_t n(0);
    std::generate(std::begin(y), std::end(y), [&]{ return n++; });
    std::sort(std::begin(y),std::end(y),[&](int i1, int i2) { return dRofhadrons[i1] < dRofhadrons[i2]; });

    return y;
}
