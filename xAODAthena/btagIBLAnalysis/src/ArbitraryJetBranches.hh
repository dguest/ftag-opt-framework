#ifndef ARBITRARY_JET_BRANCHES_HH
#define ARBITRARY_JET_BRANCHES_HH

class TTree;

namespace xAOD {
  class Jet_v1;
  typedef Jet_v1 Jet;
}

#include <vector>
#include <string>
#include <utility>

class ArbitraryJetBranches
{
public:
  // might want to add a prefix to the constructor for the tree branches
  ArbitraryJetBranches(const std::vector<std::string>& float_branches);
  ~ArbitraryJetBranches();

  // disable copying and assignment
  ArbitraryJetBranches& operator=(ArbitraryJetBranches) = delete;
  ArbitraryJetBranches(const ArbitraryJetBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const xAOD::Jet& jet);
  void clear();
private:
  std::vector<std::pair<std::string, std::vector<float>*  > > m_floats;
};

#endif // ARBITRARY_JET_BRANCHES_HH
