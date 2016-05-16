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
  ArbitraryJetBranches(const std::vector<std::string>& double_branches,
                       const std::vector<std::string>& float_vec_branches);
  ~ArbitraryJetBranches();

  // disable copying and assignment
  ArbitraryJetBranches& operator=(ArbitraryJetBranches) = delete;
  ArbitraryJetBranches(const ArbitraryJetBranches&) = delete;

  void set_tree(TTree& output_tree, const std::string& prefix) const;
  void fill(const xAOD::Jet& jet);
  void clear();
private:
  typedef std::vector<std::vector<float> > VecVecF;
  std::vector<std::pair<std::string, std::vector<double>*  > > m_doubles;
  std::vector<std::pair<std::string, VecVecF* > > m_vec_floats;
};

#endif // ARBITRARY_JET_BRANCHES_HH
