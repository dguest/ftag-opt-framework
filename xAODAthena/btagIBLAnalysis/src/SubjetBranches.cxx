#include "SubjetBranches.hh"
#include "SubjetBranchBuffer.hh"
#include "DummyValues.hh"

#include "xAODJet/Jet.h"
#include "TTree.h"

#include <string>

namespace {
  void add_jetfitter(const std::vector<const xAOD::Jet*>& subjets,
                     SubjetBranchBuffer&);
  void add_ip3d(const std::vector<const xAOD::Jet*>& subjets,
                SubjetBranchBuffer&);
  void add_sv1(const std::vector<const xAOD::Jet*>& subjets,
               SubjetBranchBuffer&);
}

SubjetBranches::SubjetBranches():
  m_branches(new SubjetBranchBuffer)
{
  m_branches->pt = new std::vector<std::vector<float> >;
  m_branches->eta = new std::vector<std::vector<float> >;
  m_branches->phi = new std::vector<std::vector<float> >;
  m_branches->m = new std::vector<std::vector<float> >;

  m_branches->ntrk = new std::vector<std::vector<int> >;
  m_branches->mv2c20 = new std::vector<std::vector<float> >;

  m_branches->jf_m = new              std::vector<std::vector<float> >;
  m_branches->jf_mUncorr = new        std::vector<std::vector<float> >;
  m_branches->jf_efc = new            std::vector<std::vector<float> >;
  m_branches->jf_deta = new           std::vector<std::vector<float> >;
  m_branches->jf_dphi = new           std::vector<std::vector<float> >;
  m_branches->jf_dRFlightDir = new    std::vector<std::vector<float> >;
  m_branches->jf_ntrkAtVx = new       std::vector<std::vector<int> >;
  m_branches->jf_nvtx = new           std::vector<std::vector<int> >;
  m_branches->jf_sig3d = new          std::vector<std::vector<float> >;
  m_branches->jf_nvtx1t = new         std::vector<std::vector<int> >;
  m_branches->jf_n2t = new            std::vector<std::vector<int> >;
  m_branches->jf_VTXsize = new        std::vector<std::vector<int> >;

  m_branches->ip3d_pb = new std::vector<std::vector<float> >;
  m_branches->ip3d_pc = new std::vector<std::vector<float> >;
  m_branches->ip3d_pu = new std::vector<std::vector<float> >;
  m_branches->ip3d_ntrk = new std::vector<std::vector<int> >;

  m_branches->sv1_ntrkv = new std::vector<std::vector<int> >;
  m_branches->sv1_n2t = new std::vector<std::vector<int> >;
  m_branches->sv1_m = new std::vector<std::vector<float> >;
  m_branches->sv1_efc = new std::vector<std::vector<float> >;
  m_branches->sv1_Nvtx = new std::vector<std::vector<int> >;
  m_branches->sv1_normdist = new std::vector<std::vector<float> >;
}

SubjetBranches::~SubjetBranches()
{
  delete m_branches->pt;
  delete m_branches->eta;
  delete m_branches->phi;
  delete m_branches->m;

  delete m_branches->ntrk;
  delete m_branches->mv2c20;

  delete m_branches->jf_m;
  delete m_branches->jf_mUncorr;
  delete m_branches->jf_efc;
  delete m_branches->jf_deta;
  delete m_branches->jf_dphi;
  delete m_branches->jf_dRFlightDir;
  delete m_branches->jf_ntrkAtVx;
  delete m_branches->jf_nvtx;
  delete m_branches->jf_sig3d;
  delete m_branches->jf_nvtx1t;
  delete m_branches->jf_n2t;
  delete m_branches->jf_VTXsize;

  delete m_branches->ip3d_pb;
  delete m_branches->ip3d_pc;
  delete m_branches->ip3d_pu;
  delete m_branches->ip3d_ntrk;

  delete m_branches->sv1_ntrkv;
  delete m_branches->sv1_n2t;
  delete m_branches->sv1_m;
  delete m_branches->sv1_efc;
  delete m_branches->sv1_Nvtx;
  delete m_branches->sv1_normdist;

  delete m_branches;
}

