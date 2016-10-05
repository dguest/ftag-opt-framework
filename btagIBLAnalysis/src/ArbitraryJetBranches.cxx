#include "ArbitraryJetBranches.hh"

#include "xAODJet/Jet.h"
#include "TTree.h"


ArbitraryJetBranches::ArbitraryJetBranches(
  const std::vector<std::string>& double_branch_names,
  const std::vector<std::string>& float_vec_branches)
{
  for (const auto& nm: double_branch_names) {
    m_doubles.emplace_back(nm, new std::vector<double>());
  }
  for (const auto& nm: float_vec_branches) {
    m_vec_floats.emplace_back(nm, new VecVecF());
  }
}

ArbitraryJetBranches::~ArbitraryJetBranches()
{
  for (auto& br: m_doubles) {
    delete br.second;
    br.second = 0;
  }
  for (auto& br: m_vec_floats) {
    delete br.second;
    br.second = 0;
  }
}

void ArbitraryJetBranches::set_tree(TTree& output_tree,
                                    const std::string& prefix) const {
  for (auto& br: m_doubles) {
    // no fucking idea why I don't do `&br.second` here... but this
    // compiles
    output_tree.Branch((prefix + br.first).c_str(), br.second);
  }
  for (auto& br: m_vec_floats) {
    output_tree.Branch((prefix + br.first).c_str(), br.second);
  }
}

void ArbitraryJetBranches::fill(const xAOD::Jet& jet) {
  const xAOD::BTagging *btag = jet.btagging();
  for (const auto& br: m_doubles) {
    br.second->push_back(btag->auxdata<double>(br.first));
  }
  for (const auto& br: m_vec_floats) {
    br.second->push_back(btag->auxdata<std::vector<float> >(br.first));
  }
}

void ArbitraryJetBranches::clear() {
  for (const auto& br: m_doubles) {
    br.second->clear();
  }
  for (const auto& br: m_vec_floats) {
    br.second->clear();
  }
}
