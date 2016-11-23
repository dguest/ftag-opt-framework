#ifndef KSHORT_BRANCH_BUFFER_HH
#define KSHORT_BRANCH_BUFFER_HH

#include <vector>

struct KShortBranchBuffer {
  std::vector<int> *nKShort;

  std::vector<std::vector< int> > *kShort_origPdgid;

  std::vector<std::vector< float> > *kShort_px;
  std::vector<std::vector< float> > *kShort_py;
  std::vector<std::vector< float> > *kShort_pz;
  std::vector<std::vector< float> > *kShort_pt;
  std::vector<std::vector< float> > *kShort_eta;
  std::vector<std::vector< float> > *kShort_phi;
  std::vector<std::vector< float> > *kShort_E;
  std::vector<std::vector< float> > *kShort_x;
  std::vector<std::vector< float> > *kShort_y;
  std::vector<std::vector< float> > *kShort_z;
  std::vector<std::vector< float> > *kShort_Rxy;
  std::vector<std::vector< float> > *kShort_xProd;
  std::vector<std::vector< float> > *kShort_yProd;
  std::vector<std::vector< float> > *kShort_zProd;
  std::vector<std::vector< float> > *kShort_d0_truth;
  std::vector<std::vector< float> > *kShort_z0_truth;


};

#endif // KSHORT_BRANCH_BUFFER_HH
