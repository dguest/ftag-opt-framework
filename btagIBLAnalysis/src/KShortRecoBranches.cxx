#include "KShortRecoBranches.hh"
#include "KShortRecoBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include "xAODJet/Jet.h"

#include "TVector3.h"
#include "TF1.h"

#include <string>
#include <stdexcept>

using xAOD::IParticle;

//!-----------------------------------------------------------------------------------------------------------------------------!//
KShortRecoBranches::KShortRecoBranches():
  m_branches(new KShortRecoBranchBuffer)
{
  // instantiate all the vectors here ...
  m_branches->nKShortReco              = new std::vector<int>();
  m_branches->kShortReco_ndof          = new std::vector<std::vector< int> >();
  m_branches->kShortReco_px            = new std::vector<std::vector< float> >();
  m_branches->kShortReco_py            = new std::vector<std::vector< float> >();
  m_branches->kShortReco_pz            = new std::vector<std::vector< float> >();
  m_branches->kShortReco_pt            = new std::vector<std::vector< float> >();
  m_branches->kShortReco_ptErr         = new std::vector<std::vector< float> >();  
  m_branches->kShortReco_x             = new std::vector<std::vector< float> >();
  m_branches->kShortReco_y             = new std::vector<std::vector< float> >();
  m_branches->kShortReco_z             = new std::vector<std::vector< float> >();
  m_branches->kShortReco_Rxy           = new std::vector<std::vector< float> >();
  m_branches->kShortReco_RxyErr        = new std::vector<std::vector< float> >();
  m_branches->kShortReco_chi2          = new std::vector<std::vector< float> >();
  m_branches->kShortReco_kShortMass    = new std::vector<std::vector< float> >();
  m_branches->kShortReco_kShortMassErr = new std::vector<std::vector< float> >();
  m_branches->kShortReco_d0            = new std::vector<std::vector< float> >();  
  m_branches->kShortReco_z0            = new std::vector<std::vector< float> >();  

  m_branches->nKShortRecoGood              = new std::vector<int>();
  m_branches->kShortRecoGood_ndof          = new std::vector<std::vector< int> >();
  m_branches->kShortRecoGood_px            = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_py            = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_pz            = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_pt            = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_ptErr         = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_x             = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_y             = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_z             = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_Rxy           = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_RxyErr        = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_chi2          = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_kShortMass    = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_kShortMassErr = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_d0            = new std::vector<std::vector< float> >();
  m_branches->kShortRecoGood_z0            = new std::vector<std::vector< float> >();

}

//!-----------------------------------------------------------------------------------------------------------------------------!//
KShortRecoBranches::~KShortRecoBranches() {
  // delete all the vectors here ...
  delete m_branches->nKShortReco;
  delete m_branches->kShortReco_ndof;
  delete m_branches->kShortReco_px;
  delete m_branches->kShortReco_py;
  delete m_branches->kShortReco_pz;
  delete m_branches->kShortReco_pt;
  delete m_branches->kShortReco_ptErr;
  delete m_branches->kShortReco_x;
  delete m_branches->kShortReco_y;
  delete m_branches->kShortReco_z;
  delete m_branches->kShortReco_Rxy;
  delete m_branches->kShortReco_RxyErr;
  delete m_branches->kShortReco_chi2;
  delete m_branches->kShortReco_kShortMass;
  delete m_branches->kShortReco_kShortMassErr;
  delete m_branches->kShortReco_d0;
  delete m_branches->kShortReco_z0;
  
  delete m_branches->nKShortRecoGood;
  delete m_branches->kShortRecoGood_ndof;
  delete m_branches->kShortRecoGood_px;
  delete m_branches->kShortRecoGood_py;
  delete m_branches->kShortRecoGood_pz;
  delete m_branches->kShortRecoGood_pt;
  delete m_branches->kShortRecoGood_ptErr;
  delete m_branches->kShortRecoGood_x;
  delete m_branches->kShortRecoGood_y;
  delete m_branches->kShortRecoGood_z;
  delete m_branches->kShortRecoGood_Rxy;
  delete m_branches->kShortRecoGood_RxyErr;
  delete m_branches->kShortRecoGood_chi2;
  delete m_branches->kShortRecoGood_kShortMass;
  delete m_branches->kShortRecoGood_kShortMassErr;
  delete m_branches->kShortRecoGood_d0;
  delete m_branches->kShortRecoGood_z0;
  
  

  delete m_branches;
}

