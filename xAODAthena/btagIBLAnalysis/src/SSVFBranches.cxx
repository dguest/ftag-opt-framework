#include "SSVFBranches.hh"
#include "SSVFBranchBuffer.hh"

#include "AthContainers/exceptions.h"
#include "TTree.h"

#include "xAODJet/Jet.h"

#include <string>
#include <stdexcept>

using xAOD::IParticle;

//!-----------------------------------------------------------------------------------------------------------------------------!//
SSVFBranches::SSVFBranches(bool isSV1):
  m_branches(new SSVFBranchBuffer)
{
  m_isSV1=isSV1;
  // instantiate all the vectors here ...
  m_branches->n2t      = new std::vector< int> ();
  m_branches->ntrkj    = new std::vector< int> ();
  m_branches->ntrkv    = new std::vector< int> ();
  m_branches->pt       = new std::vector< float> ();
  m_branches->m        = new std::vector< float> ();
  m_branches->efc      = new std::vector< float> ();
  m_branches->normdist = new std::vector< float> ();
  m_branches->pb       = new std::vector< float> ();
  m_branches->pc       = new std::vector< float> ();
  m_branches->pu       = new std::vector< float> ();
  m_branches->sig3d    = new std::vector< float> ();
  m_branches->llr      = new std::vector< float> ();
  m_branches->hasVtx   = new std::vector< int> ();
  m_branches->vtxX     = new std::vector< float> ();
  m_branches->vtxY     = new std::vector< float> ();
  m_branches->vtxZ     = new std::vector< float> ();
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
SSVFBranches::~SSVFBranches() {
  // delete all the vectors here ...
  delete m_branches->n2t;
  delete m_branches->ntrkj;
  delete m_branches->ntrkv;
  delete m_branches->pt;
  delete m_branches->m;
  delete m_branches->efc;
  delete m_branches->normdist;
  delete m_branches->pb;
  delete m_branches->pc;
  delete m_branches->pu;
  delete m_branches->sig3d;
  delete m_branches->llr;
  delete m_branches->vtxX;
  delete m_branches->vtxY;
  delete m_branches->vtxZ;
  delete m_branches;
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void SSVFBranches::set_tree(TTree& output_tree) const {
  std::string prefix = "jet_SV1_";
  if (!m_isSV1) prefix="jet_SV0_";
  output_tree.Branch( (prefix+"n2t").c_str()     , &m_branches->n2t     );
  output_tree.Branch( (prefix+"ntrkj").c_str()   , &m_branches->ntrkj   );
  output_tree.Branch( (prefix+"ntrkv").c_str()   , &m_branches->ntrkv   );
  output_tree.Branch( (prefix+"pt").c_str()      , &m_branches->pt      );
  output_tree.Branch( (prefix+"m").c_str()       , &m_branches->m       );
  output_tree.Branch( (prefix+"efc").c_str()     , &m_branches->efc     );
  output_tree.Branch( (prefix+"normdist").c_str(), &m_branches->normdist);
  if (m_isSV1) output_tree.Branch( (prefix+"pb").c_str()      , &m_branches->pb      );
  if (m_isSV1) output_tree.Branch( (prefix+"pc").c_str()      , &m_branches->pc      );
  if (m_isSV1) output_tree.Branch( (prefix+"pu").c_str()      , &m_branches->pu      );
  output_tree.Branch( (prefix+"sig3d").c_str()   , &m_branches->sig3d   );
  if (m_isSV1) output_tree.Branch( (prefix+"llr").c_str()     , &m_branches->llr     );
  output_tree.Branch( (prefix+"hasVtx").c_str()  , &m_branches->hasVtx  );
  output_tree.Branch( (prefix+"vtx_x").c_str()       , &m_branches->vtxX    );
  output_tree.Branch( (prefix+"vtx_y").c_str()       , &m_branches->vtxY    );
  output_tree.Branch( (prefix+"vtx_z").c_str()       , &m_branches->vtxZ    );
}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void SSVFBranches::fill(const xAOD::BTagging& bjet) {
  //m_branches->bH_pt->push_back(j_bH_pt);
  int ntrkj = -1;
  int ntrkv = -1;
  int n2t = -1;
  float pt = -99;
  float m = -99;
  float efc = -99;
  float ndist = -99;
  float sig3d = -99;
  float pb = -99;
  float pc = -99;
  float pu = -99;
  float llr = -99;
  int hasVtx=0;
  float vX=0;
  float vY=0;
  float vZ=0;

  if (m_isSV1) {
    // VD: check the existence of the vertex and only then fill the variables
    const std::vector<ElementLink<xAOD::VertexContainer > > SV1vertices = bjet.auxdata<std::vector<ElementLink<xAOD::VertexContainer > > >("SV1_vertices");
    if (SV1vertices.size() != 0) {
      bjet.taggerInfo(ntrkj, xAOD::SV1_NGTinJet);
      bjet.taggerInfo(ntrkv, xAOD::SV1_NGTinSvx);
      bjet.taggerInfo(n2t  , xAOD::SV1_N2Tpair );
      bjet.taggerInfo(m    , xAOD::SV1_masssvx );
      bjet.taggerInfo(efc  , xAOD::SV1_efracsvx);
      bjet.taggerInfo(ndist, xAOD::SV1_normdist);
      try {
	bjet.variable<float>("SV1", "significance3d" , sig3d);
      } catch(...) {
	std::cout << " ERROR: 3dsignificance not available for SV1 .... PLEASE CHECK!!!" << std::endl;
      }
      pb=bjet.SV1_pb();
      pc=bjet.SV1_pc();
      pu=bjet.SV1_pu();
      llr=bjet.SV1_loglikelihoodratio();
      hasVtx=true;
      if ( SV1vertices.at(0).isValid() ) {
	const xAOD::Vertex *tmpVertex = *(SV1vertices.at(0));
	vX=tmpVertex->x();
	vX=tmpVertex->y();
	vX=tmpVertex->z();
      }
      // NEED TO IMPLEMENT THE VECT SUM PT!!!!!
    }
  } else {
    const std::vector<ElementLink<xAOD::VertexContainer > > SV0vertices = bjet.auxdata<std::vector<ElementLink<xAOD::VertexContainer > > >("SV0_vertices");
    if (SV0vertices.size() != 0) {
      bjet.taggerInfo(ntrkj, xAOD::SV0_NGTinJet);
      bjet.taggerInfo(ntrkv, xAOD::SV0_NGTinSvx);
      bjet.taggerInfo(n2t  , xAOD::SV0_N2Tpair );
      bjet.taggerInfo(m    , xAOD::SV0_masssvx );
      bjet.taggerInfo(efc  , xAOD::SV0_efracsvx);
      bjet.taggerInfo(ndist, xAOD::SV0_normdist);
      sig3d=bjet.SV0_significance3D();
  
      hasVtx=true;
      if ( SV0vertices.at(0).isValid() ) {
	const xAOD::Vertex *tmpVertex = *(SV0vertices.at(0));
	vX=tmpVertex->x();
	vX=tmpVertex->y();
	vX=tmpVertex->z();
      }
    }
  }

  m_branches->n2t     ->push_back(n2t);
  m_branches->ntrkj   ->push_back(ntrkv);
  m_branches->ntrkv   ->push_back(ntrkj);
  m_branches->pt      ->push_back(pt);
  m_branches->m       ->push_back(m);
  m_branches->efc     ->push_back(efc);
  m_branches->normdist->push_back(ndist);
  m_branches->pb      ->push_back(pb);
  m_branches->pc      ->push_back(pc);
  m_branches->pu      ->push_back(pu);
  m_branches->sig3d   ->push_back(sig3d);
  m_branches->llr     ->push_back(llr);
  m_branches->hasVtx  ->push_back(hasVtx);
  m_branches->vtxX    ->push_back(vX);
  m_branches->vtxY    ->push_back(vY);
  m_branches->vtxZ    ->push_back(vZ);

}

//!-----------------------------------------------------------------------------------------------------------------------------!//
void SSVFBranches::clear() {
  // clear vectors
  m_branches->n2t->clear();
  m_branches->ntrkj->clear();
  m_branches->ntrkv->clear();
  m_branches->pt->clear();
  m_branches->m->clear();
  m_branches->efc->clear();
  m_branches->normdist->clear();
  m_branches->pb->clear();
  m_branches->pc->clear();
  m_branches->pu->clear();
  m_branches->sig3d->clear();
  m_branches->llr->clear();
  m_branches->hasVtx->clear();
  m_branches->vtxX->clear();
  m_branches->vtxY->clear();
  m_branches->vtxZ->clear();
}
