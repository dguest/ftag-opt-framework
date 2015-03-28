#define TrackPlotting_cxx
#include "TrackPlotting.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <math.h>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <dirent.h>

#include "TLorentzVector.h"

using namespace std;

std::string outPutName="a.root";

bool HistoSet::FillHistos(int jIndex, int tIndex, TrackPlotting* baseTree) {
  bool newJet=false;
  if (m_jIndex!=jIndex || m_event!=baseTree->eventnb) newJet=true;
  
  if (newJet) {
    // finally put in previous value
    if (m_event!=0) {
      (m_1Dhist.at(N_TRK))->Fill( m_counter );
      (m_2Dhist.at(N_TRK_JPT))->Fill( m_pt/1e3, m_counter );
      if ( m_Lxy>0 ) (m_2Dhist.at(N_TRK_BLXY))->Fill( m_Lxy, m_counter );
    }
    m_jIndex =jIndex;
    m_event  =baseTree->eventnb;
    m_counter=0;
    m_pt=baseTree->jet_pt->at(m_jIndex);
    m_Lxy=baseTree->bH_Lxy->at(m_jIndex);
  } else {
    m_counter++;
  }

  (m_1Dhist.at(TRK_PT))->Fill( (baseTree->jet_trk_pt->at(jIndex)).at(tIndex)/1e3 );
  (m_1Dhist.at(TRK_PT_JPT_R))->Fill( (baseTree->jet_trk_pt->at(jIndex)).at(tIndex)/baseTree->jet_pt->at(jIndex) ); 
  (m_1Dhist.at(TRK_ETA))->Fill( (baseTree->jet_trk_eta->at(jIndex)).at(tIndex) );
  (m_1Dhist.at(TRK_ORIG))->Fill( (baseTree->jet_trk_orig->at(jIndex)).at(tIndex) );
  (m_1Dhist.at(TRK_ALGO))->Fill( (baseTree->jet_trk_algo->at(jIndex)).at(tIndex) );
  (m_1Dhist.at(TRK_NIBL))->Fill( (baseTree->jet_trk_nBLHits->at(jIndex)).at(tIndex) );
  (m_1Dhist.at(TRK_NPIX))->Fill( (baseTree->jet_trk_nPixHits->at(jIndex)).at(tIndex) );
  (m_1Dhist.at(TRK_NSI))->Fill( (baseTree->jet_trk_nPixHits->at(jIndex)).at(tIndex)+(baseTree->jet_trk_nSCTHits->at(jIndex)).at(tIndex) );
  //cout << " ... jIndex: " << jIndex << "  tIndex: " << tIndex << " .. all: " << baseTree->jet_btag_ntrk->at(jIndex) << endl;
  //(m_1Dhist.at(TRK_D0))->Fill( (baseTree->jet_trk_d0->at(jIndex)).at(tIndex) );
  //(m_1Dhist.at(TRK_Z0))->Fill( (baseTree->jet_trk_z0->at(jIndex)).at(tIndex)-baseTree->PVz-7.3  );
  //(m_1Dhist.at(TRK_Z0ST))->Fill( (baseTree->jet_trk_z0->at(jIndex)).at(tIndex)*sin((baseTree->jet_trk_theta->at(jIndex)).at(tIndex))-baseTree->PVz-7.3 );
  
  //(m_1Dhist.at(TRK_IP3D_D0))->Fill( (baseTree->jet_trk_ip3d_d0->at(jIndex)).at(tIndex) );
  //(m_1Dhist.at(TRK_IP3D_Z0))->Fill( (baseTree->jet_trk_ip3d_z0->at(jIndex)).at(tIndex) );
  (m_1Dhist.at(TRK_IP3D_Z0ST))->Fill( (baseTree->jet_trk_ip3d_z0->at(jIndex)).at(tIndex)*sin((baseTree->jet_trk_theta->at(jIndex)).at(tIndex)) );
  (m_1Dhist.at(TRK_IP3D_GRADE))->Fill( (baseTree->jet_trk_ip3d_grade->at(jIndex)).at(tIndex) );

  (m_2Dhist.at(TRK_PT_JPT))->Fill( baseTree->jet_pt->at(jIndex)/1e3,
				   (baseTree->jet_trk_pt->at(jIndex)).at(tIndex)/1e3); 
  
  (m_2Dhist.at(IP3D_GRADE_JPT))->Fill( baseTree->jet_pt->at(jIndex)/1e3,
					 (baseTree->jet_trk_ip3d_grade->at(jIndex)).at(tIndex) ); 
  (m_2Dhist.at(IP3D_GRADE_BLXY))->Fill( baseTree->bH_Lxy->at(jIndex),
					(baseTree->jet_trk_ip3d_grade->at(jIndex)).at(tIndex) ); 
  
  (m_2Dhist.at(TRK_ORIG_GRADE_JPT))->Fill( baseTree->jet_pt->at(jIndex)/1e3,
					   (baseTree->jet_trk_orig->at(jIndex)).at(tIndex) ); 
  (m_2Dhist.at(TRK_ORIG_GRADE_BLXY))->Fill( baseTree->bH_Lxy->at(jIndex),
					   (baseTree->jet_trk_orig->at(jIndex)).at(tIndex) ); 
  
  return true;
}