void KShortRecoBranches::set_tree(TTree& output_tree) const {
  output_tree.Branch( "jet_nKShortReco"              , &m_branches->nKShortReco);
  output_tree.Branch( "jet_kShortReco_ndof"          , &m_branches->kShortReco_ndof);
  output_tree.Branch( "jet_kShortReco_px"            , &m_branches->kShortReco_px);
  output_tree.Branch( "jet_kShortReco_py"            , &m_branches->kShortReco_py);
  output_tree.Branch( "jet_kShortReco_pz"            , &m_branches->kShortReco_pz);
  output_tree.Branch( "jet_kShortReco_pt"            , &m_branches->kShortReco_pt);
  output_tree.Branch( "jet_kShortReco_ptErr"         , &m_branches->kShortReco_ptErr);
  output_tree.Branch( "jet_kShortReco_x"             , &m_branches->kShortReco_x);
  output_tree.Branch( "jet_kShortReco_y"             , &m_branches->kShortReco_y);
  output_tree.Branch( "jet_kShortReco_z"             , &m_branches->kShortReco_z);
  output_tree.Branch( "jet_kShortReco_Rxy"           , &m_branches->kShortReco_Rxy);
  output_tree.Branch( "jet_kShortReco_RxyErr"        , &m_branches->kShortReco_RxyErr);
  output_tree.Branch( "jet_kShortReco_chi2"          , &m_branches->kShortReco_chi2);
  output_tree.Branch( "jet_kShortReco_kShortMass"    , &m_branches->kShortReco_kShortMass);
  output_tree.Branch( "jet_kShortReco_kShortMassErr" , &m_branches->kShortReco_kShortMassErr);
  output_tree.Branch( "jet_kShortReco_d0"            , &m_branches->kShortReco_d0);
  output_tree.Branch( "jet_kShortReco_z0"            , &m_branches->kShortReco_z0);

  output_tree.Branch( "jet_nKShortRecoGood"              , &m_branches->nKShortRecoGood);
  output_tree.Branch( "jet_kShortRecoGood_ndof"          , &m_branches->kShortRecoGood_ndof);
  output_tree.Branch( "jet_kShortRecoGood_px"            , &m_branches->kShortRecoGood_px);
  output_tree.Branch( "jet_kShortRecoGood_py"            , &m_branches->kShortRecoGood_py);
  output_tree.Branch( "jet_kShortRecoGood_pz"            , &m_branches->kShortRecoGood_pz);
  output_tree.Branch( "jet_kShortRecoGood_pt"            , &m_branches->kShortRecoGood_pt);
  output_tree.Branch( "jet_kShortRecoGood_ptErr"         , &m_branches->kShortRecoGood_ptErr);
  output_tree.Branch( "jet_kShortRecoGood_x"             , &m_branches->kShortRecoGood_x);
  output_tree.Branch( "jet_kShortRecoGood_y"             , &m_branches->kShortRecoGood_y);
  output_tree.Branch( "jet_kShortRecoGood_z"             , &m_branches->kShortRecoGood_z);
  output_tree.Branch( "jet_kShortRecoGood_Rxy"           , &m_branches->kShortRecoGood_Rxy);
  output_tree.Branch( "jet_kShortRecoGood_RxyErr"        , &m_branches->kShortRecoGood_RxyErr);
  output_tree.Branch( "jet_kShortRecoGood_chi2"          , &m_branches->kShortRecoGood_chi2);
  output_tree.Branch( "jet_kShortRecoGood_kShortMass"    , &m_branches->kShortRecoGood_kShortMass);
  output_tree.Branch( "jet_kShortRecoGood_kShortMassErr" , &m_branches->kShortRecoGood_kShortMassErr);
  output_tree.Branch( "jet_kShortRecoGood_d0"            , &m_branches->kShortRecoGood_d0);
  output_tree.Branch( "jet_kShortRecoGood_z0"            , &m_branches->kShortRecoGood_z0);

}


