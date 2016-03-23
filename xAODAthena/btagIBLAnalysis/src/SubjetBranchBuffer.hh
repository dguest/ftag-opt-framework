#ifndef SUBJET_BRANCH_BUFFER_HH
#define SUBJET_BRANCH_BUFFER_HH

struct SubjetBranchBuffer
{
  std::vector<std::vector<float> >* pt;
  std::vector<std::vector<float> >* eta;
  std::vector<std::vector<float> >* phi;
  std::vector<std::vector<float> >* m;
  std::vector<std::vector<int> >* ntrk;

  std::vector<std::vector<float> >* mv2c20;

  // jetfitter
  std::vector<std::vector<float> >* jf_m;
  std::vector<std::vector<float> >* jf_mUncorr;
  std::vector<std::vector<float> >* jf_efc;
  std::vector<std::vector<float> >* jf_deta;
  std::vector<std::vector<float> >* jf_dphi;
  std::vector<std::vector<float> >* jf_dRFlightDir;
  std::vector<std::vector<int> >*   jf_ntrkAtVx;
  std::vector<std::vector<int> >*   jf_nvtx;
  std::vector<std::vector<float> >* jf_sig3d;
  std::vector<std::vector<int> >*   jf_nvtx1t;
  std::vector<std::vector<int> >*   jf_n2t;
  std::vector<std::vector<int> >*   jf_VTXsize;
  // TODO: Missing some clustered vertex information here

  // TODO: figure out what these variables are all about
  // std::vector<std::vector<float> >* jf_phi;
  // std::vector<std::vector<float> >* jf_theta;

  // IP3D
  std::vector<std::vector<float> >* ip3d_pb;
  std::vector<std::vector<float> >* ip3d_pc;
  std::vector<std::vector<float> >* ip3d_pu;
  std::vector<std::vector<int> >* ip3d_ntrk;

  // SV1
  // std::vector<std::vector<float> >* sv1_ntrkj; // this one is garbage
  std::vector<std::vector<int> >* sv1_ntrkv;
  std::vector<std::vector<int> >*   sv1_n2t;
  std::vector<std::vector<float> >* sv1_m;
  std::vector<std::vector<float> >* sv1_efc;
  std::vector<std::vector<int> >*   sv1_Nvtx;
  std::vector<std::vector<float> >* sv1_normdist;

};

#endif
