#include "TrackCovBranches.hh"
#include "TrackCovBranchBuffer.hh"

#include "xAODTracking/TrackParticle.h"
#include "TTree.h"

#include <string>
#include <map>

namespace {
  const std::vector<std::string> TRK_PAR = {
    "d0","z0", "phi", "theta", "qoverp"};
}

TrackCovBranches::TrackCovBranches():
  m_branches(new TrackCovBranchBuffer)
{
  typedef std::vector<std::vector<float> > VVF;
  size_t n_pars = TRK_PAR.size();
  for (size_t iii = 0; iii < n_pars; iii++) {
    for (size_t jjj = iii; jjj < n_pars; jjj++) {
      m_branches->cov[std::make_pair(iii,jjj)] = new VVF;
    }
  }
}

TrackCovBranches::~TrackCovBranches()
{
  for (auto& branch: m_branches->cov) {
    delete branch.second;
  }
  delete m_branches;
}

void TrackCovBranches::set_tree(TTree& output_tree,
                             const std::string& prefix) const {
  for (auto& pair: m_branches->cov) {
    std::string par1 = TRK_PAR.at(pair.first.first);
    std::string par2 = TRK_PAR.at(pair.first.second);
    std::string branch_name = prefix + "cov_" + par1 + par2;
    output_tree.Branch(branch_name.c_str(), &pair.second);
  }
}

void TrackCovBranches::fill(const TrackCovBranches::Tracks& tracks) {
  // add a vector to each branch
  for (auto& pair: m_branches->cov) {
    pair.second->push_back(std::vector<float>());
  }
  for (const auto* track: tracks) {
    const auto cov_matrix = track->definingParametersCovMatrix();
    for (auto& pair: m_branches->cov) {
      const auto& idx = pair.first;
      float val = cov_matrix(idx.first, idx.second);
      pair.second->back().push_back(val);
    }
  }
}

void TrackCovBranches::clear() {
  for (auto& pair: m_branches->cov) {
    pair.second->clear();
  }
}
