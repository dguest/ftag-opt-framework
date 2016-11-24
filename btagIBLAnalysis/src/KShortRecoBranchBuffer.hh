#ifndef KSHORTRECO_BRANCH_BUFFER_HH
#define KSHORTRECO_BRANCH_BUFFER_HH

#include <vector>

struct KShortRecoBranchBuffer {
  std::vector<int>                  *nKShortReco;
  std::vector<std::vector< int> >   *kShortReco_ndof;
  std::vector<std::vector< float> > *kShortReco_px;
  std::vector<std::vector< float> > *kShortReco_py;
  std::vector<std::vector< float> > *kShortReco_pz;
  std::vector<std::vector< float> > *kShortReco_pt;
  std::vector<std::vector< float> > *kShortReco_ptErr;
  std::vector<std::vector< float> > *kShortReco_x;
  std::vector<std::vector< float> > *kShortReco_y;
  std::vector<std::vector< float> > *kShortReco_z;
  std::vector<std::vector< float> > *kShortReco_Rxy;
  std::vector<std::vector< float> > *kShortReco_RxyErr;
  std::vector<std::vector< float> > *kShortReco_chi2;
  std::vector<std::vector< float> > *kShortReco_kShortMass;
  std::vector<std::vector< float> > *kShortReco_kShortMassErr;
  std::vector<std::vector< float> > *kShortReco_d0;
  std::vector<std::vector< float> > *kShortReco_z0;

  std::vector<int>                  *nKShortRecoGood;
  std::vector<std::vector< int> >   *kShortRecoGood_ndof;
  std::vector<std::vector< float> > *kShortRecoGood_px;
  std::vector<std::vector< float> > *kShortRecoGood_py;
  std::vector<std::vector< float> > *kShortRecoGood_pz;
  std::vector<std::vector< float> > *kShortRecoGood_pt;
  std::vector<std::vector< float> > *kShortRecoGood_ptErr;
  std::vector<std::vector< float> > *kShortRecoGood_x;
  std::vector<std::vector< float> > *kShortRecoGood_y;
  std::vector<std::vector< float> > *kShortRecoGood_z;
  std::vector<std::vector< float> > *kShortRecoGood_Rxy;
  std::vector<std::vector< float> > *kShortRecoGood_RxyErr;
  std::vector<std::vector< float> > *kShortRecoGood_chi2;
  std::vector<std::vector< float> > *kShortRecoGood_kShortMass;
  std::vector<std::vector< float> > *kShortRecoGood_kShortMassErr;
  std::vector<std::vector< float> > *kShortRecoGood_d0;
  std::vector<std::vector< float> > *kShortRecoGood_z0;




};

#endif // KSHORTRECO_BRANCH_BUFFER_HH
