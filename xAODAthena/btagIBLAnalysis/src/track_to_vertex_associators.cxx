#include "track_to_vertex_associators.hh"

#include "xAODBTagging/BTagging.h"
#include "xAODTracking/TrackParticle.h"

#include <string>

namespace {
  typedef std::vector<ElementLink<xAOD::BTagVertexContainer> > BTagVertices;
  typedef std::vector< ElementLink< xAOD::VertexContainer > > Vertices;

}

namespace trkvx {
  VxMap get_jf_map(const xAOD::BTagging& bj,
                   const std::string& name) {
    VxMap vx_map;
    const BTagVertices& vertices = bj.auxdata<BTagVertices>(
      name + "_JFvertices");
    for (size_t vtx_n = 0; vtx_n < vertices.size(); vtx_n++) {
      const auto& vtx = vertices.at(vtx_n);
      for (const auto& link: (*vtx)->track_links()) {
        const xAOD::TrackParticle* track = *link;
        vx_map[track] = vtx_n;
      }
    }
    return vx_map;
  }

  VxMap get_msv_map(const xAOD::BTagging& bj, const std::string& name) {
    VxMap vx_map;
    const Vertices& vertices = bj.auxdata<Vertices>(name + "_vertices");
    for (size_t vtx_n = 0; vtx_n < vertices.size(); vtx_n++) {
      const auto& vtx = vertices.at(vtx_n);
      for (const auto& link: (*vtx)->trackParticleLinks()) {
        const xAOD::TrackParticle* track = *link;
        vx_map[track] = vtx_n;
      }
    }
    return vx_map;
  }

}
