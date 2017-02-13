#include "TrackBranches.hh"
#include "TrackBranchBuffer.hh"

#include "xAODTracking/TrackParticleContainer.h"
#include "xAODTracking/Vertex.h"
#include "TrkVertexFitterInterfaces/ITrackToVertexIPEstimator.h"
#include "AthContainers/exceptions.h"

#include "TTree.h"

#include <string>
#include <stdexcept>
#include <memory>

TrackBranches::TrackBranches():
  m_branches(new TrackBranchBuffer),
  m_active(false)
{
  typedef std::vector<std::vector<int> > vvi_t;
  typedef std::vector<std::vector<float> > vvf_t;

  m_branches->ntrk = new std::vector<int>;

  m_branches->pt = new vvf_t;
  m_branches->eta = new vvf_t;
  m_branches->theta = new vvf_t;
  m_branches->phi = new vvf_t;
  m_branches->qoverp = new vvf_t;

  m_branches->charge = new vvf_t;

  m_branches->chi2 = new vvf_t;
  m_branches->ndf = new vvf_t;

  m_branches->nNextToInnHits = new vvi_t;
  m_branches->nInnHits = new vvi_t;
  m_branches->nBLHits = new vvi_t; // soo this will be deprecated
  m_branches->nsharedBLHits = new vvi_t;
  m_branches->nsplitBLHits = new vvi_t;
  m_branches->nPixHits = new vvi_t;
  m_branches->nPixHoles = new vvi_t;
  m_branches->nsharedPixHits = new vvi_t;
  m_branches->nsplitPixHits = new vvi_t;
  m_branches->nSCTHits = new vvi_t;
  m_branches->nSCTHoles = new vvi_t;
  m_branches->nsharedSCTHits = new vvi_t;

  m_branches->expectBLayerHit = new vvi_t;
  m_branches->expectInnermostPixelLayerHit = new vvi_t;
  m_branches->expectNextToInnermostPixelLayerHit = new vvi_t;

  m_branches->d0 = new vvf_t;
  m_branches->z0 = new vvf_t;

  // actual d0 variables (not lifetime-signed)
  m_branches->ip_d0 = new vvf_t;
  m_branches->ip_z0 = new vvf_t;
  m_branches->ip_d0sig = new vvf_t;
  m_branches->ip_z0sig = new vvf_t;

}

