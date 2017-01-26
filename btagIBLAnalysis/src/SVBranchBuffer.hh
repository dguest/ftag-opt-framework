#ifndef SV_BRANCH_BUFFER_HH
#define SV_BRANCH_BUFFER_HH

#include "xAODTracking/VertexContainer.h"

#include <vector>
#include <string>

struct SVAccessors {
  typedef std::vector<ElementLink<xAOD::VertexContainer > > Vertices;
  SG::AuxElement::ConstAccessor<Vertices> vertices;
  SG::AuxElement::ConstAccessor<int> NGTinJet;
  SG::AuxElement::ConstAccessor<int> NGTinSvx;
  SG::AuxElement::ConstAccessor<int> N2Tpair;
  SG::AuxElement::ConstAccessor<float> masssvx;
  SG::AuxElement::ConstAccessor<float> efracsvx;
  SG::AuxElement::ConstAccessor<float> normdist;
  SG::AuxElement::ConstAccessor<float> significance3D;

  SVAccessors(const std::string& prefix):
#define SET_PREFIX(name) name(prefix + "_" #name)
    SET_PREFIX(vertices),
    SET_PREFIX(NGTinJet),
    SET_PREFIX(NGTinSvx),
    SET_PREFIX(N2Tpair),
    SET_PREFIX(masssvx),
    SET_PREFIX(efracsvx),
    SET_PREFIX(normdist),
    SET_PREFIX(significance3D)
    {}
#undef SET_PREFIX

};

struct SVBranchBuffer {

  std::vector<int>* NVtx;
  std::vector<int>* NGTinJet;
  std::vector<int>* NGTinSvx;
  std::vector<int>* N2Tpair;
  std::vector<float>* masssvx;
  std::vector<float>* efracsvx;
  std::vector<float>* normdist;
  std::vector<float>* significance3D;

};

#endif // SV_BRANCH_BUFFER_HH
