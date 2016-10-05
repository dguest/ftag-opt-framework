#include "SubstructureMomentBranches.hh"
#include "SubstructureMomentBranchBuffer.hh"

#include "xAODJet/Jet.h"
#include "JetSubStructureUtils/Nsubjettiness.h"
#include "JetSubStructureUtils/EnergyCorrelator.h"
#include "TTree.h"

#include <string>
#include <stdexcept>

namespace {
  const float N_SUBJETIENESS_ALPHA = 1.0;
  JetSubStructureUtils::EnergyCorrelator ecf(int, float beta);
  double ec_c(int n, float beta, const xAOD::Jet&);
  double ec_d2(float beta, const xAOD::Jet&);
}

SubstructureMomentBranches::SubstructureMomentBranches():
  m_branches(new SubstructureMomentBranchBuffer)
{
  m_branches->tau21 = new std::vector<float>;
  m_branches->c1 = new std::vector<float>;
  m_branches->c2 = new std::vector<float>;
  m_branches->c1_beta2 = new std::vector<float>;
  m_branches->c2_beta2 = new std::vector<float>;
  m_branches->d2 = new std::vector<float>;
  m_branches->d2_beta2 = new std::vector<float>;
}

SubstructureMomentBranches::~SubstructureMomentBranches()
{
  delete m_branches->tau21;
  delete m_branches->c1;
  delete m_branches->c2;
  delete m_branches->c1_beta2;
  delete m_branches->c2_beta2;
  delete m_branches->d2;
  delete m_branches->d2_beta2;

  delete m_branches;
}

void SubstructureMomentBranches::set_tree(TTree& output_tree) const {
  std::string prefix = "jet_substructure_moment_";
#define ADD_SIMPLE(nm) \
  output_tree.Branch((prefix + #nm).c_str(), &m_branches->nm)
  // basic kinematics
  ADD_SIMPLE(tau21);
  ADD_SIMPLE(c1);
  ADD_SIMPLE(c2);
  ADD_SIMPLE(c1_beta2);
  ADD_SIMPLE(c2_beta2);
  ADD_SIMPLE(d2);
  ADD_SIMPLE(d2_beta2);
#undef ADD_SIMPLE
}

void SubstructureMomentBranches::fill(const xAOD::Jet& jet) {

  // n-subjetieness
  fastjet::contrib::NormalizedCutoffMeasure normalized_measure(
    N_SUBJETIENESS_ALPHA, jet.getSizeParameter(), 1000000);
  fastjet::contrib::KT_Axes kt_axes;
  JetSubStructureUtils::Nsubjettiness tau1(1, kt_axes, normalized_measure);
  JetSubStructureUtils::Nsubjettiness tau2(2, kt_axes, normalized_measure);
  double tau21 = tau2.result(jet) / tau1.result(jet);
  m_branches->tau21->push_back(tau21);

  // ECF
  m_branches->c1->push_back(ec_c(1, 1, jet));
  m_branches->c2->push_back(ec_c(2, 1, jet));
  m_branches->c1_beta2->push_back(ec_c(1, 2, jet));
  m_branches->c2_beta2->push_back(ec_c(2, 2, jet));
  m_branches->d2->push_back(ec_d2(1, jet));
  m_branches->d2_beta2->push_back(ec_d2(2, jet));
}

void SubstructureMomentBranches::clear() {
  m_branches->tau21->clear();
  m_branches->c1->clear();
  m_branches->c2->clear();
  m_branches->c1_beta2->clear();
  m_branches->c2_beta2->clear();
  m_branches->d2->clear();
  m_branches->d2_beta2->clear();
}


// misc tools
namespace {
  JetSubStructureUtils::EnergyCorrelator ecf(int x, float beta){
    return JetSubStructureUtils::EnergyCorrelator(
      x, beta, JetSubStructureUtils::EnergyCorrelator::pt_R);
  }
  double ec_c(int n, float beta, const xAOD::Jet& jet) {
    return ecf(n + 1, beta).result(jet) * ecf(n - 1, beta).result(jet) /
      std::pow(ecf(n, beta).result(jet), 2.0);
  }
  double ec_d2(float beta, const xAOD::Jet& jet) {
    double ecf1 = ecf(1, beta).result(jet);
    double ecf2 = ecf(2, beta).result(jet);
    double ecf3 = ecf(3, beta).result(jet);
    return ecf3 * std::pow(ecf1, 3.0) / std::pow(ecf2, 3.0);
  }

}
