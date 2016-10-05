#include <dirent.h>

#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TProfile.h"
#include "TSystem.h"
#include "TH2D.h"

#include "Helpers.C"

using namespace std;

TChain* theChain4;
string outFolder4;

void plotVariable_2D(string varName, string varLabel, string moreCuts, int nBin, float Max, float Min, bool log=false) {
  TCanvas* can=new TCanvas(varLabel.c_str(),varLabel.c_str(),800,600);
  
  TH2D* H_1  =new TH2D("histo_1", "1", 40,  0, 500, nBin, Max, Min); H_1->Sumw2();
  TH2D* H_2  =new TH2D("histo_2", "2", 40,  0, 500, nBin, Max, Min); H_2->Sumw2();
  TH2D* H_3;
  TH2D* H_4;
  if (use3) {H_3=new TH2D("histo_3", "3", 40,  0, 500, nBin, Max, Min); H_3->Sumw2();}
  if (use4) {H_4=new TH2D("histo_4", "4", 40,  0, 500, nBin, Max, Min); H_4->Sumw2();}
  
  theChain4->Draw( (varName+":jet_pt/1e3>>histo_1").c_str(), cut1.c_str(),"goff",limit);
  theChain4->Draw( (varName +":jet_pt/1e3>>histo_2").c_str(), cut2.c_str(),"goff",limit);
  if (use3) theChain4->Draw( (varName+":jet_pt/1e3>>histo_3").c_str(), cut3.c_str(),"goff",limit);
  if (use4) theChain4->Draw( (varName+":jet_pt/1e3>>histo_4").c_str(), cut4.c_str(),"goff",limit);
  
  if (log) can->SetLogy();

  TProfile* Hp_1=H_1->ProfileX( (varLabel+"__1").c_str() );
  TProfile* Hp_2=H_2->ProfileX( (varLabel+"__2").c_str() ); 
  TProfile* Hp_3=0;
  if (use3) Hp_3=H_3->ProfileX( (varLabel+"__3").c_str() );
  TProfile* Hp_4=0;
  if (use4) Hp_4=H_4->ProfileX( (varLabel+"__4").c_str() );
  float maxV=Hp_1->GetMaximum();
  if (Hp_2->GetMaximum()>maxV) maxV=Hp_2->GetMaximum();
  if (Hp_3->GetMaximum()>maxV) maxV=Hp_3->GetMaximum();
  float minV=Hp_1->GetMinimum();
  if (Hp_2->GetMinimum()<maxV) minV=Hp_2->GetMinimum();
  if (Hp_3->GetMinimum()<maxV) minV=Hp_3->GetMinimum();
  Hp_1->SetMaximum(maxV*1.3);
  Hp_1->SetMinimum(minV*0.9);
  if (Hp_1->GetMinimum()<0.)   Hp_1->SetMinimum(-4);
  
  
  Hp_1->SetTitle( (";jet pt (GeV);<"+varLabel+">;").c_str() );
  Hp_1->SetLineColor(2);
  Hp_1->SetLineWidth(4);
  Hp_1->SetMarkerSize(1.2);
  Hp_1->SetMarkerColor(2);
  Hp_1->GetXaxis()->SetTitleOffset(1.0);
  Hp_1->GetXaxis()->SetTitleSize(0.05);
  Hp_1->GetXaxis()->SetLabelSize(0.05);
  Hp_1->Draw("E");

  Hp_2->SetLineColor(4);
  Hp_2->SetLineWidth(3);
  Hp_2->SetMarkerSize(0.9);
  Hp_2->SetMarkerColor(4);
  Hp_2->Draw("SAMEE");

  if (use3) {
    Hp_3->SetLineColor(8);
    Hp_3->SetLineWidth(3);
    Hp_3->SetMarkerSize(0.7);
    Hp_3->SetMarkerColor(8);
    Hp_3->Draw("SAMEE");
  }

  if (use4) {
    Hp_4->SetLineColor(4);
    Hp_4->SetLineWidth(3);
    Hp_4->SetMarkerSize(0.7);
    Hp_4->SetMarkerColor(4);
    Hp_4->Draw("SAMEE");
  }
  
  TLegend *leg = new TLegend(0.75,0.70,0.92,0.92,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetTextFont(62);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->AddEntry(Hp_1,"b-jets","lpe");
  leg->AddEntry(Hp_2,"light-jets","lpe");
  if (use3) leg->AddEntry(Hp_3,"c-jets","lpe");
  if (use4) leg->AddEntry(Hp_4,"Sherpa Masless","l");
  leg->Draw("SAME");

  string newName=varName;
  if ( newName.find("jet_jf_m/1e3")!=string::npos )  newName="jet_jf_m";
  if ( newName.find("abs(jet_jf_deta)")!=string::npos )  newName="jet_jf_deta";
  if ( newName.find("abs(jet_jf_dphi)")!=string::npos )  newName="jet_jf_dphi";
  can->Print( (outFolder4+"/"+newName+".eps").c_str() );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void QuickComparison_2D(const char* infile="tmp2.root",  //can also be a folder
			const char* outfolder="test") {
  
  gStyle->SetOptStat(0);
  SetAtlasStyle();

  use3=true;
  use4=false;
  normalize=false;

  string file1=infile;
  outFolder4=outfolder;
  if (outF!=0) {
    outF->Close();
    outF=0;
  }

  gSystem->Exec( ("mkdir -p "+outFolder4+"/plots_2D/").c_str());
  outF=new TFile( (outFolder4+"/plots_2D/JF2dprofile.root").c_str(),"RECREATE");
  //cout << "Created file: " << outF->GetName() << endl;

  // get the file (with caching)
  if (theChain4==0) {
    theChain4=new TChain("bTag");
    cout << " OPENING FILE: " << file1 << endl;
    if ( file1.find("root")!=string::npos )  {
      theChain4->Add( file1.c_str() );
    } else {
      cout << "Input is a directory: " << file1 << endl;
      DIR*     dir;
      dirent*  pdir;
      dir = opendir( file1.c_str() );     // open current directory
      while (pdir = readdir(dir))  {
	string foldName=pdir->d_name;
	if (foldName.find("mc")==string::npos) continue;
	cout << pdir->d_name << endl;
	DIR*     dir2;
	dirent*  pdir2;
	dir2 = opendir( (file1+"/"+foldName).c_str() );     // open current directory
	while (pdir2 = readdir(dir2))  {
	  string fName=pdir2->d_name;
	  if (fName.find("root")==string::npos) continue;
	  theChain4->Add( (file1+"/"+foldName+"/"+fName).c_str() );
	}
      }
    }
    cout << "TOTAL number of events is: " << theChain4->GetEntries() << endl;     
  }
 
  string newCut=" jet_pt>25e3 && jet_truthMatch==1 && jet_jf_nvtx!=-1 && jet_jf_dphi!=-10 ";
  cut1=newCut+" && jet_truthflav==5   ";
  cut2=newCut+" && jet_truthflav==0   ";
  cut3=newCut+" && jet_truthflav==4   ";

  plotVariable_2D("jet_jf_llr"  ," jf LLR"  , newCut ,  50,  -5, 5);
  plotVariable_2D("jet_jf_m/1e3"," jf mass (GeV)" , newCut ,  50,  0, 10);
  plotVariable_2D("jet_jf_efc"  ," jf efrac", newCut ,  50,  0, 1);
  plotVariable_2D("jet_jf_n2t"  ," jf n2t"  , newCut ,  20,   -1.5, 18.5);
  plotVariable_2D("abs(jet_jf_deta)" ," jf #Delta#eta"  , newCut ,  30,   0, 0.4);
  plotVariable_2D("abs(jet_jf_dphi)" ," jf #Delta#phi"  , newCut ,  30,   0, 0.4);
  plotVariable_2D("jet_jf_nvtx" ," jf vtx" , newCut ,    6,   -0.5, 5.5);
  plotVariable_2D("jet_jf_nvtx1t"," jf 1-track vtx" , newCut ,    9,   -0.5, 8.5);
  plotVariable_2D("jet_jf_ntrkAtVx"," jf track at vertex" , newCut ,    16,   -0.5, 15.5);
}
