#include "TrackBranches.hh"
#include "TrackBranchBuffer.hh"

#include "xAODTracking/TrackParticleContainer.h"
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
  m_branches->ntrk = new std::vector<int>;

  m_branches->pt = new std::vector<std::vector<float> >;
  m_branches->eta = new std::vector<std::vector<float> >;
  m_branches->theta = new std::vector<std::vector<float> >;
  m_branches->phi = new std::vector<std::vector<float> >;

  m_branches->qoverp = new std::vector<std::vector<float> >;
  m_branches->chi2 = new std::vector<std::vector<float> >;
  m_branches->ndf = new std::vector<std::vector<float> >;

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

  // actual d0 variables (not lifetime-signed)
  m_branches->ip_d0 = new std::vector<std::vector<float> >;
  m_branches->ip_z0 = new std::vector<std::vector<float> >;
  m_branches->ip_d0sig = new std::vector<std::vector<float> >;
  m_branches->ip_z0sig = new std::vector<std::vector<float> >;

}

TrackBranches::~TrackBranches()
{
  delete m_branches->ntrk;

  delete m_branches->pt;
  delete m_branches->eta;
  delete m_branches->theta;
  delete m_branches->phi;

  delete m_branches->qoverp;
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

void TrackBranches::fill(const TrackBranches::PartLinks& tracks,
                         const xAOD::Vertex& vxp,
                         const Trk::ITrackToVertexIPEstimator& trk2vrt) {
  if (!m_active) return;

  std::vector<float> pt;
  std::vector<float> eta;
  std::vector<float> theta;
  std::vector<float> phi;
  std::vector<float> qoverp;

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

  std::vector<float> d0;
  std::vector<float> z0;
  std::vector<float> ip_d0;
  std::vector<float> ip_z0;
  std::vector<float> ip_d0sig;
  std::vector<float> ip_z0sig;

  for (const PartLink& tl: tracks) {
    const auto* tmpTrk = dynamic_cast<const xAOD::TrackParticle*>(*tl);
    if (!tmpTrk) throw std::logic_error("This isn't a track particle");
    const auto& track = *tmpTrk;
    pt.push_back(tmpTrk->pt());
    eta.push_back(tmpTrk->eta());
    theta.push_back(tmpTrk->theta());
    phi.push_back(tmpTrk->phi());
    qoverp.push_back(tmpTrk->qOverP());

    chi2.push_back(tmpTrk->chiSquared());
    ndf.push_back(tmpTrk->numberDoF());

    nInnHits.push_back(get(track, xAOD::numberOfInnermostPixelLayerHits));
    nNextToInnHits.push_back(get(track, xAOD::numberOfNextToInnermostPixelLayerHits));
    nBLHits.push_back(get(track, xAOD::numberOfBLayerHits));
    nsharedBLHits.push_back(get(track, xAOD::numberOfBLayerSharedHits));
    nsplitPixHits.push_back(get(track, xAOD::numberOfBLayerSplitHits));
    nPixHits.push_back(get(track, xAOD::numberOfPixelHits));
    nPixHoles.push_back(get(track, xAOD::numberOfPixelHoles));
    nsharedPixHits.push_back(get(track, xAOD::numberOfPixelSharedHits));
    nsplitPixHits.push_back(get(track, xAOD::numberOfPixelSplitHits));
    nSCTHits.push_back(get(track, xAOD::numberOfSCTHits));
    nSCTHoles.push_back(get(track, xAOD::numberOfSCTHoles));
    nsharedSCTHits.push_back(get(track, xAOD::numberOfSCTSharedHits));
    expectBLayerHit.push_back(get(track, xAOD::expectBLayerHit));

    d0.push_back(track.d0());
    z0.push_back(track.z0());

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

  CLEAR(d0);
  CLEAR(z0);
  CLEAR(ip_d0);
  CLEAR(ip_z0);
  CLEAR(ip_d0sig);
  CLEAR(ip_z0sig);

#undef CLEAR

}
