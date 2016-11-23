#include "KShortBranches.hh"
#include "KShortBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include "xAODJet/Jet.h"
#include "xAODTruth/TruthParticle.h"
#include "xAODTruth/TruthVertex.h"
#include "xAODTruth/TruthEvent.h"
#include "xAODTruth/TruthEventContainer.h"
#include "xAODTruth/TruthEventAuxContainer.h"

#include "TVector3.h"
#include "TF1.h"

#include <string>
#include <stdexcept>

using xAOD::IParticle;

//!-----------------------------------------------------------------------------------------------------------------------------!//
KShortBranches::KShortBranches():
  m_branches(new KShortBranchBuffer)
{
  // instantiate all the vectors here ...
  m_branches->nKShort             = new std::vector<int>();

  m_branches->kShort_origPdgid    = new std::vector<std::vector< int> >();

  m_branches->kShort_px           = new std::vector<std::vector< float> >();
  m_branches->kShort_py           = new std::vector<std::vector< float> >();
  m_branches->kShort_pz           = new std::vector<std::vector< float> >();
  m_branches->kShort_pt           = new std::vector<std::vector< float> >();
  m_branches->kShort_eta          = new std::vector<std::vector< float> >();
  m_branches->kShort_phi          = new std::vector<std::vector< float> >();
  m_branches->kShort_E            = new std::vector<std::vector< float> >();
  m_branches->kShort_x            = new std::vector<std::vector< float> >();
  m_branches->kShort_y            = new std::vector<std::vector< float> >();
  m_branches->kShort_z            = new std::vector<std::vector< float> >();
  m_branches->kShort_Rxy          = new std::vector<std::vector< float> >();
  m_branches->kShort_xProd        = new std::vector<std::vector< float> >();
  m_branches->kShort_yProd        = new std::vector<std::vector< float> >();
  m_branches->kShort_zProd        = new std::vector<std::vector< float> >();
  m_branches->kShort_d0_truth     = new std::vector<std::vector< float> >();
  m_branches->kShort_z0_truth     = new std::vector<std::vector< float> >();

}

//!-----------------------------------------------------------------------------------------------------------------------------!//
KShortBranches::~KShortBranches() {
  // delete all the vectors here ...
  delete m_branches->nKShort;

  delete m_branches->kShort_origPdgid;

  delete m_branches->kShort_px;
  delete m_branches->kShort_py;
  delete m_branches->kShort_pz;
  delete m_branches->kShort_pt;
  delete m_branches->kShort_eta;
  delete m_branches->kShort_phi;
  delete m_branches->kShort_E;
  delete m_branches->kShort_x;
  delete m_branches->kShort_y;
  delete m_branches->kShort_z;
  delete m_branches->kShort_Rxy;
  delete m_branches->kShort_xProd;
  delete m_branches->kShort_yProd;
  delete m_branches->kShort_zProd;
  delete m_branches->kShort_d0_truth;
  delete m_branches->kShort_z0_truth;

  delete m_branches;
}

void KShortBranches::set_tree(TTree& output_tree) const {
  output_tree.Branch( "jet_nKShort"       , &m_branches->nKShort);

  output_tree.Branch( "jet_kShort_origPdgid"       , &m_branches->kShort_origPdgid);

  output_tree.Branch( "jet_kShort_px"       , &m_branches->kShort_px);
  output_tree.Branch( "jet_kShort_py"       , &m_branches->kShort_py);
  output_tree.Branch( "jet_kShort_pz"       , &m_branches->kShort_pz);
  output_tree.Branch( "jet_kShort_pt"       , &m_branches->kShort_pt);
  output_tree.Branch( "jet_kShort_eta"      , &m_branches->kShort_eta);
  output_tree.Branch( "jet_kShort_phi"      , &m_branches->kShort_phi);
  output_tree.Branch( "jet_kShort_E"        , &m_branches->kShort_E);
  output_tree.Branch( "jet_kShort_x"        , &m_branches->kShort_x);
  output_tree.Branch( "jet_kShort_y"        , &m_branches->kShort_y);
  output_tree.Branch( "jet_kShort_z"        , &m_branches->kShort_z);
  output_tree.Branch( "jet_kShort_Rxy"      , &m_branches->kShort_Rxy);
  output_tree.Branch( "jet_kShort_xProd"    , &m_branches->kShort_xProd);
  output_tree.Branch( "jet_kShort_yProd"    , &m_branches->kShort_yProd);
  output_tree.Branch( "jet_kShort_zProd"    , &m_branches->kShort_zProd);
  output_tree.Branch( "jet_kShort_d0_truth" , &m_branches->kShort_d0_truth);
  output_tree.Branch( "jet_kShort_z0_truth" , &m_branches->kShort_z0_truth);


}