void HistoSet::WriteHistosToFile(TFile* myFile) {
  if (myFile==0) {
    cout << " ... genious, you provided me with a corrupted file " << endl;
    exit(-1);
  }
  
  /// put here the UF/OF ... tricky for 2D histos ...
  TDirectory *dir = myFile->mkdir( m_name.c_str() );
  dir->cd();
  for (unsigned int i=0; i<m_1Dhist.size(); i++) {
    TH1D* den=m_1Dhist.at(i);
    den->SetBinContent(1, den->GetBinContent(0)+den->GetBinContent(1));
    den->SetBinError(1, sqrt(pow(den->GetBinError(0),2)+pow(den->GetBinError(1),2)));
    den->SetBinContent(0, 0.0);
    den->SetBinError(0, 0.0);
    den->SetBinContent(den->GetNbinsX(), den->GetBinContent(den->GetNbinsX())+den->GetBinContent(den->GetNbinsX()+1));
    den->SetBinError(den->GetNbinsX(), sqrt(pow(den->GetBinError(den->GetNbinsX()),2)+pow(den->GetBinError(den->GetNbinsX()+1),2)));
    den->SetBinContent(den->GetNbinsX()+1, 0.0);
    den->SetBinError(den->GetNbinsX()+1, 0.0);
    dir->WriteObject( m_1Dhist.at(i), m_1Dhist.at(i)->GetName() );
  }
  for (unsigned int i=0; i<m_2Dhist.size(); i++) {
    dir->WriteObject( m_2Dhist.at(i), m_2Dhist.at(i)->GetName() );
  }
}

