#ifndef SUBSTRUCTUREMOMENT_BRANCH_BUFFER_HH
#define SUBSTRUCTUREMOMENT_BRANCH_BUFFER_HH

#include <vector>

struct SubstructureMomentBranchBuffer {
  std::vector<float>* tau21;
  std::vector<float>* c1;
  std::vector<float>* c2;
  std::vector<float>* c1_beta2;
  std::vector<float>* c2_beta2;
  std::vector<float>* d2;
  std::vector<float>* d2_beta2;
};

#endif // SUBSTRUCTUREMOMENT_BRANCH_BUFFER_HH