//!-----------------------------------------------------------------------------------------------------------------------------!//
//void KShortBranches::fill(const xAOD::Jet& jet,const xAOD::TruthEvent& truth) {
void KShortBranches::fill(const xAOD::Jet& jet,const xAOD::TruthEventContainer& xTruthEventContainer,double PV_x, double PV_y, double PV_z) {

  std::vector<float>   j_kS_px;
  std::vector<float>   j_kS_py;
  std::vector<float>   j_kS_pz;
  std::vector<float>   j_kS_pt;
  std::vector<float>   j_kS_eta;
  std::vector<float>   j_kS_phi;
  std::vector<float>   j_kS_E;
  std::vector<float>   j_kS_x;
  std::vector<float>   j_kS_y;
  std::vector<float>   j_kS_z;
  std::vector<float>   j_kS_Rxy;
  std::vector<float>   j_kS_xProd;
  std::vector<float>   j_kS_yProd;
  std::vector<float>   j_kS_zProd;
  std::vector<int>     j_kS_origPdgid;
  std::vector<float>   j_kS_d0_truth;
  std::vector<float>   j_kS_z0_truth;

  for ( const auto* truth : xTruthEventContainer ) { // Get truth event
    for(unsigned int i = 0; i < truth->nTruthParticles(); i++) { // Loop over truth particles
      const xAOD::TruthParticle *particle = truth->truthParticle(i);
      if( isMatchedKShort(particle,jet) ){
	const xAOD::TruthVertex *decayVtx = particle->decayVtx();
	const xAOD::TruthVertex *prodVtx  = particle->prodVtx();
      
	float Rxy = TMath::Sqrt( TMath::Power(decayVtx->x(),2) + TMath::Power(decayVtx->y(),2) );
	int origPdgid = kShortOrigPdgid(particle);

	std::vector<float> IPs_truth = getIPs(particle->px(), particle->py(), particle->pz(), decayVtx->x(), decayVtx->y(), decayVtx->z(), PV_x, PV_y, PV_z);
	float d0_truth = IPs_truth.at(0);
	float z0_truth = IPs_truth.at(1);

	//std::cout << "JEFF    " << origPdgid << std::endl;
	j_kS_origPdgid.push_back(origPdgid);
	j_kS_px.push_back(particle->px());
	j_kS_py.push_back(particle->py());
	j_kS_pz.push_back(particle->pz());
	j_kS_pt.push_back(particle->pt());
	j_kS_eta.push_back(particle->eta());
	j_kS_phi.push_back(particle->phi());
	j_kS_E.push_back(particle->e());
	j_kS_x.push_back(decayVtx->x());
	j_kS_y.push_back(decayVtx->y());
	j_kS_z.push_back(decayVtx->z());
	j_kS_Rxy.push_back(Rxy);
	j_kS_xProd.push_back(prodVtx->x());
	j_kS_yProd.push_back(prodVtx->y());
	j_kS_zProd.push_back(prodVtx->z());
	j_kS_d0_truth.push_back(d0_truth);
	j_kS_z0_truth.push_back(z0_truth);
      }
    } // end loop over truth particles
  } // end loop over truth events

  m_branches->nKShort->push_back(j_kS_px.size());

  if( j_kS_px.size() == 0 ){
    j_kS_origPdgid.push_back(-99);
    j_kS_px.push_back(-99);
    j_kS_py.push_back(-99);
    j_kS_pz.push_back(-99);
    j_kS_pt.push_back(-99);
    j_kS_eta.push_back(-99);
    j_kS_phi.push_back(-99);
    j_kS_E.push_back(-99);
    j_kS_x.push_back(-99);
    j_kS_y.push_back(-99);
    j_kS_z.push_back(-99);
    j_kS_Rxy.push_back(-99);
    j_kS_xProd.push_back(-99);
    j_kS_yProd.push_back(-99);
    j_kS_zProd.push_back(-99);
    j_kS_d0_truth.push_back(-99);
    j_kS_z0_truth.push_back(-99);
  }

  m_branches->kShort_origPdgid->push_back(j_kS_origPdgid);
  m_branches->kShort_px->push_back(j_kS_px);
  m_branches->kShort_py->push_back(j_kS_py);
  m_branches->kShort_pz->push_back(j_kS_pz);
  m_branches->kShort_pt->push_back(j_kS_pt);
  m_branches->kShort_eta->push_back(j_kS_eta);
  m_branches->kShort_phi->push_back(j_kS_phi);
  m_branches->kShort_E->push_back(j_kS_E);
  m_branches->kShort_x->push_back(j_kS_x);
  m_branches->kShort_y->push_back(j_kS_y);
  m_branches->kShort_z->push_back(j_kS_z);
  m_branches->kShort_Rxy->push_back(j_kS_Rxy);
  m_branches->kShort_xProd->push_back(j_kS_xProd);
  m_branches->kShort_yProd->push_back(j_kS_yProd);
  m_branches->kShort_zProd->push_back(j_kS_zProd);
  m_branches->kShort_d0_truth->push_back(j_kS_d0_truth);
  m_branches->kShort_z0_truth->push_back(j_kS_z0_truth);

}