void HistoSet::Initialize() {
  m_jIndex=0;
  m_event=0;
  m_counter=0;
  
  m_1Dhist.push_back( new TH1D("n_trk"   ,"n_trk"   , 40, -0.5,39.5) );
  m_1Dhist.push_back( new TH1D("trk_pt"  ,"trk_pt"  ,500, 0,50) );
  m_1Dhist.push_back( new TH1D("trk_pt_jpt"  ,"trk_pt_jpt"  ,1000, 0,0.3) );
  m_1Dhist.push_back( new TH1D("trk_eta" ,"trk_eta" ,100, -2.5, 2.5) );
  m_1Dhist.push_back( new TH1D("trk_orig","trk_orig", 5, -1.5, 3.5) );
  m_1Dhist.push_back( new TH1D("trk_algo","trk_algo", 60, -0.5, 59.5) );
  m_1Dhist.push_back( new TH1D("trk_nIBL","trk_nIBL", 3 , -0.5, 2.5) );
  m_1Dhist.push_back( new TH1D("trk_nPix","trk_nPix", 10, -0.5, 9.5) );
  m_1Dhist.push_back( new TH1D("trk_nSi","trk_nSi"  , 20, -0.5,19.5) );
  m_1Dhist.push_back( new TH1D("trk_d0" ,"trk_d0"   , 200, -6, 6) );
  m_1Dhist.push_back( new TH1D("trk_z0" ,"trk_z0"   , 200, -6, 6) );
  m_1Dhist.push_back( new TH1D("trk_z0st" ,"trk_z0st"   , 200, -6, 6) );
  m_1Dhist.push_back( new TH1D("trk_ip3d_d0" ,"trk_ip3d_d0"   , 200, -6,6) );
  m_1Dhist.push_back( new TH1D("trk_ip3d_z0" ,"trk_ip3d_z0"   , 200, -6,6) );
  m_1Dhist.push_back( new TH1D("trk_ip3d_z0st","trk_ip3d_z0st"   , 200, -6,6) );
  m_1Dhist.push_back( new TH1D("trk_ip3D_grade" ,"trk_ip3d_grade"   ,  15, -1.5, 13.5) );//8, -1.5, 6.5) );

  m_2Dhist.push_back( new TH2D("n_trk_jpt" , "n_trk_jpt" , 100,0, 1000,30,0,30) );
  m_2Dhist.push_back( new TH2D("n_trk_blxy", "n_trk_blxy", 100,0, 100 ,30,0,30) );
  m_2Dhist.push_back( new TH2D("jet_pt_VS_trk_pt","jet_pt_VS_trk_pt"  ,100,0,1000,500,0,50) );
  m_2Dhist.push_back( new TH2D("jet_pt_VS_ip3d_grade","jet_pt_VS_ip3d_grade"  ,100,0,1000,  15, -1.5, 13.5) );//8, -1.5, 6.5) );
  m_2Dhist.push_back( new TH2D("blxy_VS_ip3d_grade","blxy_VS_ip3d_grade"  ,50,0,100,  15, -1.5, 13.5) );//8, -1.5, 6.5) );
  m_2Dhist.push_back( new TH2D("jet_pt_VS_trk_orig","jet_pt_VS_trk_orig"  ,100,0,1000, 5, -1.5, 3.5) );
  m_2Dhist.push_back( new TH2D("blxy_VS_trk_orig","blxy_VS_trk_orig"  ,50,0,100, 5, -1.5, 3.5) );


  for (unsigned int i=0; i<m_1Dhist.size(); i++) m_1Dhist.at(i)->Sumw2();
  for (unsigned int i=0; i<m_2Dhist.size(); i++) m_2Dhist.at(i)->Sumw2();
  
  if (m_1Dhist.size()!= LAST ) {
    cout << "PROBLEM: inconsistency in the number of histograms: " << m_1Dhist.size() << " != " << LAST << "   ...... please resolve this!!!!" << endl;
    exit(-1);
  }
  if (m_2Dhist.size()!= LAST2D ) {
    cout << "PROBLEM: inconsistency in the number of 2D histograms: " << m_2Dhist.size() << " != " << LAST2D << "   ...... please resolve this!!!!" << endl;
    exit(-1);
  }
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
float deltaR(float eta1, float eta2, float phi1, float phi2) {
  float DEta=fabs( eta1-eta2 );
  float DPhi=acos(cos( fabs( phi1-phi2 ) ) );
  return sqrt( pow(DEta,2)+pow(DPhi,2) );
}

/////////////////////////////////////////////////////////////////////////////////////
bool TrackPlotting::SelectJet(int jIndex) {
  if (jet_truthMatch  ->at(jIndex)!=1)  return false;
  if (jet_aliveAfterOR->at(jIndex)!=1)  return false;
  if (jet_pt->at(jIndex)<25e3)          return false;
  return true;
}

/////////////////////////////////////////////////////////////////////////////////////
bool TrackPlotting::SelectTrack(int jIndex, int tIndex) {

}


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
void TrackPlotting::Loop()
{
  if (fChain == 0) return;
  
  Long64_t nentries = fChain->GetEntries();
  vector<HistoSet*> histos;
  
  HistoSet* AllTracks   =new HistoSet("AllTracks");    histos.push_back(AllTracks);
  HistoSet* AllTracks_B =new HistoSet("AllTracksB");   histos.push_back(AllTracks_B);
  HistoSet* AllTracks_B_fromB =new HistoSet("AllTracksB_fromB");   histos.push_back(AllTracks_B_fromB);
  HistoSet* AllTracks_B_fromF =new HistoSet("AllTracksB_fromF");   histos.push_back(AllTracks_B_fromF);
  HistoSet* AllTracks_B_other =new HistoSet("AllTracksB_other");   histos.push_back(AllTracks_B_other);

  HistoSet* AllTracks_C =new HistoSet("AllTracksC");   histos.push_back(AllTracks_C);
  HistoSet* AllTracks_L =new HistoSet("AllTracksL");   histos.push_back(AllTracks_L);
  HistoSet* IP3DTracks  =new HistoSet("IP3DTracks");   histos.push_back(IP3DTracks);
  HistoSet* IP3DTracks_B=new HistoSet("IP3DTracksB");  histos.push_back(IP3DTracks_B);
  HistoSet* IP3DTracks_Bspe=new HistoSet("IP3DTracksBspe");  histos.push_back(IP3DTracks_Bspe);
  HistoSet* IP3DTracks_Bs=new HistoSet("IP3DTracksBs");  histos.push_back(IP3DTracks_Bs);
  HistoSet* IP3DTracks_Bb=new HistoSet("IP3DTracksBb");  histos.push_back(IP3DTracks_Bb);
  HistoSet* IP3DTracks_Bf=new HistoSet("IP3DTracksBf");  histos.push_back(IP3DTracks_Bf);
  HistoSet* IP3DTracks_C=new HistoSet("IP3DTracksC");  histos.push_back(IP3DTracks_C);
  HistoSet* IP3DTracks_L=new HistoSet("IP3DTracksL");  histos.push_back(IP3DTracks_L);
  HistoSet* IP3DTracks_Lspe=new HistoSet("IP3DTracksLspe");  histos.push_back(IP3DTracks_Lspe);
  HistoSet* IP3DTracks_Ls=new HistoSet("IP3DTracksLs");  histos.push_back(IP3DTracks_Ls);
  HistoSet* IP3DTracks_Lf=new HistoSet("IP3DTracksLf");  histos.push_back(IP3DTracks_Lf);

  HistoSet* IP2DTracks  =new HistoSet("IP2DTracks");   histos.push_back(IP2DTracks);
  HistoSet* IP2DTracks_B=new HistoSet("IP2DTracksB");  histos.push_back(IP2DTracks_B);
  HistoSet* IP2DTracks_Bs=new HistoSet("IP2DTracksBs");  histos.push_back(IP2DTracks_Bs);
  HistoSet* IP2DTracks_Bb=new HistoSet("IP2DTracksBb");  histos.push_back(IP2DTracks_Bb);
  HistoSet* IP2DTracks_Bf=new HistoSet("IP2DTracksBf");  histos.push_back(IP2DTracks_Bf);
  HistoSet* IP2DTracks_C=new HistoSet("IP2DTracksC");  histos.push_back(IP2DTracks_C);
  HistoSet* IP2DTracks_L=new HistoSet("IP2DTracksL");  histos.push_back(IP2DTracks_L);
  HistoSet* IP2DTracks_Ls=new HistoSet("IP2DTracksLs");  histos.push_back(IP2DTracks_Ls);
  HistoSet* IP2DTracks_Lf=new HistoSet("IP2DTracksLf");  histos.push_back(IP2DTracks_Lf);
   

  HistoSet* SV1Tracks  =new HistoSet("SV1Tracks");   histos.push_back(SV1Tracks);
  HistoSet* SV1Tracks_B=new HistoSet("SV1TracksB");  histos.push_back(SV1Tracks_B);
  HistoSet* SV1Tracks_C=new HistoSet("SV1TracksC");  histos.push_back(SV1Tracks_C);
  HistoSet* SV1Tracks_L=new HistoSet("SV1TracksL");  histos.push_back(SV1Tracks_L);
 
  
  HistoSet* JFTracks  =new HistoSet("JFTracks");   histos.push_back(JFTracks);
  HistoSet* JFTracks_B=new HistoSet("JFTracksB");  histos.push_back(JFTracks_B);
  HistoSet* JFTracks_C=new HistoSet("JFTracksC");  histos.push_back(JFTracks_C);
  HistoSet* JFTracks_L=new HistoSet("JFTracksL");  histos.push_back(JFTracks_L);


  for (unsigned int i=0; i<histos.size(); i++) {
    histos[i]->Initialize();
  }

  counter=0;
  //nentries=40000;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    counter++;
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    if (jentry%1000==0) cout << "processing: " << jentry << " ......." << endl;

    for (int j=0; j<njets; j++) {
      if ( !SelectJet(j) ) continue;
      //cout << "Jet: " << j << " with Flavour: " << jet_truthflav->at(j) << endl;
      
      int nTracks=jet_btag_ntrk->at(j);
      for (int t=0; t<nTracks; t++) {
	int orig=(jet_trk_orig->at(j)).at(t);
	AllTracks->FillHistos(j, t, this);
	//if (jet_truthflav->at(j)==5) {
	if (jet_GhostL_HadF->at(j)==5) {
	  AllTracks_B->FillHistos(j, t, this);
	  if (orig==0 || orig==1) AllTracks_B_fromB->FillHistos(j, t, this);
	  else if (orig==2)       AllTracks_B_fromF->FillHistos(j, t, this);
	  else                    AllTracks_B_other->FillHistos(j, t, this);
	}
	if (jet_GhostL_HadF->at(j)==4) AllTracks_C->FillHistos(j, t, this);
	if (jet_GhostL_HadF->at(j)==0) AllTracks_L->FillHistos(j, t, this);
	
	int algo=(jet_trk_algo->at(j)).at(t);
	if ( (algo&(1<<1))!=0 ) {
	  IP3DTracks->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==5) {
	    IP3DTracks_B->FillHistos(j, t, this);
	    if (orig==0 || orig==1) IP3DTracks_Bb->FillHistos(j, t, this);
	    else if (orig==2)       IP3DTracks_Bf->FillHistos(j, t, this);
	    else                    IP3DTracks_Bs->FillHistos(j, t, this);
	    if ( (jet_trk_pt->at(j)).at(t)>(1+2/100e3*jet_pt->at(j))*1e3 )  IP3DTracks_Bspe->FillHistos(j, t, this);
	  }
	  if (jet_GhostL_HadF->at(j)==4) IP3DTracks_C->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==0) {
	    IP3DTracks_L->FillHistos(j, t, this);
	    if (orig==2) IP3DTracks_Bf->FillHistos(j, t, this);
	    else         IP3DTracks_Bs->FillHistos(j, t, this);
	    if ( (jet_trk_pt->at(j)).at(t)>(1+2/100e3*jet_pt->at(j))*1e3 ) IP3DTracks_Lspe->FillHistos(j, t, this);
	  }
	}

	if ( (algo&(1<<0))!=0 ) {
	  IP2DTracks->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==5) {
	    IP2DTracks_B->FillHistos(j, t, this);
	    if (orig==0 || orig==1) IP2DTracks_Bb->FillHistos(j, t, this);
	    else if (orig==2)       IP2DTracks_Bf->FillHistos(j, t, this);
	    else                    IP2DTracks_Bs->FillHistos(j, t, this);
	  }
	  if (jet_GhostL_HadF->at(j)==4) IP2DTracks_C->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==0) {
	    IP2DTracks_L->FillHistos(j, t, this);
	    if (orig==2) IP2DTracks_Bf->FillHistos(j, t, this);
	    else         IP2DTracks_Bs->FillHistos(j, t, this);
	  }
	}

	if ( (algo&(1<<3))!=0 ) {
	  SV1Tracks->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==5) SV1Tracks_B->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==4) SV1Tracks_C->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==0) SV1Tracks_L->FillHistos(j, t, this);
	}
	if ( (algo&(1<<4))!=0 ) {
	  JFTracks->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==5) JFTracks_B->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==4) JFTracks_C->FillHistos(j, t, this);
	  if (jet_GhostL_HadF->at(j)==0) JFTracks_L->FillHistos(j, t, this);
	}
      }

    } // jet loop
  } // event loop
  
  TFile* output=new TFile(outPutName.c_str(),"recreate");
  for (unsigned int i=0; i<histos.size(); i++) {
    histos[i]->WriteHistosToFile(output);
  }
  output->Close();
}






