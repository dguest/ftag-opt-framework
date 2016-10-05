#include <dirent.h>

#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TProfile.h"
#include "TSystem.h"

#include "Helpers.C"

using namespace std;

TChain* theChain2;
string outFolder2;

void plotVariableType(string varName, 
		      string varLabel, string moreCuts, 
		      int nBin, float Max, float Min, 
		      bool log=false) {

  TCanvas* can=new TCanvas(varLabel.c_str(),varLabel.c_str(),800,600);
  
  TH1D* H_1  =GetHisto(theChain2, varName, cut1, 
		       varLabel, "norm", nBin, Max, Min); 
  TH1D* H_2  =GetHisto(theChain2, varName, cut2, 
		       varLabel, "norm", nBin, Max, Min); 
  TH1D* H_3;
  TH1D* H_4;
  if (use3) H_3=GetHisto(theChain2, varName, cut3, 
			 varLabel, "norm", nBin, Max, Min); 
  if (use4) H_4=GetHisto(theChain2, varName, cut4, 
		       varLabel, "norm", nBin, Max, Min);

  if (log) can->SetLogy();

  H_1->SetTitle( (";"+varLabel+";norm").c_str() );
  H_1->SetLineColor(2);
  H_1->SetLineWidth(4);
  H_1->SetMarkerSize(1.2);
  H_1->SetMarkerColor(2);
  H_1->GetXaxis()->SetTitleOffset(1.0);
  H_1->GetXaxis()->SetTitleSize(0.05);
  H_1->GetXaxis()->SetLabelSize(0.05);
  float maxV=H_1->GetMaximum()/H_1->Integral()*1.6;
  if (normalize) {
    H_1->Scale( 1./H_1->Integral() );
    H_1->SetMaximum(maxV);
  }
  H_1->Draw("E");


  H_2->SetLineColor(4);
  H_2->SetLineWidth(3);
  H_2->SetMarkerSize(0.9);
  H_2->SetMarkerColor(4);
  if (normalize) {
    float maxV2=H_2->GetMaximum()/H_2->Integral()*1.6;
    if (maxV2>maxV) H_1->SetMaximum(maxV2);
    H_2->Scale( 1./H_2->Integral() );
  }
  H_2->Draw("SAMEE");

  if (use3) {
    H_3->SetLineColor(8);
    H_3->SetLineWidth(3);
    H_3->SetMarkerSize(0.7);
    H_3->SetMarkerColor(8);
    if (normalize) H_3->Scale( 1./H_3->Integral() );
    H_3->Draw("SAMEE");
  }

  if (use4) {
    H_4->SetLineColor(4);
    H_4->SetLineWidth(3);
    H_4->SetMarkerSize(0.7);
    H_4->SetMarkerColor(4);
    if (normalize) H_4->Scale( 1./H_4->Integral() );
    
    H_4->Draw("SAMEE");
  }
  
  if (log) maxV=maxV*4;
  else     maxV=maxV*1.4;
  if (maxV>1.5) maxV=1.5; 
  H_1->SetMaximum(maxV);
  H_1->Draw("SAMEE");

  TLegend *leg = new TLegend(0.75,0.70,0.92,0.92,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetTextFont(62);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->AddEntry(H_1,"b-jets","l");
  leg->AddEntry(H_2,"light-jets","l");
  if (use3) leg->AddEntry(H_3,"c-jets","l");
  if (use4) leg->AddEntry(H_4,"Sherpa Masless","l");
  leg->Draw("SAME");

  string newName=varName;
  if ( newName.find("jet_jf_m/1e3")!=string::npos )  newName="jet_jf_m";
  can->Print( (outFolder2+"/plotsByType/"+newName+".eps").c_str() );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void QuickComparison_type(const char* infile="tmp2.root",  //can also be a folder
			  const char* outfolder="test") {

  gStyle->SetOptStat(0);
  SetAtlasStyle();

  use3=true;
  use4=false;
  normalize=true;

  string file1=infile;
  outFolder2=outfolder;
  
  if (outF!=0) {
    outF->Close();
    outF=0;
  }
  gSystem->Exec( ("mkdir -p "+outFolder2+"/plotsByType/").c_str());
  outF=new TFile( (outFolder2+"/plotsByType/JFplotByType.root").c_str(),"RECREATE");
  cout << "Created file: " << outF->GetName() << endl;

  // get the file (with caching)
  if (theChain2==0) {
    theChain2=new TChain("bTag");
    cout << " OPENING FILE: " << file1 << endl;
    if ( file1.find("root")!=string::npos )  {
      theChain2->Add( file1.c_str() );
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
	  theChain2->Add( (file1+"/"+foldName+"/"+fName).c_str() );
	}
      }
    }
    cout << "TOTAL number of events is: " << theChain2->GetEntries() << endl;     
  }

  // WARNING!!!!: please check the distribution of jet_jf_nvtx in your ntuples BEFORE!!!! running
  string newCut=" jet_pt>25e3 && jet_truthMatch==1 && jet_jf_nvtx!=-1 && jet_jf_dphi!=-10 ";
  cut1=newCut+" && jet_truthflav==5   ";
  cut2=newCut+" && jet_truthflav==0   ";
  cut3=newCut+" && jet_truthflav==4   ";
  
  plotVariableType("jet_jf_llr"  ," jf LLR"  , newCut ,  50,  -5, 5,true);
  plotVariableType("jet_jf_m/1e3"," jf mass (GeV)" , newCut ,  50,  0, 10, true);
  plotVariableType("jet_jf_efc"  ," jf efrac", newCut ,  50,  0, 1,true);
  plotVariableType("jet_jf_n2t"  ," jf n2t"  , newCut ,  20,   -1.5, 18.5,true);
  plotVariableType("jet_jf_deta" ," jf #Delta#eta"  , newCut ,  50,   -0.4, 0.4,true);
  plotVariableType("jet_jf_dphi" ," jf #Delta#phi"  , newCut ,  50,   -0.4, 0.4,true);
  plotVariableType("jet_jf_nvtx" ," jf vtx" , newCut ,    6,   -0.5, 5.5,true);
  plotVariableType("jet_jf_nvtx1t"," jf 1-track vtx" , newCut ,    9,   -0.5, 8.5,true);
  plotVariableType("jet_jf_ntrkAtVx"," jf track at vertex" , newCut ,    16,   -0.5, 15.5,true);

  outF->Close();
}
