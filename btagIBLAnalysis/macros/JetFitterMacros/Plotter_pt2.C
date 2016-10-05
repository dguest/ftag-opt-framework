#include <dirent.h>

#include "TCanvas.h"
#include "TLegend.h"
#include "TApplication.h"
#include "TProfile.h"
#include "TSystem.h"

#include "Helpers.C"

using namespace std;

TChain* theChain;
string outFolder;

//////////////////////////////////////////////////////////////////////////////////
string getCut(string tagger, bool a8TeV) {
  if (tagger=="MV1") {
    if (a8TeV) return "0.793362375";
    else      return  "0.945487725";
  }
  if (tagger=="MV1c") {
    if (a8TeV) return "0.7068333333";
    else      return  "0.779833333333";
  }
  if (tagger=="MV2c00") {
    if (a8TeV) return "0.0561666666667";
    else      return  "0.0308333333333";
  }
  if (tagger=="MV2c10") {
    if (a8TeV) return  "0.0295";
    else      return  "-0.00416666666667";
  }
  if (tagger=="MV2c20") {
    if (a8TeV) return  "0.0105";
    else      return  "-0.0215";
  }
  if (tagger=="IP3D") {
    if (a8TeV) return  "1.475";
    else      return  "2.007";
  }
  if (tagger=="IP3D+SV1") {
    if (a8TeV) return  "2.4575";
    else      return  "4.3625";
  }
  if (tagger=="MVb") {
    if (a8TeV) return  "0.0874416666667";
    else      return  "-0.120991666667";
  }
  if (tagger=="SV1") {
    if (a8TeV) return  "0.0874416666667";
    else      return  "0.354833333333";
  }
  if (tagger=="JetFitter") {
    if (a8TeV) return  "-1.4655";
    else      return  "-1.6125"; ///"-1.215  , -2.3 on Z
  }
  
  cout << "NOT SUPPORTED!!! " << endl;
  return "0";
}