/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> fileToStrVec(string fileList) {
  static const int MAX_LINE_LENGTH = 15000;
  char m_lineBuffer[MAX_LINE_LENGTH];

  std::ifstream m_file;
  m_file.open(fileList.c_str());
  if(!m_file) {
    std::cerr << "Error: could not open " << fileList << std::endl;
    exit(-1);
  }
  
  /////////////////////////////////////////////
  std::vector<std::string> fileContents;
  string line;
  while(!m_file.eof() ) {
    getline(m_file,line);
    //m_file.getline(line);
    if ( line.find("root")!=string::npos ) 
      fileContents.push_back(line);
  }
  return fileContents;

}

/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
//###############################################################################
int main(int argc, char* argv[]){  
  //SetAtlasStyle(); 
  gROOT->ProcessLine("#include <vector>");
  gROOT->ProcessLine(".L myFile.C+");

  bool usePseudo=false;

  string path="";
  string fileList="";
  string mcString="";
  bool special=false;
  if (argc>=2) {
    cout << endl;
    cout << " Reading input from fileList: " << fileList << endl;
    fileList=string(argv[1]);
  } else {
    cout << "NOT enough arguments" << endl;
    exit(-1);
  }
  if (argc>=3) {
    outPutName=string(argv[2]);
  }
  
  TChain* fChain;
  vector<string> listFiles;
  
  if (fileList=="") {
    cout << "WTF file are you giving me!!!  ... BYE" << endl;
    exit(-1);
  } else if (fileList.find("root")!=string::npos) {
    cout << "Reading file: " << fileList << endl;
    listFiles.push_back(fileList);
  } else if (fileList.find("txt")!=string::npos) {    
    cout << "Reading fileList from file: " << fileList << endl;
    std::vector<std::string> files;
    files=fileToStrVec(fileList);
    for (unsigned iF=0; iF<files.size(); iF++) listFiles.push_back(files[iF]);
  } else {
    cout << "Input is a directory: reading all the files " << endl;
    DIR*     dir;
    dirent*  pdir;
    dir = opendir( fileList.c_str() );     // open current directory
    while (pdir = readdir(dir))  {
      string foldName=pdir->d_name;
      if (foldName.find("root")!=string::npos) {
	listFiles.push_back( (fileList+"/"+foldName).c_str() );
	continue;
      }
      if (foldName.find("mc")==string::npos) continue;
      cout << pdir->d_name << endl;
      DIR*     dir2;
      dirent*  pdir2;
      dir2 = opendir( (fileList+"/"+foldName).c_str() );  // open current directory
      while (pdir2 = readdir(dir2))  {
	string fName=pdir2->d_name;
	if (fName.find("root")==string::npos) continue;
	listFiles.push_back( (fileList+"/"+foldName+"/"+fName).c_str() );
      }
    }
  }
  
  fChain=new TChain("bTag");
  for (unsigned iF=0; iF<listFiles.size(); iF++) {
    cout << " ... adding: " << listFiles[iF] << endl;
    fChain->Add( listFiles[iF].c_str() );
  }

  TrackPlotting mainClass(fChain);
  mainClass.Loop();
}