//!-----------------------------------------------------------------------------------------------------------------------------!//
void KShortRecoBranches::fill(const xAOD::Jet& jet,const xAOD::VertexContainer& v0s,double PV_x, double PV_y, double PV_z) {

  std::vector<int>     j_kSReco_ndof;
  std::vector<float>   j_kSReco_px;
  std::vector<float>   j_kSReco_py;
  std::vector<float>   j_kSReco_pz;
  std::vector<float>   j_kSReco_pt;
  std::vector<float>   j_kSReco_ptErr;
  std::vector<float>   j_kSReco_x;
  std::vector<float>   j_kSReco_y;
  std::vector<float>   j_kSReco_z;
  std::vector<float>   j_kSReco_Rxy;
  std::vector<float>   j_kSReco_RxyErr;
  std::vector<float>   j_kSReco_chi2;
  std::vector<float>   j_kSReco_kShortMass;
  std::vector<float>   j_kSReco_kShortMassErr;
  std::vector<float>   j_kSReco_d0;
  std::vector<float>   j_kSReco_z0;

  std::vector<int>     j_kSRecoGood_ndof;
  std::vector<float>   j_kSRecoGood_px;
  std::vector<float>   j_kSRecoGood_py;
  std::vector<float>   j_kSRecoGood_pz;
  std::vector<float>   j_kSRecoGood_pt;
  std::vector<float>   j_kSRecoGood_ptErr;
  std::vector<float>   j_kSRecoGood_x;
  std::vector<float>   j_kSRecoGood_y;
  std::vector<float>   j_kSRecoGood_z;
  std::vector<float>   j_kSRecoGood_Rxy;
  std::vector<float>   j_kSRecoGood_RxyErr;
  std::vector<float>   j_kSRecoGood_chi2;
  std::vector<float>   j_kSRecoGood_kShortMass;
  std::vector<float>   j_kSRecoGood_kShortMassErr;
  std::vector<float>   j_kSRecoGood_d0;
  std::vector<float>   j_kSRecoGood_z0;

  TLorentzVector jet_p4;
  jet_p4.SetPtEtaPhiM(jet.pt(), jet.eta(), jet.phi(), jet.m());

  xAOD::VertexContainer::const_iterator v0_itr = v0s.begin();
  xAOD::VertexContainer::const_iterator v0_end = v0s.end();
  for( ; v0_itr != v0_end; ++v0_itr ) { // loop over V0 candidates
    const xAOD::Vertex* candVtx = (*v0_itr);
    TLorentzVector candVtx_p4;
    candVtx_p4.SetPxPyPzE( candVtx->auxdataConst<float>("px"), candVtx->auxdataConst<float>("py"), candVtx->auxdataConst<float>("pz"), 999. );
    if( jet_p4.DeltaR( candVtx_p4 ) <= 0.4 ){ // check if V0 is inside the jet
      j_kSReco_ndof.push_back( candVtx->numberDoF() );
      j_kSReco_px.push_back( candVtx->auxdataConst<float>("px") );
      j_kSReco_py.push_back( candVtx->auxdataConst<float>("py") );
      j_kSReco_pz.push_back( candVtx->auxdataConst<float>("pz") );
      j_kSReco_pt.push_back( candVtx->auxdataConst<float>("pT") );
      j_kSReco_ptErr.push_back( candVtx->auxdataConst<float>("pTError") );
      j_kSReco_x.push_back( candVtx->x() );
      j_kSReco_y.push_back( candVtx->y() );
      j_kSReco_z.push_back( candVtx->z() );
      j_kSReco_Rxy.push_back( candVtx->auxdataConst<float>("Rxy") );
      j_kSReco_RxyErr.push_back( candVtx->auxdataConst<float>("RxyError") );
      j_kSReco_chi2.push_back( candVtx->chiSquared() );
      j_kSReco_kShortMass.push_back( candVtx->auxdataConst<float>("Kshort_mass") );
      j_kSReco_kShortMassErr.push_back( candVtx->auxdataConst<float>("Kshort_massError") );


      double px = candVtx->auxdataConst<float>("px");
      double py = candVtx->auxdataConst<float>("py");
      double pz = candVtx->auxdataConst<float>("pz");
      double x = candVtx->x();
      double y = candVtx->y();
      double z = candVtx->z();
      std::vector<float> IPs = getIPs(px, py, pz, x, y, z, PV_x, PV_y, PV_z);
      float d0 = IPs.at(0);
      float z0 = IPs.at(1);

      j_kSReco_d0.push_back( d0 );
      j_kSReco_z0.push_back( z0 );

      // Get Good KShorts
      if( fabs(candVtx->auxdataConst<float>("Kshort_mass") - 497.6) < 20.  &&
          fabs(candVtx->auxdataConst<float>("Rxy") - 24.)           > 2.   &&
          fabs(candVtx->auxdataConst<float>("Rxy") - 33.)           > 2.   &&
	  fabs(candVtx->auxdataConst<float>("Rxy") - 51.)           > 2.   &&
	  fabs(candVtx->auxdataConst<float>("Rxy") - 89.)           > 2.   &&
	  fabs(candVtx->auxdataConst<float>("Rxy") - 123.)          > 2.   &&
          candVtx->chiSquared()                                     < 3.   &&
	  fabs(z0)                                                  < 2.   &&
          d0                                                        < 1.25){

	j_kSRecoGood_ndof.push_back( candVtx->numberDoF() );
	j_kSRecoGood_px.push_back( candVtx->auxdataConst<float>("px") );
	j_kSRecoGood_py.push_back( candVtx->auxdataConst<float>("py") );
	j_kSRecoGood_pz.push_back( candVtx->auxdataConst<float>("pz") );
	j_kSRecoGood_pt.push_back( candVtx->auxdataConst<float>("pT") );
	j_kSRecoGood_ptErr.push_back( candVtx->auxdataConst<float>("pTError") );
	j_kSRecoGood_x.push_back( candVtx->x() );
	j_kSRecoGood_y.push_back( candVtx->y() );
	j_kSRecoGood_z.push_back( candVtx->z() );
	j_kSRecoGood_Rxy.push_back( candVtx->auxdataConst<float>("Rxy") );
	j_kSRecoGood_RxyErr.push_back( candVtx->auxdataConst<float>("RxyError") );
	j_kSRecoGood_chi2.push_back( candVtx->chiSquared() );
	j_kSRecoGood_kShortMass.push_back( candVtx->auxdataConst<float>("Kshort_mass") );
	j_kSRecoGood_kShortMassErr.push_back( candVtx->auxdataConst<float>("Kshort_massError") );


	double px = candVtx->auxdataConst<float>("px");
	double py = candVtx->auxdataConst<float>("py");
	double pz = candVtx->auxdataConst<float>("pz");
	double x = candVtx->x();
	double y = candVtx->y();
	double z = candVtx->z();
	std::vector<float> IPs = getIPs(px, py, pz, x, y, z, PV_x, PV_y, PV_z);
	float d0 = IPs.at(0);
	float z0 = IPs.at(1);

	j_kSRecoGood_d0.push_back( d0 );
	j_kSRecoGood_z0.push_back( z0 );

      }


    }

  } // end loop over V0 candidated

  m_branches->nKShortReco->push_back(j_kSReco_px.size());
  m_branches->nKShortRecoGood->push_back(j_kSRecoGood_px.size());

  if( j_kSReco_px.size() == 0 ){
    j_kSReco_ndof.push_back( -99 );
    j_kSReco_px.push_back( -99 );
    j_kSReco_py.push_back( -99 );
    j_kSReco_pz.push_back( -99 );
    j_kSReco_pt.push_back( -99 );
    j_kSReco_ptErr.push_back( -99 );
    j_kSReco_x.push_back( -99 );
    j_kSReco_y.push_back( -99 );
    j_kSReco_z.push_back( -99 );
    j_kSReco_Rxy.push_back( -99 );
    j_kSReco_RxyErr.push_back( -99 );
    j_kSReco_chi2.push_back( -99 );
    j_kSReco_kShortMass.push_back( -99 );
    j_kSReco_kShortMassErr.push_back( -99 );
    j_kSReco_d0.push_back( -99 );
    j_kSReco_z0.push_back( -99 );
  }

  if( j_kSRecoGood_px.size() == 0 ){
    j_kSRecoGood_ndof.push_back( -99 );
    j_kSRecoGood_px.push_back( -99 );
    j_kSRecoGood_py.push_back( -99 );
    j_kSRecoGood_pz.push_back( -99 );
    j_kSRecoGood_pt.push_back( -99 );
    j_kSRecoGood_ptErr.push_back( -99 );
    j_kSRecoGood_x.push_back( -99 );
    j_kSRecoGood_y.push_back( -99 );
    j_kSRecoGood_z.push_back( -99 );
    j_kSRecoGood_Rxy.push_back( -99 );
    j_kSRecoGood_RxyErr.push_back( -99 );
    j_kSRecoGood_chi2.push_back( -99 );
    j_kSRecoGood_kShortMass.push_back( -99 );
    j_kSRecoGood_kShortMassErr.push_back( -99 );
    j_kSRecoGood_d0.push_back( -99 );
    j_kSRecoGood_z0.push_back( -99 );
  }


  m_branches->kShortReco_ndof->push_back(j_kSReco_ndof);
  m_branches->kShortReco_px->push_back(j_kSReco_px);
  m_branches->kShortReco_py->push_back(j_kSReco_py);
  m_branches->kShortReco_pz->push_back(j_kSReco_pz);
  m_branches->kShortReco_pt->push_back(j_kSReco_pt);
  m_branches->kShortReco_ptErr->push_back(j_kSReco_ptErr);  
  m_branches->kShortReco_x->push_back(j_kSReco_x);
  m_branches->kShortReco_y->push_back(j_kSReco_y);
  m_branches->kShortReco_z->push_back(j_kSReco_z);
  m_branches->kShortReco_Rxy->push_back(j_kSReco_Rxy);
  m_branches->kShortReco_RxyErr->push_back(j_kSReco_RxyErr);
  m_branches->kShortReco_chi2->push_back(j_kSReco_chi2);
  m_branches->kShortReco_kShortMass->push_back(j_kSReco_kShortMass);
  m_branches->kShortReco_kShortMassErr->push_back(j_kSReco_kShortMassErr);
  m_branches->kShortReco_d0->push_back(j_kSReco_d0);
  m_branches->kShortReco_z0->push_back(j_kSReco_z0);

  m_branches->kShortRecoGood_ndof->push_back(j_kSRecoGood_ndof);
  m_branches->kShortRecoGood_px->push_back(j_kSRecoGood_px);
  m_branches->kShortRecoGood_py->push_back(j_kSRecoGood_py);
  m_branches->kShortRecoGood_pz->push_back(j_kSRecoGood_pz);
  m_branches->kShortRecoGood_pt->push_back(j_kSRecoGood_pt);
  m_branches->kShortRecoGood_ptErr->push_back(j_kSRecoGood_ptErr);
  m_branches->kShortRecoGood_x->push_back(j_kSRecoGood_x);
  m_branches->kShortRecoGood_y->push_back(j_kSRecoGood_y);
  m_branches->kShortRecoGood_z->push_back(j_kSRecoGood_z);
  m_branches->kShortRecoGood_Rxy->push_back(j_kSRecoGood_Rxy);
  m_branches->kShortRecoGood_RxyErr->push_back(j_kSRecoGood_RxyErr);
  m_branches->kShortRecoGood_chi2->push_back(j_kSRecoGood_chi2);
  m_branches->kShortRecoGood_kShortMass->push_back(j_kSRecoGood_kShortMass);
  m_branches->kShortRecoGood_kShortMassErr->push_back(j_kSRecoGood_kShortMassErr);
  m_branches->kShortRecoGood_d0->push_back(j_kSRecoGood_d0);
  m_branches->kShortRecoGood_z0->push_back(j_kSRecoGood_z0);

}



