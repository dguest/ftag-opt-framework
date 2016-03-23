#ifndef CLUSTER_BRANCH_BUFFER_HH
#define CLUSTER_BRANCH_BUFFER_HH

#include <vector>

struct ClusterBranchBuffer {
  std::vector<std::vector<float> >* pt;
  std::vector<std::vector<float> >* eta;
  std::vector<std::vector<float> >* phi;
  std::vector<std::vector<float> >* e;

  std::vector<std::vector<unsigned int> >* clusterSize;
  std::vector<std::vector<float> >* ISOLATION;
  std::vector<std::vector<float> >* LATERAL;
  std::vector<std::vector<float> >* LONGITUDINAL;
  std::vector<std::vector<float> >* SECOND_LAMBDA;
  std::vector<std::vector<float> >* SECOND_R;
  std::vector<std::vector<float> >* CENTER_LAMBDA;
  std::vector<std::vector<float> >* CENTER_MAG;
  std::vector<std::vector<float> >* ENG_POS;
  std::vector<std::vector<float> >* EM_PROBABILITY;
  std::vector<std::vector<float> >* ENG_FRAC_MAX;
  std::vector<std::vector<float> >* FIRST_ENG_DENS;
};

#endif // CLUSTER_BRANCH_BUFFER_HH
