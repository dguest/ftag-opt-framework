#ifndef TRACK_COV_BRANCH_BUFFER_HH
#define TRACK_COV_BRANCH_BUFFER_HH

#include <vector>
#include <map>
#include <utility>

struct TrackCovBranchBuffer {
  // map of covariance branches
  std::map< std::pair<int, int>,  std::vector<std::vector<float> >*> cov;
};

#endif
