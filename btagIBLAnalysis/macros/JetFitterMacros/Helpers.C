#include <string>
#include <iostream>
#include <sstream>

#include "../AtlasStyle.C"
#include "../AtlasUtils.C"

#include "TFile.h"
#include "TChain.h"
#include "TGraphAsymmErrors.h"
#include "TH1.h"

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
string outputFolder;
TFile* outF;

string cut1;
string cut2;
string cut3;
string cut4;

string Ecut1;
string Ecut2;
string Ecut3;
string Ecut4;

string label1;
string label2;
string label3;
string label4;

string title;

bool use3;
bool use4;
bool normalize;

int limit=5000; //10000000;

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////
TH1D* GetHisto(TChain* myT,
	       string varName, string cutBase, 
	       string varLabel,string yLabel, 
	       int nBin, float Max, float Min,
	       bool normalize=false) {
  
  /// this is over ultra stupid but I am in a rush and I don't manage to get it to work otherwise
  TString tmpName=varName+cutBase;
  tmpName=tmpName.ReplaceAll(" ","").ReplaceAll("&","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll("=","").ReplaceAll(">","").ReplaceAll("<","").ReplaceAll("/1e3","").ReplaceAll(".","").ReplaceAll("+","");
  string theName=string(tmpName);

  TH1D* den  =new TH1D( theName.c_str(), theName.c_str(), nBin, Max, Min); den->Sumw2();
  string fullVar=varName+">>"+den->GetName();
  myT->Draw( fullVar.c_str(), cutBase.c_str(),"goff",limit);
 
  den->SetBinContent(1, den->GetBinContent(0)+den->GetBinContent(1));
  den->SetBinError(1, sqrt(pow(den->GetBinError(0),2)+pow(den->GetBinError(1),2)));
  den->SetBinContent(0, 0.0);
  den->SetBinError(0, 0.0);
  den->SetBinContent(den->GetNbinsX(), den->GetBinContent(den->GetNbinsX())+den->GetBinContent(den->GetNbinsX()+1));
  den->SetBinError(den->GetNbinsX(), sqrt(pow(den->GetBinError(den->GetNbinsX()),2)+pow(den->GetBinError(den->GetNbinsX()+1),2)));
  den->SetBinContent(den->GetNbinsX()+1, 0.0);
  den->SetBinError(den->GetNbinsX()+1, 0.0);
  den->SetLineWidth(3);
  den->SetLineColor(2);
  den->SetMarkerStyle(20);
  den->SetMarkerSize(0.6);
  den->SetMarkerColor(2);
 
  if (normalize) {
    float maxV=den->GetMaximum()*1.1/den->Integral();
    den->Scale(1./den->Integral());
    den->SetMaximum(maxV);
  }
  return den;
}


////////////////////////////////////////////////////////////////////////////////////
TGraphAsymmErrors* GetEfficiency(TChain* myT,
				 string varName, 
				 string cutBase, string effCut, 
				 string varLabel,string yLabel, 
				 int nBin, float Max, float Min, float &numC) {
  
  TH1D* num  =GetHisto(myT,varName, cutBase+effCut, varLabel, "num", nBin, Max, Min);
  TH1D* den  =GetHisto(myT,varName, cutBase, varLabel, "num", nBin, Max, Min);
  //cout << " num: " << num->Integral() << " , den: " << den->Integral() << endl;
  numC=num->Integral();

  TGraphAsymmErrors* graphHisto= new TGraphAsymmErrors(num,den);
  graphHisto->SetLineWidth(3);
  graphHisto->SetLineColor(2);
  graphHisto->SetMarkerStyle(20);
  graphHisto->SetMarkerSize(0.6);
  graphHisto->SetMarkerColor(2);

  return graphHisto;
}