//!-----------------------------------------------------------------------------------------------------------------------------!//
void KShortRecoBranches::clear() {
  // clear vectors
  m_branches->nKShortReco->clear();
  m_branches->kShortReco_ndof->clear();
  m_branches->kShortReco_px->clear();
  m_branches->kShortReco_py->clear();
  m_branches->kShortReco_pz->clear();
  m_branches->kShortReco_pt->clear();
  m_branches->kShortReco_ptErr->clear(); 
  m_branches->kShortReco_x->clear();
  m_branches->kShortReco_y->clear();
  m_branches->kShortReco_z->clear();
  m_branches->kShortReco_Rxy->clear();
  m_branches->kShortReco_RxyErr->clear();
  m_branches->kShortReco_chi2->clear();
  m_branches->kShortReco_kShortMass->clear();
  m_branches->kShortReco_kShortMassErr->clear();
  m_branches->kShortReco_d0->clear(); 
  m_branches->kShortReco_z0->clear();
 
  m_branches->nKShortRecoGood->clear();
  m_branches->kShortRecoGood_ndof->clear();
  m_branches->kShortRecoGood_px->clear();
  m_branches->kShortRecoGood_py->clear();
  m_branches->kShortRecoGood_pz->clear();
  m_branches->kShortRecoGood_pt->clear();
  m_branches->kShortRecoGood_ptErr->clear();
  m_branches->kShortRecoGood_x->clear();
  m_branches->kShortRecoGood_y->clear();
  m_branches->kShortRecoGood_z->clear();
  m_branches->kShortRecoGood_Rxy->clear();
  m_branches->kShortRecoGood_RxyErr->clear();
  m_branches->kShortRecoGood_chi2->clear();
  m_branches->kShortRecoGood_kShortMass->clear();
  m_branches->kShortRecoGood_kShortMassErr->clear();
  m_branches->kShortRecoGood_d0->clear();
  m_branches->kShortRecoGood_z0->clear();

}



