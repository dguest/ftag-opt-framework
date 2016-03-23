#include "ClusterBranches.hh"
#include "ClusterBranchBuffer.hh"

#include "xAODJet/JetConstituentVector.h"
#include "AthContainers/exceptions.h"
#include "TTree.h"

#include <string>
#include <stdexcept>

ClusterBranches::ClusterBranches():
  m_branches(new ClusterBranchBuffer)
{
  m_branches->pt = new std::vector<std::vector<float> >;
  m_branches->eta = new std::vector<std::vector<float> >;
  m_branches->phi = new std::vector<std::vector<float> >;
  m_branches->e = new std::vector<std::vector<float> >;

  m_branches->clusterSize = new std::vector<std::vector<unsigned int> >;

  m_branches->ISOLATION = new std::vector<std::vector<float> >;
  m_branches->LATERAL = new std::vector<std::vector<float> >;
  m_branches->LONGITUDINAL = new std::vector<std::vector<float> >;
  m_branches->SECOND_LAMBDA = new std::vector<std::vector<float> >;
  m_branches->SECOND_R = new std::vector<std::vector<float> >;
  m_branches->CENTER_LAMBDA = new std::vector<std::vector<float> >;
  m_branches->CENTER_MAG = new std::vector<std::vector<float> >;
  m_branches->ENG_POS = new std::vector<std::vector<float> >;
  m_branches->EM_PROBABILITY = new std::vector<std::vector<float> >;
  m_branches->ENG_FRAC_MAX = new std::vector<std::vector<float> >;
  m_branches->FIRST_ENG_DENS = new std::vector<std::vector<float> >;

}

ClusterBranches::~ClusterBranches()
{
  delete m_branches->pt;
  delete m_branches->eta;
  delete m_branches->phi;
  delete m_branches->e;

  delete m_branches->clusterSize;
  delete m_branches->ISOLATION;
  delete m_branches->LATERAL;
  delete m_branches->LONGITUDINAL;
  delete m_branches->SECOND_LAMBDA;
  delete m_branches->SECOND_R;
  delete m_branches->CENTER_LAMBDA;
  delete m_branches->CENTER_MAG;
  delete m_branches->ENG_POS;
  delete m_branches->EM_PROBABILITY;
  delete m_branches->ENG_FRAC_MAX;
  delete m_branches->FIRST_ENG_DENS;

  delete m_branches;
}

namespace {
  // branch name function (lowercase the variable name)
  std::string brnm(const std::string& pfx, std::string in) {
    std::transform(in.begin(), in.end(), in.begin(), ::tolower);
    return pfx + in;
  }
}

void ClusterBranches::set_tree(TTree& output_tree) const {
  std::string prefix = "jet_cluster_";
#define ADD_SIMPLE(nm) \
  output_tree.Branch(brnm(prefix, #nm).c_str(), &m_branches->nm)
  // basic kinematics
  ADD_SIMPLE(pt);
  ADD_SIMPLE(eta);
  ADD_SIMPLE(phi);
  ADD_SIMPLE(e);
  // cluster attributes
  ADD_SIMPLE(clusterSize);
  ADD_SIMPLE(ISOLATION);
  ADD_SIMPLE(LATERAL);
  ADD_SIMPLE(LONGITUDINAL);
  ADD_SIMPLE(SECOND_LAMBDA);
  ADD_SIMPLE(SECOND_R);
  ADD_SIMPLE(CENTER_LAMBDA);
  ADD_SIMPLE(CENTER_MAG);
  ADD_SIMPLE(ENG_POS);
  ADD_SIMPLE(EM_PROBABILITY);
  ADD_SIMPLE(ENG_FRAC_MAX);
  ADD_SIMPLE(FIRST_ENG_DENS);
#undef ADD_SIMPLE
}

void ClusterBranches::fill(const xAOD::JetConstituentVector& constituents) {
  std::vector<float> pt;
  std::vector<float> eta;
  std::vector<float> phi;
  std::vector<float> e;

  for (const auto& cluster: constituents) {
    pt.push_back(cluster->pt());
    eta.push_back(cluster->eta());
    phi.push_back(cluster->phi());
    e.push_back(cluster->e());
  }
  m_branches->pt->push_back(std::move(pt));
  m_branches->eta->push_back(std::move(eta));
  m_branches->phi->push_back(std::move(phi));
  m_branches->e->push_back(std::move(e));

  // get aux attributes
  std::vector<unsigned int> clusterSize;
  std::vector<float> ISOLATION;
  std::vector<float> LATERAL;
  std::vector<float> LONGITUDINAL;
  std::vector<float> SECOND_LAMBDA;
  std::vector<float> SECOND_R;
  std::vector<float> CENTER_LAMBDA;
  std::vector<float> CENTER_MAG;
  std::vector<float> ENG_POS;
  std::vector<float> EM_PROBABILITY;
  std::vector<float> ENG_FRAC_MAX;
  std::vector<float> FIRST_ENG_DENS;
  try {
    for (const auto& cluster: constituents) {
      clusterSize.push_back(cluster->auxdata<unsigned int>("clusterSize"));
#define PUSH_FLOAT(nm) nm.push_back(cluster->auxdata<float>(#nm))
      PUSH_FLOAT(ISOLATION);
      PUSH_FLOAT(LATERAL);
      PUSH_FLOAT(LONGITUDINAL);
      PUSH_FLOAT(SECOND_LAMBDA);
      PUSH_FLOAT(SECOND_R);
      PUSH_FLOAT(CENTER_LAMBDA);
      PUSH_FLOAT(CENTER_MAG);
      PUSH_FLOAT(ENG_POS);
      PUSH_FLOAT(EM_PROBABILITY);
      PUSH_FLOAT(ENG_FRAC_MAX);
      PUSH_FLOAT(FIRST_ENG_DENS);
#undef PUSH_BACK
    }
  } catch (SG::ExcBadAuxVar& exc) {
    std::string problem = "Error filling branches in " __FILE__ " ";
    problem.append(exc.what());
    throw std::runtime_error(problem);
  }
  // push back into member vectors
#define PUSH(var) m_branches->var->push_back(std::move(var))
  PUSH(clusterSize);
  PUSH(ISOLATION);
  PUSH(LATERAL);
  PUSH(LONGITUDINAL);
  PUSH(SECOND_LAMBDA);
  PUSH(SECOND_R);
  PUSH(CENTER_LAMBDA);
  PUSH(CENTER_MAG);
  PUSH(ENG_POS);
  PUSH(EM_PROBABILITY);
  PUSH(ENG_FRAC_MAX);
  PUSH(FIRST_ENG_DENS);
#undef PUSH
}

void ClusterBranches::clear() {
  m_branches->pt->clear();
  m_branches->eta->clear();
  m_branches->phi->clear();
  m_branches->e->clear();

  m_branches->clusterSize->clear();
  m_branches->ISOLATION     ->clear();
  m_branches->LATERAL       ->clear();
  m_branches->LONGITUDINAL  ->clear();
  m_branches->SECOND_LAMBDA ->clear();
  m_branches->SECOND_R      ->clear();
  m_branches->CENTER_LAMBDA ->clear();
  m_branches->CENTER_MAG    ->clear();
  m_branches->ENG_POS       ->clear();
  m_branches->EM_PROBABILITY->clear();
  m_branches->ENG_FRAC_MAX  ->clear();
  m_branches->FIRST_ENG_DENS->clear();

}
