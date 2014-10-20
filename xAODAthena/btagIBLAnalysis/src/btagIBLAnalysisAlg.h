#ifndef BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
#define BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H 1

#include "AthenaBaseComps/AthAlgorithm.h"

#include "TFile.h"
#include "TTree.h"

class btagIBLAnalysisAlg: public ::AthAlgorithm { 
 public: 
  btagIBLAnalysisAlg( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~btagIBLAnalysisAlg(); 

  virtual StatusCode  initialize();
  virtual StatusCode  execute();
  virtual StatusCode  finalize();


 private: 

  bool isBHadron(int pdgid);

  TFile* output;
  TTree* tree;

  std::vector<float> *v_jet_pt;
  std::vector<float> *v_jet_eta;
  std::vector<float> *v_jet_phi;
}; 

#endif //> !BTAGIBLANALYSIS_BTAGIBLANALYSISALG_H