//!-----------------------------------------------------------------------------------------------------------------------------!//
std::vector<float> KShortRecoBranches :: getIPs( double px, double py, double pz, double decayVtx_x, double decayVtx_y, double decayVtx_z, double PV_x, double PV_y, double PV_z ){
  TVector3 track;
  TVector3 refVtx;
  track.SetXYZ( px, py, 0. );
  refVtx.SetXYZ( decayVtx_x - PV_x, decayVtx_y - PV_y, 0. );
  float d0 = refVtx.Mag() * TMath::Sin( refVtx.Angle(track) );
  //float a  = d0; // uncomment for full expression
  float b  = px;
  float c  = decayVtx_x - PV_x;
  float e  = py;
  float d  = decayVtx_y - PV_y;
  //float min_x1 = (TMath::Sqrt( TMath::Power(2*b*c+2*d*e,2) - 4*(b*b+e*e)*(-a*a+c*c+d*d) ) - 2*b*c - 2*d*e)/(2*(b*b+e*e));
  //float min_x2 = (-TMath::Sqrt( TMath::Power(2*b*c+2*d*e,2) - 4*(b*b+e*e)*(-a*a+c*c+d*d) ) - 2*b*c - 2*d*e)/(2*(b*b+e*e));
  float min_x = (-2*b*c - 2*d*e)/(2*(b*b+e*e)); //aproximate to above, but still very accurate (avoids nan's)

  //Calculate z0:
  TF1 *z_eq = new TF1("z_eq", "[0] * x + [1]", -10000., 10000.);
  z_eq->SetParameter(0, pz);
  z_eq->SetParameter(1, decayVtx_z);
  float z0 = z_eq->Eval(min_x) - PV_z;

  std::vector<float> IPs;
  IPs.push_back(d0); // 0th element = d0
  IPs.push_back(z0); // 1st element = z0
  return IPs;

}
