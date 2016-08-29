#ifndef SSVF_BRANCH_BUFFER_HH
#define SSVF_BRANCH_BUFFER_HH

#include <vector>

struct SSVFBranchBuffer {

  std::vector<int> *n2t;
  std::vector<int> *ntrkj;
  std::vector<int> *ntrkv;
  std::vector<float> *pt;
  std::vector<float> *m;
  std::vector<float> *efc;
  std::vector<float> *normdist;
  std::vector<float> *pb;
  std::vector<float> *pc;
  std::vector<float> *pu;
  std::vector<float> *sig3d;
  std::vector<float> *llr;
  std::vector<int>   *hasVtx;
  std::vector<float> *vtxX;
  std::vector<float> *vtxY;
  std::vector<float> *vtxZ;

};

#endif // SSVF_BRANCH_BUFFER_HH