string getVariable(string tagger, bool a8TeV) {
  if (tagger=="MV1")        return "jet_mv1";
  if (tagger=="MV1c")       return "jet_mv1c";
  if (tagger=="MV2c00")     return "jet_mv2c00";
  if (tagger=="MV2c10")     return "jet_mv2c10";
  if (tagger=="MV2c20")     return "jet_mv2c20";
  if (tagger=="MVb")        return "jet_mvb";
  if (tagger=="IP3D")       return "jet_ip3d_llr";
  if (tagger=="IP3D+SV1")   return  "jet_sv1ip3d";
  if (tagger=="SV1")        return  "jet_sv1_llr";
  if (tagger=="JetFitter")  return  "jet_jf_llr";
  cout << "NOT SUPPORTED!!! " << endl;
  return "0";
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Get3Efficiency(string file1, 
		    string cutBase1, string cutBase2, string cutBase3, string cutBase4,
		    string Cut1 , string Cut2 , string Cut3 , string Cut4,
		    string varName, string varLabel,
		    string yLabel , string ofileName, 
		    int nBin, float Max, float Min) {
  
  TCanvas* can=new TCanvas( (ofileName).c_str(), (ofileName).c_str(), 800, 600);
  float numC;
  
  
  TH1D* mainH=new TH1D("histo_den", "2", nBin, Max, Min);
  mainH->Reset();
  mainH->SetTitle( (";"+varLabel+";"+yLabel+";").c_str() );
  mainH->SetMaximum(1.25);
  mainH->SetMinimum(0.0);
  mainH->Draw("HIST");
  mainH->SetDirectory(0);
  if ( ofileName.find("__0")!=string::npos ) mainH->SetMaximum(1.0);
  
  if ( ofileName.find("JetFitterFull")!=string::npos ) {
    if ( ofileName.find("__0")!=string::npos ) mainH->SetMaximum(0.15);
    if ( ofileName.find("__4")!=string::npos ) mainH->SetMaximum(0.60);
  }

  // get the file (with caching)
  if (theChain==0) {
    theChain=new TChain("bTag");
    cout << " OPENING FILE: " << file1 << endl;
    if ( file1.find("root")!=string::npos )  {
      theChain->Add( file1.c_str() );
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
	  theChain->Add( (file1+"/"+foldName+"/"+fName).c_str() );
	}
      }
    }
    cout << "TOTAL number of events is: " << theChain->GetEntries() << endl;     
  }

  TGraphAsymmErrors* gra1=GetEfficiency( theChain,
					 varName, 
					 cutBase1,  Cut1, 
					 varLabel, yLabel,
					 nBin, Max, Min, numC);
  gra1->SetLineColor(2);
  gra1->SetMarkerColor(2);
  gra1->Draw("P");

  
  TGraphAsymmErrors* gra2=GetEfficiency( theChain,
					 varName, 
					 cutBase2,  Cut2, 
					 varLabel, yLabel,
					 nBin, Max, Min, numC);
  gra2->SetLineColor(8);
  gra2->SetMarkerColor(8);
  gra2->Draw("P");
  
  TGraphAsymmErrors* gra3=NULL;
  if (Cut3!="") {
    gra3=GetEfficiency( theChain,
			varName, 
			cutBase3,  Cut3, 
			varLabel, yLabel,
			nBin, Max, Min, numC);
    gra3->SetLineColor(4);
    gra3->SetMarkerColor(4);
    gra3->Draw("P");
  }

  TGraphAsymmErrors* gra4=NULL;
  if (Cut4!="") {
    gra4=GetEfficiency( theChain,
			varName, 
			cutBase4,  Cut4, 
			varLabel, yLabel,
			nBin, Max, Min, numC);
    gra4->SetLineColor(6);
    gra4->SetMarkerColor(6);
    gra4->Draw("P");
  }
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
  legend4->AddEntry(gra1 , label1.c_str(),"lpe");
  legend4->AddEntry(gra2 , label2.c_str(),"lpe");
  if (Cut3!="") legend4->AddEntry(gra3 , label3.c_str(),"lpe");
  if (Cut4!="") legend4->AddEntry(gra4 , label4.c_str(),"lpe");
  legend4->Draw("SAME");
 
  if (ofileName.find("__0")!=string::npos) myText(0.20,0.90,1,"light jets",0.05);
  if (ofileName.find("__4")!=string::npos) myText(0.20,0.90,1,"c-jets",0.05);
  if (ofileName.find("__5")!=string::npos) myText(0.20,0.90,1,"b-jets",0.05);
  
  gra1->SetName( (ofileName+"__A").c_str() );
  gra1->Write();
  gra2->SetName( (ofileName+"__B").c_str() );
  gra2->Write();
  if (Cut3!="") {
    gra3->SetName( (ofileName+"__C").c_str() );
    gra3->Write();
  }
  if (Cut4!="") {
    gra4->SetName( (ofileName+"__D").c_str() );
    gra4->Write();
  }
  
  string varToPrint=outFolder+"/"+ofileName+".eps";
  can->SaveAs( varToPrint.c_str() );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PrintEffs(string file1, string jetType, string configuration)  {
  /// even more ugly
  
  string cutBase=" abs(jet_eta)<2.5 && jet_pt>25e3 && jet_truthMatch==1 ";
  cutBase+=" && jet_truthflav=="+jetType;

  string yLabel="@70% eff.";

  if ( configuration=="JetFitterReco" ) {
    label1=" all. ";
    label2=" only 1t vtx. ";
    label3=" only normal vtx.";
    label4=" both vertex";
    if (jetType=="5") title="b-jets";
    else              title="light-jets";

    cut1=cutBase;
    cut2=cutBase;
    cut3=cutBase;
    cut4=cutBase;
    
    yLabel="JetFitter reco eff.";

    Ecut1=" && (jet_jf_nvtx>0 || jet_jf_nvtx1t>0) ";
    Ecut2=" && (jet_jf_nvtx==0 && jet_jf_nvtx1t>0) ";
    Ecut3=" && (jet_jf_nvtx>0 && jet_jf_nvtx1t==0) ";
    Ecut4=" && (jet_jf_nvtx>0 && jet_jf_nvtx1t>0) ";

  } else if (configuration=="JetFitterFull") {
    label1=" all. ";
    label2=" only 1t vtx. ";
    label3=" only normal vtx.";
    label4=" both vertex";
    if (jetType=="5") title="b-jets";
    else              title="light-jets";

    cut1=cutBase;
    cut2=cutBase;
    cut3=cutBase;
    cut4=cutBase;
    
    yLabel="JetFitter full eff.";

    Ecut1=" && (jet_jf_nvtx>0 || jet_jf_nvtx1t>0) && jet_jf_llr>-1.6125 ";
    Ecut2=" && (jet_jf_nvtx==0 && jet_jf_nvtx1t>0) && jet_jf_llr>-1.6125 ";
    Ecut3=" && (jet_jf_nvtx>0 && jet_jf_nvtx1t==0) && jet_jf_llr>-1.6125 ";
    Ecut4=" && (jet_jf_nvtx>0 && jet_jf_nvtx1t>0) && jet_jf_llr>-1.6125 ";

  } else if (configuration=="JetFitterID") {
    label1=" all. ";
    label2=" only 1t vtx. ";
    label3=" only normal vtx.";
    label4=" both vertex";
    if (jetType=="5") title="b-jets";
    else              title="light-jets";

    cut1=cutBase+" && (jet_jf_nvtx>0 || jet_jf_nvtx1t>0)  ";
    cut2=cutBase+" && (jet_jf_nvtx==0 && jet_jf_nvtx1t>0) ";
    cut3=cutBase+" && (jet_jf_nvtx>0 && jet_jf_nvtx1t==0) ";
    cut4=cutBase+" && (jet_jf_nvtx>0 && jet_jf_nvtx1t>0)  ";
    
    yLabel="JetFitter 'ID' eff.";

    Ecut1=" && jet_jf_llr>-1.6125 ";
    Ecut2=" && jet_jf_llr>-1.6125 ";
    Ecut3=" && jet_jf_llr>-1.6125 ";
    Ecut4=" && jet_jf_llr>-1.6125 ";
    
  } else if (configuration=="SV1Reco" ) {
    label1=" all. ";
    label2=" 2t vertex ";
    label3=" >2t vertex ";
    label4=" both vertex";
    if (jetType=="5") title="b-jets";
    else              title="light-jets";

    cut1=cutBase;
    cut2=cutBase;
    cut3=cutBase;
    cut4=cutBase;
    
    yLabel="SV1 reco eff.";

    Ecut1=" && (jet_sv1_Nvtx>0 ) ";
    Ecut2=" && (jet_sv1_ntrkv==2 ) ";
    Ecut3=" && (jet_sv1_ntrkv>2 )";
    Ecut4="";

  } else {
    return;
  }
  
  
  Get3Efficiency(file1,
		 cut1, cut2, cut3, cut4,
		 Ecut1, Ecut2, Ecut3, Ecut4,
		 "jet_pt/1e3", "jet p_{T} (GeV)", 
		 yLabel, configuration+"__jet_pt__"+jetType,
		 40,  0, 500);

  /*
  Get3Efficiency(file1,
		 cut1, cut2, cut3, cut4,
		 Ecut1, Ecut2, Ecut3, Ecut4,
		 "abs(jet_eta)", "jet #eta", 
		 yLabel, configuration+"__jet_eta__"+jetType,
		 25, 0, 2.5);
  */

  if (jetType=="5") {
    Get3Efficiency(file1,
		   cut1, cut2, cut3, cut4,
		   Ecut1, Ecut2, Ecut3, Ecut4,
		   "bH_Lxy", "b-hadron transverse decay length (mm)",
		   yLabel, configuration+"__jet_bH__"+jetType,
		   40,  0, 80);
  }

}
  


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Plotter_pt2(const char* infile="tmp2.root",  //can also be a folder
		 const char* outfolder="test") {
  gStyle->SetOptStat(0);
  SetAtlasStyle();
  string file1=infile;
  outFolder=outfolder;

  if (outF!=0) {
    outF->Close();
    outF=0;
  }
  gSystem->Exec( ("mkdir -p "+outFolder).c_str());
  outF=new TFile( (outFolder+"/JFefficiencies.root").c_str(),"RECREATE");
  cout << "Created file: " << outF->GetName() << endl;
   

  PrintEffs(file1,"5","JetFitterReco");
  PrintEffs(file1,"4","JetFitterReco");  
  PrintEffs(file1,"0","JetFitterReco");

  PrintEffs(file1,"5","JetFitterID");
  PrintEffs(file1,"4","JetFitterID");  
  PrintEffs(file1,"0","JetFitterID");

  PrintEffs(file1,"5","JetFitterFull");
  PrintEffs(file1,"4","JetFitterFull");  
  PrintEffs(file1,"0","JetFitterFull");

  /*
  PrintEffs(file1,"5","SV1Reco");
  PrintEffs(file1,"4","SV1Reco");  
  PrintEffs(file1,"0","SV1Reco");  
  */

  delete theChain;
  theChain=0;
  
  outF->Close();
}
