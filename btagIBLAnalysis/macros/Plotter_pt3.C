#include <string>
#include <iostream>
#include <sstream>

#include <dirent.h>

#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TH2D.h"
#include "TProfile.h"
#include "TSystem.h"

#include "AtlasStyle.C"
#include "AtlasUtils.C"

using namespace std;

TChain* myT_1;

bool is8TeV=false;
bool isXAOD=false;
float workpoint=0.70;
string outputFolder;

TFile* outF;
TGraphAsymmErrors* graTMP;

Double_t bins[] = {20,40,60,80,100,120,140,160,180,220,260,300,1000000 };
Int_t  binnum = 12;

/////////////////////////////////////////////////
/// Plotting a lot of histograms from a given tree
/////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


float getLowBin(string tagger) {
  if (tagger=="MV1")       return  0.0;
  if (tagger=="MV1c")      return  0.0;
  if (tagger=="MV2c00")    return  -1.;
  if (tagger=="MV2c10")    return  -1.;
  if (tagger=="MV2c20")    return  -1.;
  if (tagger=="IP3D")      return  -12.; 
  if (tagger=="IP2D")      return  -12.; 
  if (tagger=="IP3D+SV1")  return  -10.;
  if (tagger=="MVb")       return  -1;
  if (tagger=="SV1")       return  -4.;
  if (tagger=="JetFitter") return  -15;
  cout << "NOT SUPPORTED!!! " << endl;
  return 0.0;
}

float getHighBin(string tagger) {
  if (tagger=="MV1")       return  1.0;
  if (tagger=="MV1c")      return  1.0;
  if (tagger=="MV2c00")    return  1.;
  if (tagger=="MV2c10")    return  1.;
  if (tagger=="MV2c20")    return  1.;
  if (tagger=="IP3D")      return  30; 
  if (tagger=="IP2D")      return  30; 
  if (tagger=="IP3D+SV1")  return  35;
  if (tagger=="MVb")       return  1.;
  if (tagger=="SV1")       return  13;
  if (tagger=="JetFitter") return  10;
  cout << "NOT SUPPORTED!!! " << endl;
  return 0.;
}

string getVariable(string tagger, bool a8TeV) {
  if (tagger=="MV1")    return "jet_mv1";
  if (tagger=="MV1c")   return "jet_mv1c";
  if (tagger=="MV2c00") return "jet_mv2c00";
  if (tagger=="MV2c10") return "jet_mv2c10";
  if (tagger=="MV2c20") return "jet_mv2c20";
  if (tagger=="MVb")    return "jet_mvb";
  if (tagger=="IP3D") {
    if (isXAOD) return  "jet_ip3d_llr";
    else       return  "jet_ip3d";
  }
  if (tagger=="IP2D") {
    if (isXAOD) return  "jet_ip2d_llr";
    else       return  "jet_ip2d";
  }
  if (tagger=="IP3D+SV1") {
    if (isXAOD) return  "jet_sv1ip3d";
    else       return  "jet_ip3dsv1";
  }
  
  if (tagger=="SV1") {
    if (isXAOD) return  "jet_sv1_llr";
    else       return  "jet_sv1";
  }

  if (tagger=="JetFitter") {
    if (isXAOD) return  "jet_jf_llr";
    else       return  "jet_tagNN";
  }

  cout << "NOT SUPPORTED!!! " << endl;
  return "0.";
}