//!-----------------------------------------------------------------------------------------------------------------------------!//
void KShortBranches::clear() {
  // clear vectors
  m_branches->nKShort->clear();
  m_branches->kShort_origPdgid->clear();
  m_branches->kShort_px->clear();
  m_branches->kShort_py->clear();
  m_branches->kShort_pz->clear();
  m_branches->kShort_pt->clear();
  m_branches->kShort_eta->clear();
  m_branches->kShort_phi->clear();
  m_branches->kShort_E->clear();
  m_branches->kShort_x->clear();
  m_branches->kShort_y->clear();
  m_branches->kShort_z->clear();
  m_branches->kShort_Rxy->clear();
  m_branches->kShort_xProd->clear();
  m_branches->kShort_yProd->clear();
  m_branches->kShort_zProd->clear();
  m_branches->kShort_d0_truth->clear();
  m_branches->kShort_z0_truth->clear();

}


//!-----------------------------------------------------------------------------------------------------------------------------!//
bool KShortBranches :: isMatchedKShort( const xAOD::TruthParticle* particle, const xAOD::Jet& jet ){
  // Check if truth particle is a K0short, has two decay products, is inside the jet
  TLorentzVector jet_p4;
  jet_p4.SetPtEtaPhiM(jet.pt(), jet.eta(), jet.phi(), jet.m());
  bool result = false;
  if( particle->hasDecayVtx() && particle->hasProdVtx() ){ // check that particle has a production/decay vertex
    if( particle->decayVtx()->nOutgoingParticles() == 2 && particle->absPdgId() == 310 && particle->decayVtx()->outgoingParticle(0)->absPdgId() == 211 ){ // check that incoming particle is a K0short
      TLorentzVector particle_p4;
      particle_p4.SetPtEtaPhiM(particle->pt(), particle->eta(), particle->phi(), particle->m());
      if( jet_p4.DeltaR(particle_p4) <= 0.4 ){ // check that true K0short is inside the jet
        result = true;
      }
    }
  }
  return result;
}


//!-----------------------------------------------------------------------------------------------------------------------------!//
int KShortBranches :: kShortOrigPdgid( const xAOD::TruthParticle* particle ){
  auto particle_copy            = particle;
  int  next_nParents            = particle->nParents();
  bool next_parentIsParton      = false;
  bool next_parentIsGenSpecific = false;
  int  this_parentPdgid         = -999;
  while( next_nParents > 0 && !next_parentIsParton && !next_parentIsGenSpecific ){
    auto next_parent         = particle_copy->parent();
    // http://acode-browser2.usatlas.bnl.gov/lxr-AthAna/source/atlas/PhysicsAnalysis/TrackingID/InDetTrackSystematicsTools/Root/InDetTrackTruthOriginTool.cxx
    //if (next_parent->isCharmHadron()){
    //  std::cout << "JEFF    " << next_parent->absPdgId() << std::endl;
    //}
    this_parentPdgid         = particle_copy->absPdgId();
    next_nParents            = next_parent->nParents();
    next_parentIsParton      = next_parent->isParton();
    next_parentIsGenSpecific = next_parent->isGenSpecific();
    particle_copy            = next_parent;
  }
  int result = this_parentPdgid;
  return result;
}


//!-----------------------------------------------------------------------------------------------------------------------------!//
std::vector<float> KShortBranches :: getIPs( double px, double py, double pz, double decayVtx_x, double decayVtx_y, double decayVtx_z, double PV_x, double PV_y, double PV_z ){
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
