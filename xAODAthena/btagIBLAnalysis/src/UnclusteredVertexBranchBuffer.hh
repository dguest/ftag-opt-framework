#ifndef UNCLUSTEREDVERTEX_BRANCH_BUFFER_HH
#define UNCLUSTEREDVERTEX_BRANCH_BUFFER_HH

struct UnclusteredVertexBranchBuffer
{
  std::vector<std::vector<int> >* jf_vx_n;
  std::vector<std::vector<std::vector<float > > >* jf_vx_displacement;

};

#endif
