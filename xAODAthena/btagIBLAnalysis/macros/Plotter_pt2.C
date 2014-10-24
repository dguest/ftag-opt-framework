#include <string>
#include <iostream>
#include <sstream>

#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TH2D.h"
#include "TProfile.h"

#include "AtlasStyle.C"
#include "AtlasUtils.C"

using namespace std;

TChain* myT_1;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TH1D* GetHisto(string varName, string cutBase, 
	       string varLabel,string yLabel, 
	       int nBin, float Max, float Min,
	       bool normalize=false) {
  
  TH1D* den  =new TH1D( (varName+"_"+yLabel).c_str(), "2", nBin, Max, Min); den->Sumw2();
  string fullVar=varName+">>"+den->GetName();
  myT_1->Draw( fullVar.c_str(), cutBase.c_str(),"goff");
  
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



///////////////////////////////////////////////////////////////////////////////////////////
TGraphAsymmErrors* GetEfficiency(string varName, 
				 string cutBase, string effCut, 
				 string varLabel,string yLabel, 
				 int nBin, float Max, float Min) {
  
  TH1D* num  =GetHisto(varName, cutBase+effCut, varLabel, "num", nBin, Max, Min);
  TH1D* den  =GetHisto(varName, cutBase, varLabel, "num", nBin, Max, Min);
  cout << " num: " << num->Integral() << " , den: " << den->Integral() << endl;
  
  TGraphAsymmErrors* graphHisto= new TGraphAsymmErrors(num,den);
  graphHisto->SetLineWidth(3);
  graphHisto->SetLineColor(2);
  graphHisto->SetMarkerStyle(20);
  graphHisto->SetMarkerSize(0.6);
  graphHisto->SetMarkerColor(2);

  return graphHisto;
}


void GetComparison(string file1, string cutBase, string effCut,
		   string Cut1 , string Cut2 , string Cut3 , 
		   string varName, string varLabel,string yLabel, 
		   int nBin, float Max, float Min) {
  TCanvas* can=new TCanvas( (varLabel+"  "+yLabel).c_str(), (varLabel+"  "+yLabel).c_str(), 800, 600);
  
  TH1D* mainH=new TH1D("histo_den", "2", nBin, Max, Min);
  mainH->Reset();
  mainH->SetTitle( (";"+varLabel+";"+yLabel+";").c_str() );
  mainH->SetMaximum(1.05);
  mainH->SetMinimum(0.0);
  mainH->Draw("HIST");
  
  myT_1=new TChain("bTag");
  myT_1->Add( file1.c_str() );
  TGraphAsymmErrors* gra1=GetEfficiency( varName, 
					 (cutBase+Cut1+" ").c_str(),  effCut, 
					 varLabel, yLabel,
					 nBin, Max, Min);
  gra1->SetLineColor(2);
  gra1->SetMarkerColor(2);
  gra1->Draw("P");
  TGraphAsymmErrors* gra1b=GetEfficiency( varName, 
					  (cutBase+Cut1+" && bH_Lxy>33").c_str(),  effCut, 
					  varLabel, yLabel,
					  nBin, Max, Min);
  gra1b->SetLineColor(6);
  gra1b->SetMarkerColor(6);
  gra1b->Draw("P");
  
  TGraphAsymmErrors* gra2=GetEfficiency( varName, 
					 (cutBase+Cut2), effCut, 
					 varLabel, yLabel,
					 nBin, Max, Min);
  gra2->SetLineColor(8);
  gra2->SetMarkerColor(8);
  gra2->Draw("P");
  
  TGraphAsymmErrors* gra3=GetEfficiency( varName, 
					 cutBase+Cut3, effCut, 
					 varLabel, yLabel,
					 nBin, Max, Min);
  gra3->SetLineColor(4);
  gra3->SetMarkerColor(4);
  gra3->Draw("P");
 
  gra1->Draw("P");

  TLine* myL=new TLine(Min,1.0,Max,1.0);
  myL->SetLineStyle(2);
  myL->SetLineColor(kGray);
  myL->SetLineWidth(2);
  myL->Draw("SAME");
  if ( varName.find("Lxy")!=string::npos) {
    TLine* myL5=new TLine(33,0.0,33,1.05);
    myL5->SetLineStyle(2);
    myL5->SetLineColor(1);
    myL5->SetLineWidth(2);
    myL5->Draw("SAME");
    TLine* myL3=new TLine(50.5,0.0,50.5,1.05);
    myL3->SetLineStyle(2);
    myL3->SetLineColor(1);
    myL3->SetLineWidth(2);
    myL3->Draw("SAME");
    TLine* myL4=new TLine(88.5,0.0,88.5,1.05);
    myL4->SetLineStyle(2);
    myL4->SetLineColor(1);
    myL4->SetLineWidth(2);
    myL4->Draw("SAME");
  }

  TLegend* legend4=new TLegend(0.70,0.75,0.920,0.95);
  legend4->SetTextFont(42);
  legend4->SetTextSize(0.04);
  legend4->SetFillColor(0);
  legend4->SetLineColor(0);
  legend4->SetFillStyle(0);
  legend4->SetBorderSize(0);
  legend4->AddEntry(gra1 ,"b-jets","l");
  legend4->AddEntry(gra1b,"b-jets (after bL)","l");
  legend4->AddEntry(gra2 ,"c-jets"   ,"l");
  legend4->AddEntry(gra3 ,"light-jet","l");
  legend4->Draw("SAME");
  TString varToPrint="Eff__"+varName+".eps";
  varToPrint=varToPrint.ReplaceAll("/1e3","");
  can->Print( varToPrint );
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetComparisonSimple(string file1, string cutBase,  
			 string Cut1 , string Cut2 , string Cut3 , 
			 string varName, string varLabel,string yLabel, 
			 int nBin, float Max, float Min, bool log=false) {

  TCanvas* can=new TCanvas( (varLabel+"  "+yLabel).c_str(), (varLabel+"  "+yLabel).c_str(), 800, 600);
  if (log) can->SetLogy();
  
  myT_1=new TChain("bTag");
  myT_1->Add( file1.c_str() );
  
  cout << cutBase+Cut1 << endl;

  TH1D* gra1=GetHisto( varName,
		       (cutBase+Cut1+" ").c_str(), 
		       varLabel, yLabel,
		       nBin, Max, Min,true);
  gra1->SetLineColor(2);
  gra1->SetLineWidth(3);
  gra1->SetMarkerColor(2);
  gra1->SetTitle( (";"+varLabel+";"+yLabel+";").c_str() );
  gra1->SetMinimum(0.001);
  TH1D* gra1b=GetHisto( varName, 
			(cutBase+Cut1+" && bH_Lxy>33").c_str(), 
			varLabel, yLabel,
			nBin, Max, Min,true);
  gra1b->SetLineColor(6);
  gra1b->SetMarkerColor(6);


  TH1D* gra2=GetHisto( varName, 
		       cutBase+Cut2, 
		       varLabel, yLabel,
		       nBin, Max, Min,true);
  gra2->SetLineColor(8);
  gra2->SetMarkerColor(8);
 
  
  TH1D* gra3=GetHisto( varName, 
		       cutBase+Cut3, 
		       varLabel, yLabel,
		       nBin, Max, Min,true);
  gra3->SetLineColor(4);
  gra3->SetMarkerColor(4);

  float maxV=gra1->GetMaximum();
  if (gra1b->GetMaximum()>maxV) maxV=gra1b->GetMaximum();
  if (gra2->GetMaximum()>maxV)  maxV=gra2->GetMaximum();
  if (gra3->GetMaximum()>maxV)  maxV=gra3->GetMaximum();
  gra1->SetMaximum(maxV);

  gra1->Draw("HIST");
  gra2->Draw("SAMEHIST");
  gra3->Draw("SAMEHIST");
  gra1b->Draw("SAMEHIST");
  gra1->Draw("SAMEHIST");

  TLegend* legend4=new TLegend(0.67,0.68,0.920,0.93);
  legend4->SetTextFont(42);
  legend4->SetTextSize(0.04);
  legend4->SetFillColor(0);
  legend4->SetLineColor(0);
  legend4->SetFillStyle(0);
  legend4->SetBorderSize(0);
  legend4->AddEntry(gra1 ,"b-jets","l");
  legend4->AddEntry(gra1b,"b-jets (after IBL)","l");
  legend4->AddEntry(gra2 ,"c-jets"   ,"l");
  legend4->AddEntry(gra3 ,"light-jet","l");
  legend4->Draw("SAME");

  TString varToPrint="Var__"+varName+".eps";
  varToPrint=varToPrint.ReplaceAll("/1e3","");
  can->Print( varToPrint );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plotter_pt2() {
  gStyle->SetOptStat(0);
  SetAtlasStyle();
  
  string file1="flavntuple_110401_ttbar.root";

  string CutBase="abs(jet_eta)<2.5 && jet_pt>25e3 ";
  
  string Cut1=" && jet_truthflav==5 "; 
  string Cut2=" && jet_truthflav==4 "; 
  string Cut3=" && (jet_truthflav!=4 && jet_truthflav!=5 && jet_truthflav!=15) ";  
  string yLabel="norm.";

  GetComparisonSimple(file1,CutBase,
		      Cut1, Cut2, Cut3,
		      "jet_mv2c00"  , "MV2c00", yLabel,  
		      40, -1.0, 1.0, true); 

  
  yLabel="MV1@70% eff.";
  string effCut=" && jet_mv1>0.9275950125 ";
  GetComparison(file1,CutBase, effCut,
		Cut1, Cut2, Cut3,
	        "bH_Lxy"  , "b-hadron transverse decay length (mm)", yLabel,  
		20,  0, 100);
  
  GetComparison(file1,CutBase, effCut,
		Cut1, Cut2, Cut3,
	        "jet_pt", "jet p_{T} (GeV)", yLabel,  
		20,  25e3, 500e3);
 

  effCut=" && jet_ip3d_llr>1.797 ";
  yLabel="IP3D@70% eff.";
  GetComparison(file1,CutBase, effCut,
		Cut1, Cut2, Cut3,
	        "bH_Lxy"  , "b-hadron transverse decay length (mm)", yLabel,  
		20,  0, 100);

  effCut=" && jet_mvb>-0.142575 ";
  yLabel="MVb@70% eff.";
  GetComparison(file1,CutBase, effCut,
		Cut1, Cut2, Cut3,
	        "bH_Lxy"  , "b-hadron transverse decay length (mm)", yLabel,  
		20,  0, 100);


  /*
 
  /////////////////////////////////////////////////////////////////////////////////////////////////
  newCut2=" && jet_sv1>2.655";   ///4.725";
  yLabel="sv1@60% eff.";
  GetComparison(file1,file2,file3,
		Cut1, Cut2, Cut3,
	        "jet_bH_Lxy"  , newCut2, 
		"b-hadron transverse decay length (mm)", yLabel,  
		50,  0, 100);
  
  /////////////////////////////////////////////////////////////////////////////////////////////////
  newCut2=" && jet_combNN>0.673375";   ///4.725";
  yLabel="JetFitterCombNN@60% eff.";
  GetComparison(file1,file2,file3,
		Cut1, Cut2, Cut3,
	        "jet_bH_Lxy"  , newCut2, 
		"b-hadron transverse decay length (mm)", yLabel,  
		50,  0, 100);
  */
 

  yLabel="norm.";
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]){  
  SetAtlasStyle(); 
  TApplication* myApp= new TApplication("test",0,0);
  Plotter_pt2();
  myApp->Run();
}
