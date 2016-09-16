#include "TrackBranches.hh"
#include "TrackBranchBuffer.hh"

#include "xAODTracking/TrackParticleContainer.h"
#include "AthContainers/exceptions.h"
#include "InDetTrackSelectionTool/IInDetTrackSelectionTool.h"
#include "TTree.h"

#include <string>
#include <stdexcept>


TrackBranches::TrackBranches():
  m_branches(new TrackBranchBuffer)
{
  m_branches->ntrk = new std::vector<int>;

  m_branches->pt = new std::vector<std::vector<float> >;
  m_branches->eta = new std::vector<std::vector<float> >;
  m_branches->theta = new std::vector<std::vector<float> >;
  m_branches->phi = new std::vector<std::vector<float> >;

  m_branches->qoverp = new std::vector<std::vector<float> >;
  m_branches->dr = new std::vector<std::vector<float> >;
  m_branches->chi2 = new std::vector<std::vector<float> >;
  m_branches->ndf = new std::vector<std::vector<float> >;

  m_branches->is_tracking_cp_loose = new std::vector<std::vector<int> >;

  m_branches->nNextToInnHits = new std::vector<std::vector<int> >;
  m_branches->nInnHits = new std::vector<std::vector<int> >;
  m_branches->nBLHits = new std::vector<std::vector<int> >; // soo this will be deprecated
  m_branches->nsharedBLHits = new std::vector<std::vector<int> >;
  m_branches->nsplitBLHits = new std::vector<std::vector<int> >;
  m_branches->nPixHits = new std::vector<std::vector<int> >;
  m_branches->nPixHoles = new std::vector<std::vector<int> >;
  m_branches->nsharedPixHits = new std::vector<std::vector<int> >;
  m_branches->nsplitPixHits = new std::vector<std::vector<int> >;
  m_branches->nSCTHits = new std::vector<std::vector<int> >;
  m_branches->nSCTHoles = new std::vector<std::vector<int> >;
  m_branches->nsharedSCTHits = new std::vector<std::vector<int> >;
  m_branches->expectBLayerHit = new std::vector<std::vector<int> >;
  m_branches->d0 = new std::vector<std::vector<float> >;
  m_branches->z0 = new std::vector<std::vector<float> >;

  m_branches->IP3D_grade = new std::vector<std::vector<int> >;
  m_branches->IP3D_d0 = new std::vector<std::vector<float> >;
  m_branches->IP3D_d02D = new std::vector<std::vector<float> >;
  m_branches->IP3D_z0 = new std::vector<std::vector<float> >;
  m_branches->IP3D_d0sig = new std::vector<std::vector<float> >;
  m_branches->IP3D_z0sig = new std::vector<std::vector<float> >;
  m_branches->IP2D_llr = new std::vector<std::vector<float> >;
  m_branches->IP3D_llr = new std::vector<std::vector<float> >;

  // actual d0 variables (not lifetime-signed)
  m_branches->ip_d0 = new std::vector<std::vector<float> >;
  m_branches->ip_z0 = new std::vector<std::vector<float> >;

}

TrackBranches::~TrackBranches()
{
  delete m_branches->ntrk;

  delete m_branches->pt;
  delete m_branches->eta;
  delete m_branches->theta;
  delete m_branches->phi;

  delete m_branches->qoverp;
  delete m_branches->dr;
  delete m_branches->chi2;
  delete m_branches->ndf;

  delete m_branches->is_tracking_cp_loose;

  delete m_branches->nNextToInnHits;
  delete m_branches->nInnHits;
  delete m_branches->nBLHits; // soo this will be deprecated
  delete m_branches->nsharedBLHits;
  delete m_branches->nsplitBLHits;
  delete m_branches->nPixHits;
  delete m_branches->nPixHoles;
  delete m_branches->nsharedPixHits;
  delete m_branches->nsplitPixHits;
  delete m_branches->nSCTHits;
  delete m_branches->nSCTHoles;
  delete m_branches->nsharedSCTHits;
  delete m_branches->expectBLayerHit;
  delete m_branches->d0;
  delete m_branches->z0;
  // actual d0 variables (not lifetime-signed)
  delete m_branches->ip_d0;
  delete m_branches->ip_z0;

  delete m_branches->IP3D_grade;
  delete m_branches->IP3D_d0;
  delete m_branches->IP3D_d02D;
  delete m_branches->IP3D_z0;
  delete m_branches->IP3D_d0sig;
  delete m_branches->IP3D_z0sig;
  delete m_branches->IP2D_llr;
  delete m_branches->IP3D_llr;

  delete m_branches;
}

namespace {
  // branch name function (lowercase the variable name)
  std::string brnm(const std::string& pfx, std::string in) {
    // std::transform(in.begin(), in.end(), in.begin(), ::tolower);
    return pfx + in;
  }
}