TH1D* GetHisto1(string varName, string cutBase, 
	       string varLabel,string yLabel, 
	       int nBin, float Max, float Min,
	       bool normalize=false) {
  
  TString tmpName=varName+cutBase;
  tmpName=tmpName.ReplaceAll(" ","").ReplaceAll("&","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll("=","").ReplaceAll(">","").ReplaceAll("<","").ReplaceAll("/1e3","").ReplaceAll(".","").ReplaceAll("+","");
  string theName=string(tmpName);

  TH1D* den  =new TH1D( theName.c_str(), theName.c_str(), nBin, Max, Min); den->Sumw2();
  string fullVar=varName+">>"+den->GetName();
  myT_1->Draw( fullVar.c_str(), cutBase.c_str(),"goff");//,1000000)
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
 
  std::cout << " Integral is: " << den->Integral() << std::endl;

  if (normalize) den->Scale(1./den->Integral());
  return den;
}


TH2D* GetHisto(string varName, string cutBase, 
	       string varLabel,string tagger, 
	       int nBin, float Max, float Min,
	       bool normalize=false) {
  
  /// this is over ultra stupid but I am in a rush and I don't manage to get it to work otherwise
  TString tmpName=varName+cutBase;
  tmpName=tmpName.ReplaceAll(" ","").ReplaceAll("&","").ReplaceAll("(","").ReplaceAll(")","").ReplaceAll("=","").ReplaceAll(">","").ReplaceAll("<","").ReplaceAll("/1e3","").ReplaceAll(".","").ReplaceAll("+","");
  string theName=string(tmpName);

  TH2D* den  =NULL;
  /*
  if (varName.find("pt11111111")!=string::npos){
    den=new TH2D( theName.c_str(), theName.c_str(),
                  binnum, bins,
                  1000, getLowBin(tagger), getHighBin(tagger) );
  } else {
  */
    den=new TH2D( theName.c_str(), theName.c_str(),
                  nBin, Max, Min,
                  1000, getLowBin(tagger), getHighBin(tagger) );
    // }


  den->Sumw2();
  string fullVar=getVariable(tagger, false)+":"+varName+">>"+den->GetName();
  cout << "fullVar: " << fullVar << endl;
  cout << "cutBase: " << cutBase << endl;
  myT_1->Draw( fullVar.c_str(), cutBase.c_str(),"goff"); //,4000000);
  cout << nBin << " , " << Max << " , " << Min << endl;
  cout << "Int: " << den->Integral() << endl; 

  return den;
}


TCanvas* myC1;

///////////////////////////////////////////////////////////////////////////////////////////
TGraphAsymmErrors* GetEfficiency(string varName, 
				 string cutBase, string tagger, 
				 string varLabel,string yLabel, 
				 int nBin, float Max, float Min, float &numC) {
  
  TH2D* bjets=GetHisto(varName, cutBase+" && jet_LabDr_HadF==5", 
		       varLabel, tagger, nBin, Max, Min);

  TH2D* ljets=GetHisto(varName, cutBase+" && (jet_LabDr_HadF!=4 && jet_LabDr_HadF!=5 && jet_LabDr_HadF!=15) ", 
		       varLabel, tagger, nBin, Max, Min);

  /*
  myC1=new TCanvas("test","test",800,600);
  myC1->Divide(2,1);
  myC1->cd(1);
  bjets->Draw("COLZ");
  myC1->cd(2);
  ljets->Draw("COLZ");
  myC1->Update();
  */

  bjets->SetDirectory(0);
  ljets->SetDirectory(0);
  cout << "Integrals: " << bjets->Integral() << "  .... " << ljets->Integral() << endl;

  TH1D* den  =NULL;
  TH1D* num  =NULL;
  if (varName.find("pt111111111")!=string::npos){
    //num  =new TH1D("num","num",binnum, bins); num->Sumw2();
    //den  =new TH1D("den","den",binnum, bins); den->Sumw2();
  }else{
    num  =new TH1D("num","num",nBin, Max, Min); num->Sumw2();
    den  =new TH1D("den","den",nBin, Max, Min); den->Sumw2();
  }
  TH1D* num2  =new TH1D("num2","num2",nBin, Max, Min);// num2->Sumw2();
  TH1D* den2  =new TH1D("den2","den2",nBin, Max, Min);// den2->Sumw2();

  int masterBin=1000;;
  for (int bin=1; bin<=nBin; bin++) {
    // use the b-jets to find the cut in each bin
    int SmallX=bin;
    int BigX=bin;
    if (SmallX==1 ) SmallX=0;
    if (BigX==nBin) BigX=nBin+1;

    float bInt=bjets->Integral(SmallX,BigX,0,masterBin+1);
    float lInt=ljets->Integral(SmallX,BigX,0,masterBin+1);
    //cout << " for this bin: " << bin 
    //<< " small: " << SmallX << " big: " << BigX << endl;
    //cout << "Full integrals: " << bInt << " , " << lInt << endl;
    den->SetBinContent(bin,lInt);
    den->SetBinError(bin,sqrt(lInt));
    den2->SetBinContent(bin,bInt);

    int binCut=-1;
    float prevEff=0;
    for (int bin2=1; bin2<=masterBin; bin2++) {
      float bIntCut=bjets->Integral(SmallX,BigX,bin2,masterBin+1);
      float tmpEff=bIntCut/bInt;
      if ( fabs(tmpEff-workpoint)<=fabs(prevEff-workpoint) ) {
	binCut=bin2;
	prevEff=tmpEff;
      } else {
	break;
      }
    }
    float lIntCut=ljets->Integral(SmallX,BigX,binCut,masterBin+1);
    cout << " Cut of: " << binCut 
	 << " gives and eff of: " << prevEff 
	 << " .... and fakeRate of: " << lIntCut/lInt << endl ;
    num->SetBinContent(bin,lIntCut);
    num->SetBinError(bin,sqrt(lIntCut));
    num2->SetBinContent(bin,prevEff*bInt);
    
  } // bin loop

  TGraphAsymmErrors* graphHisto= new TGraphAsymmErrors(num,den);
  graphHisto->SetLineWidth(3);
  graphHisto->SetLineColor(2);
  graphHisto->SetMarkerStyle(20);
  graphHisto->SetMarkerSize(0.6);
  graphHisto->SetMarkerColor(2);

  graTMP= new TGraphAsymmErrors(num2,den2);
  graTMP->SetLineWidth(3);
  graTMP->SetLineColor(4);
  graTMP->SetMarkerStyle(20);
  graTMP->SetMarkerSize(0.6);
  graTMP->SetMarkerColor(4);

  return graphHisto;
}



void GetComparison(string file1, string cutBase, string tagger,
		   string Cut1 , string Cut2 , string Cut3 , 
		   string varName, string varLabel,string yLabel, 
		   int nBin, float Max, float Min) {
  TCanvas* can=new TCanvas( (varLabel+"  "+yLabel).c_str(), (varLabel+"  "+yLabel).c_str(), 800, 600);
  
  float numC;
  
  string tmpVarName=varName;
  if (!isXAOD) {
    if (varName=="bH_Lxy")          varName="jet_bH_Lxy";
    if (varName=="jet_truthPt/1e3") varName="jet_pt/1e3";
  }


  TH1D* mainH=NULL;
  if (varName.find("pt1111")!=string::npos){
    mainH=new TH1D("histo_den", "2", binnum, bins);
  }else{
    mainH=new TH1D("histo_den", "2", nBin, Max, Min);
  }

  mainH->Reset();
  mainH->SetTitle( (";"+varLabel+";"+yLabel+";").c_str() );
  mainH->SetMaximum(0.10);
  mainH->SetMinimum(0.0);
  mainH->Draw("HIST");
  mainH->SetDirectory(0);
  
  if (myT_1==0) {
    myT_1=new TChain("bTag_AntiKt4EMTopoJets");
    cout << " OPENING FILE: " << file1 << endl;
    if ( file1.find("root")!=string::npos )  {
      myT_1->Add( file1.c_str() );
    } else {
      cout << "Input is a directory: going fancy: " << endl;
      DIR*     dir;
      dirent*  pdir;
      dir = opendir( file1.c_str() );     // open current directory
      while (pdir = readdir(dir))  {
	string foldName=pdir->d_name;
	if (foldName.find("mc")==string::npos && foldName.find("valid")==string::npos ) continue;
	cout << pdir->d_name << endl;
	DIR*     dir2;
	dirent*  pdir2;
	dir2 = opendir( (file1+"/"+foldName).c_str() );     // open current directory
	while (pdir2 = readdir(dir2))  {
	  string fName=pdir2->d_name;
	  //cout << " fName: " << fName << endl;
	  if (fName.find("root")==string::npos) continue;
	  //cout << fName << endl;
	  myT_1->Add( (file1+"/"+foldName+"/"+fName).c_str() );
	}
      }
    }     
    cout << "TOTAL number of events is: " << myT_1->GetEntries() << endl;
  }
  
 
  TGraphAsymmErrors* gra1=GetEfficiency( varName, 
					 (cutBase+" ").c_str(),  tagger, 
					 varLabel, yLabel,
					 nBin, Max, Min, numC);
  gra1->SetLineColor(2);
  gra1->SetMarkerColor(2);
  gra1->Draw("P");

  /*
  TGraphAsymmErrors* gra2=GetEfficiency( varName, 
					 (cutBase+Cut2), effCut, 
					 varLabel, yLabel,
					 nBin, Max, Min, numC);
  gra2->SetLineColor(8);
  gra2->SetMarkerColor(8);
  gra2->Draw("P");
  
  TGraphAsymmErrors* gra3=GetEfficiency( varName, 
					 cutBase+Cut3, effCut, 
					 varLabel, yLabel,
					 nBin, Max, Min, numC);
  gra3->SetLineColor(4);
  gra3->SetMarkerColor(4);
  gra3->Draw("P");
  */

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
  //legend4->AddEntry(gra1 ,"b-jets","l");
  //legend4->AddEntry(gra1b,"b-jets (after bL)","l");
  //legend4->AddEntry(gra2 ,"c-jets"   ,"l");
  //legend4->AddEntry(gra3 ,"light-jet","l");
  legend4->Draw("SAME");
 
  varName=tmpVarName;
  TString varToPrint="Eff__"+varName+"__"+yLabel+".eps";
  varToPrint=varToPrint.ReplaceAll("/1e3","").ReplaceAll("abs(","").ReplaceAll(")","").ReplaceAll("=","").ReplaceAll(">","_").ReplaceAll("eff.","").ReplaceAll("%","").ReplaceAll("@","_").ReplaceAll(" ","").ReplaceAll("+","");
  
  outF->cd();
  TString baseName="Base__"+varName;
  baseName= baseName.ReplaceAll("/1e3","").ReplaceAll("abs(","").ReplaceAll(")","").ReplaceAll("=","").ReplaceAll(">","_").ReplaceAll("eff.","").ReplaceAll("%","").ReplaceAll("@","_").ReplaceAll(" ","").ReplaceAll("+","");
  
  TString plotName= varToPrint.ReplaceAll(".eps","");
  outF->WriteObject(mainH,baseName);

  TString bEff="Eff_b__"+plotName;
  TString cEff="Eff_c__"+plotName;
  TString lEff="Eff_l__"+plotName;
  gra1->SetName(bEff);
  gra1->Write();

  graTMP->SetName( ("Eff_TMP__"+plotName) );
  graTMP->Write();

  //gra2->SetName(cEff);
  //gra2->Write();
  //gra3->SetName(lEff);
  //gra3->Write();
  
  //outF->WriteObject(gra1,bEff);
  //outF->WriteObject(gra2,cEff);
  //outF->WriteObject(gra3,lEff);
  
  TString varToPrint1=outputFolder+"/"+varToPrint+".eps";
  TString varToPrint2=outputFolder+"/"+varToPrint+".png";
  TString varToPrint3=outputFolder+"/"+varToPrint+".C";
  can->Print( varToPrint1 );
  can->Print( varToPrint2 );
  can->Print( varToPrint3 );
}


void plotKine(string file1, string cutBase, 
	      string Cut1 , string Cut2 , string Cut3 , 
	      string varName, string varLabel,string yLabel, 
	      int nBin, float Max, float Min) {
  
  TCanvas* can=new TCanvas( (varLabel+"  KINE").c_str(), (varLabel+"  KINE").c_str(), 800, 600);
  
  float numC;
  
  TH1D* histoB=GetHisto1(varName,  cutBase+Cut1, 
			varLabel, yLabel, 
			nBin, Max, Min, true);
  histoB->SetLineColor(2);
  histoB->SetMarkerColor(2);
  histoB->Draw("HIST");

  
  TH1D* histoC=GetHisto1(varName,  cutBase+Cut2, 
			varLabel, yLabel, 
			nBin, Max, Min, true);
  histoC->SetLineColor(8);
  histoC->SetMarkerColor(8);
  histoC->Draw("HISTSAME");

  TH1D* histoL=GetHisto1(varName,  cutBase+Cut3, 
			varLabel, yLabel, 
			nBin, Max, Min, true);
  histoL->SetLineColor(4);
  histoL->SetMarkerColor(4);
  histoL->Draw("HISTSAME");

  outF->cd();
  TString baseName=varName;
  baseName= baseName.ReplaceAll("/1e3","").ReplaceAll("abs(","").ReplaceAll(")","").ReplaceAll("jet_","");
  
  outF->WriteObject(histoB, ("b_"+baseName) );
  outF->WriteObject(histoC, ("c_"+baseName) );
  outF->WriteObject(histoL, ("u_"+baseName) );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintTagger(string tagger, string file1)  {
  /// even more ugly
  
  string CutBase="";
  if (isXAOD) CutBase=" abs(jet_eta)<2.5 && jet_pt>20e3 && (jet_JVT>0.641 || jet_pt>50e3 || abs(jet_eta)>2.4)";// && jet_aliveAfterOR==1";
  else        CutBase=" abs(jet_eta)<2.5 && jet_pt>20e3 && (jet_JVT>0.641 || jet_pt>50e3 || abs(jet_eta)>2.4)";// && jet_aliveAfterOR==1";

  string Cut1=" "; 
  string Cut2=" "; 
  string Cut3=" ";  
  if (isXAOD) {
    Cut1=" && jet_LabDr_HadF==5 "; 
    Cut2=" && jet_LabDr_HadF==4 "; 
    Cut3=" && (jet_LabDr_HadF!=4 && jet_LabDr_HadF!=5 && jet_LabDr_HadF!=15) "; 
  } else {
    Cut1=" && jet_trueFlav==5 "; 
    Cut2=" && jet_trueFlav==4 "; 
    Cut3=" && (jet_LabDr_HadF!=4 && jet_LabDr_HadF!=5 && jet_LabDr_HadF!=15) "; 
  }
  
  // MV1: quite detailed info
  string yLabel=tagger+"l rej @70% eff.";
  string effCut=" && "+getVariable(tagger, is8TeV)+">"+" ";
 

  GetComparison(file1,CutBase, tagger,
		Cut1, Cut2, Cut3,
	        "jet_pt/1e3", "jet p_{T} (GeV)", yLabel,
		10,20,400);
		//19,20,400);
  //39,  20, 1000);
  std::cout << std::endl;

  if (tagger=="IP3D") {
   
    plotKine( file1,CutBase, Cut1, Cut2, Cut3,
	      "jet_pt/1e3", "jet p_{T} (GeV)", yLabel,  
	      39,  20, 1000);
    plotKine( file1,CutBase, Cut1, Cut2, Cut3,
	      "abs(jet_eta)", "jet #eta", yLabel,  
	      20, 0, 2.5); 

    TH1D* histoMu=GetHisto1("avgmu", "avgmu>-1",
			    "<mu>", "int", 
			    51, -0.4, 50.6, true);
    TCanvas* myCX=new TCanvas("testBlah","testBlah",800,600);
    histoMu->Draw();
    outF->cd();
    outF->WriteObject(histoMu, "avgmu" );
    myCX->Print("shit");


    TH1D* histoPV=GetHisto1("nPV", "nPV>-1",
			    "nPV", "int", 
			    41, -0.4, 40.6, true);
    TCanvas* myCY=new TCanvas("testBlah2","testBlah2",800,600);
    histoPV->Draw();
    outF->cd();
    outF->WriteObject(histoPV, "nPV" );
    myCY->Print("shit2");
  }

  GetComparison(file1,CutBase, tagger,
		Cut1, Cut2, Cut3,
	        "abs(jet_eta)", "jet #eta", yLabel,  
		20, 0, 2.5); // was 10
		
  GetComparison(file1, CutBase, tagger,
                Cut1, Cut2, Cut3,
                "avgmu", "<#mu>", yLabel,
                10,  0, 40);

  GetComparison(file1, CutBase, tagger,
                Cut1, Cut2, Cut3,
                "nPV", "# PV", yLabel,
                10,  0, 30);
}
  


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plotter_pt3(const char* infile,
		 const char* outfolder, float wkpoint) {
  gStyle->SetOptStat(0);
  SetAtlasStyle();
  cout << "inside pt3!!!!" << endl;

  workpoint=wkpoint;

  outputFolder=outfolder;
  if (outputFolder.find("8TeV")!=string::npos) is8TeV=true;
  if (outputFolder.find("XAOD")!=string::npos) isXAOD=true;
  
  gSystem->Exec( ("mkdir -p "+outputFolder).c_str());
  
  outF=new TFile( (outputFolder+"/effPlotsFix.root").c_str(),"RECREATE");
  cout << "Created file: " << outF->GetName() << endl;

  string file1=infile;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////PrintTagger("MV1",file1);
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////PrintTagger("MV1c",file1);
  ///////////////////////////////////////////////////////////////////////////PrintTagger("MV2c00",file1);
  /////////////////////////////////PrintTagger("MV2c10",file1);
  PrintTagger("MV2c20",file1);
  /////////////////////////////////PrintTagger("MVb",file1);
  PrintTagger("IP3D",file1);
  /////////////////////////////////PrintTagger("IP2D",file1);
  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////PrintTagger("IP3D+SV1",file1);
  PrintTagger("SV1",file1);
  PrintTagger("JetFitter",file1);

  outF->Close();
}