TrackBranches::~TrackBranches()
{
  delete m_branches->ntrk;

  delete m_branches->pt;
  delete m_branches->eta;
  delete m_branches->theta;
  delete m_branches->phi;
  delete m_branches->qoverp;

  delete m_branches->charge;

  delete m_branches->chi2;
  delete m_branches->ndf;

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
  delete m_branches->expectInnermostPixelLayerHit;
  delete m_branches->expectNextToInnermostPixelLayerHit;

  delete m_branches->d0;
  delete m_branches->z0;
  // actual d0 variables (not lifetime-signed)
  delete m_branches->ip_d0;
  delete m_branches->ip_z0;
  delete m_branches->ip_d0sig;
  delete m_branches->ip_z0sig;

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
  if (m_active) throw std::logic_error("tried to set tree twice");
  m_active = true;

#define ADD_SIMPLE(nm) \
  output_tree.Branch(brnm(prefix, #nm).c_str(), &m_branches->nm)
  // basic kinematics
  ADD_SIMPLE(pt);
  ADD_SIMPLE(eta);
  ADD_SIMPLE(theta);
  ADD_SIMPLE(phi);
  ADD_SIMPLE(qoverp);

  ADD_SIMPLE(charge);

  ADD_SIMPLE(chi2);
  ADD_SIMPLE(ndf);

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
  ADD_SIMPLE(expectInnermostPixelLayerHit);
  ADD_SIMPLE(expectNextToInnermostPixelLayerHit);

  ADD_SIMPLE(d0);
  ADD_SIMPLE(z0);
  ADD_SIMPLE(ip_d0);
  ADD_SIMPLE(ip_z0);
  ADD_SIMPLE(ip_d0sig);
  ADD_SIMPLE(ip_z0sig);

#undef ADD_SIMPLE
}

namespace {
  // util function to get track values
  int get(const xAOD::TrackParticle& part, xAOD::SummaryType info) {
    uint8_t val;
    bool ok = part.summaryValue(val, info);
    if (!ok) throw std::logic_error("problem getting track summary value");
    return val;
  }
}

void TrackBranches::fill(const TrackBranches::PartVector& tracks,
                         const xAOD::Vertex& vxp,
                         const Trk::ITrackToVertexIPEstimator& trk2vrt) {
  if (!m_active) return;

  std::vector<float> pt;
  std::vector<float> eta;
  std::vector<float> theta;
  std::vector<float> phi;
  std::vector<float> qoverp;

  std::vector<float> charge;

  std::vector<float> chi2;
  std::vector<float> ndf;

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
  std::vector<int> expectInnermostPixelLayerHit;
  std::vector<int> expectNextToInnermostPixelLayerHit;

  std::vector<float> d0;
  std::vector<float> z0;
  std::vector<float> ip_d0;
  std::vector<float> ip_z0;
  std::vector<float> ip_d0sig;
  std::vector<float> ip_z0sig;

  for (const auto* part: tracks) {
    const auto* tmpTrk = dynamic_cast<const xAOD::TrackParticle*>(part);
    if (!tmpTrk) throw std::logic_error("This isn't a track particle");
    const auto& track = *tmpTrk;
    pt.push_back(tmpTrk->pt());
    eta.push_back(tmpTrk->eta());
    theta.push_back(tmpTrk->theta());
    phi.push_back(tmpTrk->phi());
    qoverp.push_back(tmpTrk->qOverP());

    charge.push_back(tmpTrk->charge());

    chi2.push_back(tmpTrk->chiSquared());
    ndf.push_back(tmpTrk->numberDoF());

    nInnHits.push_back(get(track, xAOD::numberOfInnermostPixelLayerHits));
    nNextToInnHits.push_back(get(track, xAOD::numberOfNextToInnermostPixelLayerHits));
    nBLHits.push_back(get(track, xAOD::numberOfBLayerHits));
    nsharedBLHits.push_back(get(track, xAOD::numberOfBLayerSharedHits));
    nsplitBLHits.push_back(get(track, xAOD::numberOfBLayerSplitHits));
    nPixHits.push_back(get(track, xAOD::numberOfPixelHits));
    nPixHoles.push_back(get(track, xAOD::numberOfPixelHoles));
    nsharedPixHits.push_back(get(track, xAOD::numberOfPixelSharedHits));
    nsplitPixHits.push_back(get(track, xAOD::numberOfPixelSplitHits));
    nSCTHits.push_back(get(track, xAOD::numberOfSCTHits));
    nSCTHoles.push_back(get(track, xAOD::numberOfSCTHoles));
    nsharedSCTHits.push_back(get(track, xAOD::numberOfSCTSharedHits));

#define GET(NAME) NAME.push_back(get(track, xAOD::NAME))
    GET(expectBLayerHit);
    GET(expectInnermostPixelLayerHit);
    GET(expectNextToInnermostPixelLayerHit);
#undef GET

    d0.push_back(track.d0());
    z0.push_back(track.z0() + track.vz() - vxp.z());

    auto ip = std::unique_ptr<const Trk::ImpactParametersAndSigma>(
      trk2vrt.estimate(&track, &vxp));
    ip_d0.push_back(ip->IPd0);
    ip_z0.push_back(ip->IPz0SinTheta);
    ip_d0sig.push_back(ip->IPd0 / ip->sigmad0);
    ip_z0sig.push_back(ip->IPz0SinTheta / ip->sigmaz0SinTheta);
  }

  // push back into member vectors
#define PUSH(var) m_branches->var->push_back(std::move(var))
  PUSH(pt);
  PUSH(eta);
  PUSH(theta);
  PUSH(phi);
  PUSH(qoverp);

  PUSH(charge);

  PUSH(chi2);
  PUSH(ndf);

  PUSH(nInnHits);
  PUSH(nNextToInnHits);
  PUSH(nBLHits);
  PUSH(nsharedBLHits);
  PUSH(nsplitBLHits);
  PUSH(nPixHits);
  PUSH(nPixHoles);
  PUSH(nsharedPixHits);
  PUSH(nsplitPixHits);
  PUSH(nSCTHits);
  PUSH(nSCTHoles);
  PUSH(nsharedSCTHits);

  PUSH(expectBLayerHit);
  PUSH(expectInnermostPixelLayerHit);
  PUSH(expectNextToInnermostPixelLayerHit);

  PUSH(d0);
  PUSH(z0);
  PUSH(ip_d0);
  PUSH(ip_z0);
  PUSH(ip_d0sig);
  PUSH(ip_z0sig);

#undef PUSH
}

void TrackBranches::clear() {
#define CLEAR(name) m_branches->name->clear()
  CLEAR(pt);
  CLEAR(eta);
  CLEAR(theta);
  CLEAR(phi);
  CLEAR(qoverp);

  CLEAR(charge);

  CLEAR(chi2);
  CLEAR(ndf);

  CLEAR(nInnHits);
  CLEAR(nNextToInnHits);
  CLEAR(nBLHits);
  CLEAR(nsharedBLHits);
  CLEAR(nsplitBLHits);
  CLEAR(nPixHits);
  CLEAR(nPixHoles);
  CLEAR(nsharedPixHits);
  CLEAR(nsplitPixHits);
  CLEAR(nSCTHits);
  CLEAR(nSCTHoles);
  CLEAR(nsharedSCTHits);

  CLEAR(expectBLayerHit);
  CLEAR(expectInnermostPixelLayerHit);
  CLEAR(expectNextToInnermostPixelLayerHit);

  CLEAR(d0);
  CLEAR(z0);
  CLEAR(ip_d0);
  CLEAR(ip_z0);
  CLEAR(ip_d0sig);
  CLEAR(ip_z0sig);

#undef CLEAR

}
