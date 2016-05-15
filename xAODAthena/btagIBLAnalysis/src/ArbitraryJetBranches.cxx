#include "ArbitraryJetBranches.hh"

#include "xAODJet/Jet.h"
#include "TTree.h"


ArbitraryJetBranches::ArbitraryJetBranches(
  const std::vector<std::string>& branch_names)
{
  for (const auto& nm: branch_names) {
    m_floats.emplace_back(nm, new std::vector<float>());
  }
}

ArbitraryJetBranches::~ArbitraryJetBranches()
{
  for (auto& br: m_floats) {
    delete br.second;
    br.second = 0;
  }
}

void ArbitraryJetBranches::set_tree(TTree& output_tree,
                                    const std::string& prefix) const {
  for (auto& br: m_floats) {
    // no fucking idea why I don't do `&br.second` here... but this
    // compiles
    output_tree.Branch((prefix + br.first).c_str(), br.second);
  }
}

void ArbitraryJetBranches::fill(const xAOD::Jet& jet) {
  const xAOD::BTagging *btag = jet.btagging();
  for (const auto& br: m_floats) {
    br.second->push_back(btag->auxdata<double>(br.first));
  }
}

void ArbitraryJetBranches::clear() {
  for (const auto& br: m_floats) {
    br.second->clear();
  }
}