void TrackBranches::set_tree(TTree& output_tree,
                             const std::string& prefix) const {

#define ADD_SIMPLE(nm) \
  output_tree.Branch(brnm(prefix, #nm).c_str(), &m_branches->nm)
  // basic kinematics
  ADD_SIMPLE(pt);
  ADD_SIMPLE(eta);
  ADD_SIMPLE(theta);
  ADD_SIMPLE(phi);
  ADD_SIMPLE(qoverp);

  ADD_SIMPLE(dr);
  ADD_SIMPLE(chi2);
  ADD_SIMPLE(ndf);
  ADD_SIMPLE(is_tracking_cp_loose);

  ADD_SIMPLE(nNextToInnHits);
  ADD_SIMPLE(nInnHits);
  ADD_SIMPLE(nBLHits); // soo this will be deprecated
  ADD_SIMPLE(nsharedBLHits);
  ADD_SIMPLE(nsplitBLHits);
  ADD_SIMPLE(nPixHits);
  ADD_SIMPLE(nPixHoles);
  ADD_SIMPLE(nsharedPixHits);
  ADD_SIMPLE(nsplitPixHits);
  ADD_SIMPLE(nSCTHits);
  ADD_SIMPLE(nSCTHoles);
  ADD_SIMPLE(nsharedSCTHits);
  ADD_SIMPLE(expectBLayerHit);

  ADD_SIMPLE(d0);
  ADD_SIMPLE(z0);
  ADD_SIMPLE(ip_d0);
  ADD_SIMPLE(ip_z0);

  // the cases here aren't the same, kept for back compatibility
  output_tree.Branch("jet_trk_ip3d_grade", &m_branches->IP3D_grade);
  output_tree.Branch("jet_trk_ip3d_d0",    &m_branches->IP3D_d0);
  output_tree.Branch("jet_trk_ip3d_d02D",  &m_branches->IP3D_d02D);
  output_tree.Branch("jet_trk_ip3d_z0",    &m_branches->IP3D_z0);
  output_tree.Branch("jet_trk_ip3d_d0sig", &m_branches->IP3D_d0sig);
  output_tree.Branch("jet_trk_ip3d_z0sig", &m_branches->IP3D_z0sig);
  output_tree.Branch("jet_trk_ip2d_llr",   &m_branches->IP2D_llr);
  output_tree.Branch("jet_trk_ip3d_llr",   &m_branches->IP3D_llr);

#undef ADD_SIMPLE
}

void TrackBranches::fill(const TrackBranches::TrkLinks& constituents,
                         const InDet::IInDetTrackSelectionTool& cp_tool) {

  std::vector<float> pt;
  std::vector<float> eta;
  std::vector<float> theta;
  std::vector<float> phi;
  std::vector<float> qoverp;

  std::vector<float> dr; // mod nikola
  std::vector<float> chi2;
  std::vector<float> ndf;

  std::vector<int> cploose;
  std::vector<int> nInnHits;
  std::vector<int> nNextToInnHits;
  std::vector<int> nBLHits;
  std::vector<int> nsharedBLHits;
  std::vector<int> nsplitBLHits;
  std::vector<int> nPixHits;
  std::vector<int> nPixHoles;
  std::vector<int> nsharedPixHits;
  std::vector<int> nsplitPixHits;
  std::vector<int> nSCTHits;
  std::vector<int> nSCTHoles;
  std::vector<int> nsharedSCTHits;
  std::vector<int> expectBLayerHit;

  std::vector<float> d0;
  std::vector<float> z0;
  std::vector<float> ip_d0;
  std::vector<float> ip_z0;

  std::vector<int> ip3d_grade;
  std::vector<float> ip3d_d0;
  std::vector<float> ip3d_d02D;
  std::vector<float> ip3d_z0;
  std::vector<float> ip3d_d0sig;
  std::vector<float> ip3d_z0sig;
  std::vector<float> ip2d_llr;
  std::vector<float> ip3d_llr;


  // for (const auto& cluster: constituents) {
  //   if (cluster->type() != xAOD::Type::CaloCluster) return;
  //   pt.push_back(cluster->pt());
  //   eta.push_back(cluster->eta());
  //   phi.push_back(cluster->phi());
  //   e.push_back(cluster->e());
  // }
  // m_branches->pt->push_back(std::move(pt));
  // m_branches->eta->push_back(std::move(eta));
  // m_branches->phi->push_back(std::move(phi));
  // m_branches->e->push_back(std::move(e));

  // push back into member vectors
// #define PUSH(var) m_branches->var->push_back(std::move(var))
//   PUSH(clusterSize);
//   PUSH(ISOLATION);
//   PUSH(LATERAL);
//   PUSH(LONGITUDINAL);
//   PUSH(SECOND_LAMBDA);
//   PUSH(SECOND_R);
//   PUSH(CENTER_LAMBDA);
//   PUSH(CENTER_MAG);
//   PUSH(ENG_POS);
//   PUSH(EM_PROBABILITY);
//   PUSH(ENG_FRAC_MAX);
//   PUSH(FIRST_ENG_DENS);
// #undef PUSH
}

void TrackBranches::clear() {
  m_branches->pt->clear();
  // m_branches->eta->clear();
  // m_branches->phi->clear();
  // m_branches->e->clear();

  // m_branches->clusterSize->clear();
  // m_branches->ISOLATION     ->clear();
  // m_branches->LATERAL       ->clear();
  // m_branches->LONGITUDINAL  ->clear();
  // m_branches->SECOND_LAMBDA ->clear();
  // m_branches->SECOND_R      ->clear();
  // m_branches->CENTER_LAMBDA ->clear();
  // m_branches->CENTER_MAG    ->clear();
  // m_branches->ENG_POS       ->clear();
  // m_branches->EM_PROBABILITY->clear();
  // m_branches->ENG_FRAC_MAX  ->clear();
  // m_branches->FIRST_ENG_DENS->clear();

}