void SubjetBranches::set_tree(TTree& output_tree,
                              const std::string& prefix) const {
#define ADD_SIMPLE(nm) \
  output_tree.Branch((prefix + #nm).c_str(), &m_branches->nm)
  // basic kinematics
  ADD_SIMPLE(pt);
  ADD_SIMPLE(eta);
  ADD_SIMPLE(phi);
  ADD_SIMPLE(m);

  // general stuff
  ADD_SIMPLE(ntrk);
  ADD_SIMPLE(mv2c20);

  // jetfitter
  ADD_SIMPLE(jf_m);
  ADD_SIMPLE(jf_mUncorr);
  ADD_SIMPLE(jf_efc);
  ADD_SIMPLE(jf_deta);
  ADD_SIMPLE(jf_dphi);
  ADD_SIMPLE(jf_dRFlightDir);
  ADD_SIMPLE(jf_ntrkAtVx);
  ADD_SIMPLE(jf_nvtx);
  ADD_SIMPLE(jf_sig3d);
  ADD_SIMPLE(jf_nvtx1t);
  ADD_SIMPLE(jf_n2t);
  ADD_SIMPLE(jf_VTXsize);

  // IP3D
  ADD_SIMPLE(ip3d_pb);
  ADD_SIMPLE(ip3d_pc);
  ADD_SIMPLE(ip3d_pu);
  ADD_SIMPLE(ip3d_ntrk);

  // SV1
  ADD_SIMPLE(sv1_ntrkv);
  ADD_SIMPLE(sv1_n2t);
  ADD_SIMPLE(sv1_m);
  ADD_SIMPLE(sv1_efc);
  ADD_SIMPLE(sv1_Nvtx);
  ADD_SIMPLE(sv1_normdist);
#undef ADD_SIMPLE
}

void SubjetBranches::fill(const std::vector<const xAOD::Jet*>& subjets) {
  std::vector<float> pt;
  std::vector<float> eta;
  std::vector<float> phi;
  std::vector<float> m;
  std::vector<int> ntrk;
  std::vector<float> mv2c20;

  for (const auto* jet: subjets) {
    pt.push_back(jet->pt());
    eta.push_back(jet->eta());
    phi.push_back(jet->phi());
    m.push_back(jet->m());
    ntrk.push_back(jet->numConstituents());
    const xAOD::BTagging *btag = jet->btagging();
    mv2c20.push_back(btag->auxdata<double>("MV2c20_discriminant"));
  }
#define PUSH(var) m_branches->var->push_back(std::move(var))
  PUSH(pt);
  PUSH(eta);
  PUSH(phi);
  PUSH(m);
  PUSH(ntrk);
  PUSH(mv2c20);
#undef PUSH

  // add other tagger info
  add_jetfitter(subjets, *m_branches);
  add_ip3d(subjets, *m_branches);
  add_sv1(subjets, *m_branches);
}

void SubjetBranches::clear() {
#define CLEAR(var) m_branches->var->clear()
  // basic kinematics
  CLEAR(pt);
  CLEAR(eta);
  CLEAR(phi);
  CLEAR(m);

  // general stuff
  CLEAR(ntrk);
  CLEAR(mv2c20);

  // jetfitter
  CLEAR(jf_m);
  CLEAR(jf_mUncorr);
  CLEAR(jf_efc);
  CLEAR(jf_deta);
  CLEAR(jf_dphi);
  CLEAR(jf_dRFlightDir);
  CLEAR(jf_ntrkAtVx);
  CLEAR(jf_nvtx);
  CLEAR(jf_sig3d);
  CLEAR(jf_nvtx1t);
  CLEAR(jf_n2t);
  CLEAR(jf_VTXsize);

  // IP3D
  CLEAR(ip3d_pb);
  CLEAR(ip3d_pc);
  CLEAR(ip3d_pu);
  CLEAR(ip3d_ntrk);

  // SV1
  CLEAR(sv1_ntrkv);
  CLEAR(sv1_n2t);
  CLEAR(sv1_m);
  CLEAR(sv1_efc);
  CLEAR(sv1_Nvtx);
  CLEAR(sv1_normdist);

#undef CLEAR
}


// ________________________________________________________________________
// filler functions

namespace {
  typedef std::vector<ElementLink<xAOD::BTagVertexContainer> > BTagVertices;
  void add_jetfitter(const std::vector<const xAOD::Jet*>& subjets,
                     SubjetBranchBuffer& buffer) {

    std::vector<float> jf_m;
    std::vector<float> jf_mUncorr;
    std::vector<float> jf_efc;
    std::vector<float> jf_deta;
    std::vector<float> jf_dphi;
    std::vector<float> jf_dRFlightDir;
    std::vector<int> jf_ntrkAtVx;
    std::vector<int> jf_nvtx;
    std::vector<float> jf_sig3d;
    std::vector<int> jf_nvtx1t;
    std::vector<int> jf_n2t;
    std::vector<int> jf_VTXsize;
    // TODO: Missing some clustered vertex information here

    // TODO: figure out what the `Remco` variables are all about
    // std::vector<float> jf_phi;
    // std::vector<float> jf_theta;
    for (const auto* jet: subjets) {
      const xAOD::BTagging* bjet = jet->btagging();
      // get vertex counts
      int nvtx = DUMMY_INT;
      int nvtx1t = DUMMY_INT;
      bjet->taggerInfo(nvtx, xAOD::JetFitter_nVTX);
      bjet->taggerInfo(nvtx1t, xAOD::JetFitter_nSingleTracks);

      // define dummy values
      float m = DUMMY_FLOAT;
      float mUncorr = DUMMY_FLOAT;
      float efc = DUMMY_FLOAT;
      float deta = DUMMY_FLOAT;
      float dphi = DUMMY_FLOAT;
      float dRFlightDir = DUMMY_FLOAT;
      int ntrkAtVx = DUMMY_INT;
      float sig3d = DUMMY_FLOAT;
      int n2t = DUMMY_INT;
      int VTXsize = DUMMY_INT;
      if (nvtx1t + nvtx > 0) {
        bjet->taggerInfo(m, xAOD::JetFitter_mass);
        bjet->variable<float>("JetFitter", "massUncorr", mUncorr); //eloi
        bjet->taggerInfo(efc, xAOD::JetFitter_energyFraction);
        bjet->taggerInfo(deta, xAOD::JetFitter_deltaeta);
        bjet->taggerInfo(dphi, xAOD::JetFitter_deltaphi);
        bjet->variable<float>("JetFitter", "dRFlightDir", dRFlightDir); //eloi
        bjet->taggerInfo(ntrkAtVx, xAOD::JetFitter_nTracksAtVtx);
        bjet->taggerInfo(sig3d, xAOD::JetFitter_significance3d);
        bjet->taggerInfo(n2t, xAOD::JetFitter_N2Tpair);
        VTXsize = bjet->auxdata<BTagVertices>("JetFitter_JFvertices").size();
      }
      // fill for this subjet
#define PUSH(var) jf_ ## var.push_back(var)
      PUSH(m);
      PUSH(mUncorr);
      PUSH(efc);
      PUSH(deta);
      PUSH(dphi);
      PUSH(dRFlightDir);
      PUSH(ntrkAtVx);
      PUSH(sig3d);
      PUSH(n2t);
      PUSH(VTXsize);
      PUSH(nvtx1t);
      PUSH(nvtx);
#undef PUSH
    }
    // fill for this fat jet
#define PUSH(var) buffer.var->push_back(std::move(var))
    PUSH(jf_m);
    PUSH(jf_mUncorr);
    PUSH(jf_efc);
    PUSH(jf_deta);
    PUSH(jf_dphi);
    PUSH(jf_dRFlightDir);
    PUSH(jf_ntrkAtVx);
    PUSH(jf_sig3d);
    PUSH(jf_n2t);
    PUSH(jf_VTXsize);
    PUSH(jf_nvtx1t);
    PUSH(jf_nvtx);
#undef PUSH

  }

  typedef std::vector<ElementLink< xAOD::TrackParticleContainer> > Tracks;
  void add_ip3d(const std::vector<const xAOD::Jet*>& subjets,
                SubjetBranchBuffer& buffer) {
    std::vector<float> pb;
    std::vector<float> pc;
    std::vector<float> pu;
    std::vector<int> ntrk;
    for (const auto* jet: subjets) {
      const xAOD::BTagging* bjet = jet->btagging();
      int n_trk = bjet->auxdata<Tracks>("IP3D_TrackParticleLinks").size();
      ntrk.push_back(n_trk);
      if (n_trk > 0) {
        pb.push_back(bjet->IP3D_pb());
        pc.push_back(bjet->IP3D_pc());
        pu.push_back(bjet->IP3D_pu());
      } else {
        pb.push_back(DUMMY_FLOAT);
        pc.push_back(DUMMY_FLOAT);
        pu.push_back(DUMMY_FLOAT);
      }
    }
    buffer.ip3d_ntrk->push_back(std::move(ntrk));
    buffer.ip3d_pb->push_back(std::move(pb));
    buffer.ip3d_pc->push_back(std::move(pc));
    buffer.ip3d_pu->push_back(std::move(pu));
  }

  typedef std::vector<ElementLink<xAOD::VertexContainer > > Vertices;
  void add_sv1(const std::vector<const xAOD::Jet*>& subjets,
               SubjetBranchBuffer& buffer) {
    std::vector<int> sv1_ntrkv;
    std::vector<int> sv1_n2t;
    std::vector<int> sv1_Nvtx;
    std::vector<float> sv1_m;
    std::vector<float> sv1_efc;
    std::vector<float> sv1_normdist;
    for (const auto* jet: subjets) {
      const xAOD::BTagging* bjet = jet->btagging();
      int n_vx = bjet->auxdata<Vertices>("SV1_vertices").size();
      sv1_Nvtx.push_back(n_vx);

      int ntrkv = DUMMY_INT;
      int n2t = DUMMY_INT;
      float m = DUMMY_FLOAT;
      float efc = DUMMY_FLOAT;
      float normdist = DUMMY_FLOAT;
      if (n_vx > 0) {
        bjet->taggerInfo(ntrkv, xAOD::SV1_NGTinSvx);
        bjet->taggerInfo(n2t, xAOD::SV1_N2Tpair);
        bjet->taggerInfo(m, xAOD::SV1_masssvx);
        bjet->taggerInfo(efc, xAOD::SV1_efracsvx);
        bjet->taggerInfo(normdist, xAOD::SV1_normdist);
      }
      // fill for this subjet
#define PUSH(var) sv1_ ## var.push_back(var)
      PUSH(ntrkv);
      PUSH(n2t);
      PUSH(m);
      PUSH(efc);
      PUSH(normdist);
#undef PUSH
    }
    // fill for this fat jet
#define PUSH(var) buffer.var->push_back(std::move(var))
    PUSH(sv1_ntrkv);
    PUSH(sv1_n2t);
    PUSH(sv1_Nvtx);
    PUSH(sv1_m);
    PUSH(sv1_efc);
    PUSH(sv1_normdist);
#undef PUSH
  }

}
