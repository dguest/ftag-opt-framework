#ifndef TRACK_TO_VERTEX_ASSOCIATORS_HH
#define TRACK_TO_VERTEX_ASSOCIATORS_HH

namespace xAOD {
  class BTagging_v1;
  typedef BTagging_v1 BTagging;
  class TrackParticle_v1;
  typedef TrackParticle_v1 TrackParticle;
}

#include <map>
#include <string>

namespace trkvx {
  // map from the track particle to the vertex index
  typedef std::map<const xAOD::TrackParticle*, int> VxMap;

  VxMap get_jf_map(const xAOD::BTagging& btagging,
                   const std::string& name = "JetFitter");
  VxMap get_msv_map(const xAOD::BTagging& btagging,
                    const std::string& name = "MSV");
}

#endif
