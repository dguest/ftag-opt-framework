#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include <sstream>

#include <dirent.h>

#include "AtlasStyle.C"
#include "AtlasUtils.C"
//#include "Chaining.C"
//#include "parameters.C"

using namespace std;

TChain* myT_1;
TChain* myT_2;
TFile* outfile;

void histcompseparate(string file1, string file2, string output) {

  gStyle->SetOptStat(0);
  SetAtlasStyle();

  outfile = new TFile((output+"/data.root").c_str(),"RECREATE");
  TList* hList = new TList();

// Chose the functions created using make_TChain_Local.sh here to the ones of interest

//  myT_1 = NTUPTest();

  if (myT_1==0) {
    myT_1=new TChain("bTag");
    cout << " OPENING FILE: " << file1 << endl;
    if ( file1.find("root")!=string::npos )  {
      myT_1->Add( file1.c_str() );
    } else {
      cout << "Input is a directory: ging fancy: " << endl;
      DIR*     dir;
      dirent*  pdir;
      dir = opendir( file1.c_str() );     // open current directory
      while (pdir = readdir(dir))  {
	string foldName=pdir->d_name;
	cout << pdir->d_name << endl;
	DIR*     dir2;
	dirent*  pdir2;
	dir2 = opendir( (file1+"/"+foldName).c_str() );     // open current directory
	while (pdir2 = readdir(dir2))  {
	  string fName=pdir2->d_name;
	  if (fName.find("root")==string::npos) continue;
	  myT_1->Add( (file1+"/"+foldName+"/"+fName).c_str() );
	}
      }
    }
  }

  if (myT_2==0) {
    myT_2=new TChain("bTag");
    cout << " OPENING FILE: " << file2 << endl;
    if ( file1.find("root")!=string::npos )  {
      myT_2->Add( file2.c_str() );
    } else {
      cout << "Input is a directory: ging fancy: " << endl;
      DIR*     dir;
      dirent*  pdir;
      dir = opendir( file2.c_str() );     // open current directory
      while (pdir = readdir(dir))  {
	string foldName=pdir->d_name;
	cout << pdir->d_name << endl;
	DIR*     dir2;
	dirent*  pdir2;
	dir2 = opendir( (file2+"/"+foldName).c_str() );     // open current directory
	while (pdir2 = readdir(dir2))  {
	  string fName=pdir2->d_name;
	  if (fName.find("root")==string::npos) continue;
	  myT_2->Add( (file2+"/"+foldName+"/"+fName).c_str() );
	}
      }
     }
   }

const int npar = 2; //Set equal to number of plotting variables
int nvar = 0;

string parr17[npar]; string parr19[npar]; double nbin[npar]; double xmin[npar]; double xmax[npar];

//Pick which parameters need ploting

//parr19[nvar] = "truthflav"; 	 parr17[nvar] = "trueFlav"; 	  xmin[nvar] = -0.5; xmax[nvar] = 16.5; nbin[nvar]= 17; nvar += 1;
//parr19[nvar] = "truthMatch"; 	 parr17[nvar] = "truthmatched";	  xmin[nvar] = 0.;   xmax[nvar] = 2;    nbin[nvar]= 10; nvar += 1;
//parr19[nvar] = "aliveAfterOR"; 	 parr17[nvar] = "aliveAfterOR";	  xmin[nvar] = 0.;   xmax[nvar] = 2;    nbin[nvar]= 10; nvar += 1;

//parr19[nvar] = "E"; 	  parr17[nvar] = "E"; 	  xmin[nvar] = 0.;   xmax[nvar] = 700000; nbin[nvar]= 50; nvar += 1;
parr19[nvar] = "pt";	  parr17[nvar] = "pt"; 	  xmin[nvar] = 0.;   xmax[nvar] = 500000; nbin[nvar]= 20; nvar += 1;
parr19[nvar] = "eta";	  parr17[nvar] = "eta";	  xmin[nvar] = -3.;  xmax[nvar] = 3.0;    nbin[nvar]= 20; nvar += 1;
//parr19[nvar] = "truthPt"; parr17[nvar] = "pt"; 	  xmin[nvar] = 0.;   xmax[nvar] = 500000; nbin[nvar]= 40; nvar += 1;

/*//parr19[nvar]= "ip2d_pb"; parr17[nvar] = "ip2d";  xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;
//parr19[nvar]= "ip2d_pc"; parr17[nvar] = "ip2d";  xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;
//parr19[nvar]= "ip2d_pu"; parr17[nvar] = "ip2d";  xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;
//parr19[nvar]= "ip2d_llr"; parr17[nvar] = "ip2d"; xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;

//parr19[nvar]= "ip3d_pb"; parr17[nvar] = "ip3d";  xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;
//parr19[nvar]= "ip3d_pc"; parr17[nvar] = "ip3d";  xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;
//parr19[nvar]= "ip3d_pu"; parr17[nvar] = "ip3d";  xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;
parr19[nvar]= "ip3d_llr"; parr17[nvar] = "ip3d"; xmin[nvar] = -15.; xmax[nvar] = 30;     nbin[nvar]= 50; nvar += 1;

parr19[nvar] = "sv0_sig3d"; parr17[nvar] = "sv0"; xmin[nvar] = -0.5; xmax[nvar] = 10.5;    nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
/*parr19[nvar] = "sv0_ntrkj"; parr17[nvar] = "sv0_ntrkj"; xmin[nvar] = -1.5; xmax[nvar] = 11.5;    nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
parr19[nvar] = "sv0_ntrkv"; parr17[nvar] = "sv0_ntrkv"; xmin[nvar] = -1.5; xmax[nvar] = 11.5;    nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
parr19[nvar] = "sv0_m";     parr17[nvar] = "sv0_mass"; xmin[nvar] = 0.0; xmax[nvar] = 6000;    nbin[nvar]= 50; nvar += 1;
parr19[nvar] = "sv0_efc";   parr17[nvar] = "sv0_efrc";  xmin[nvar] = 0.0;  xmax[nvar] = 1.;      nbin[nvar]= 50; nvar += 1;
parr19[nvar] = "sv0_n2t";   parr17[nvar] = "sv0_n2t";	xmin[nvar] = -0.5;	   xmax[nvar] = 30.5;	 nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
//parr19[nvar] = "sv0_normdist";     parr17[nvar] = "sv0_ntrkv"; xmin[nvar] = -0.5; xmax[nvar] = 10.5;    nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;

parr19[nvar] = "sv1_ntrkj"; 	 parr17[nvar] = "sv1_ntrkj"; 	  xmin[nvar] = -0.5;   xmax[nvar] = 15.5; nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
parr19[nvar] = "sv1_ntrkv"; 	 parr17[nvar] = "sv1_ntrkv"; 	  xmin[nvar] = -0.5;   xmax[nvar] = 15.5; nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
parr19[nvar] = "sv1_m"; 	 parr17[nvar] = "sv1_mass"; 	  xmin[nvar] = 0.;   xmax[nvar] = 6000; nbin[nvar]= 50; nvar += 1;
parr19[nvar] = "sv1_efc"; 	 parr17[nvar] = "sv1_efrc"; 	  xmin[nvar] = 0.;   xmax[nvar] = 1; nbin[nvar]= 10; nvar += 1;
parr19[nvar] = "sv1_n2t"; 	 parr17[nvar] = "sv1_n2t";   	  xmin[nvar] = 0.;   xmax[nvar] = 30; nbin[nvar]= 15; nvar += 1;
//parr19[nvar] = "sv1_normdist"; 	 parr17[nvar] = "sv1_"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "sv1_pb"; 	 parr17[nvar] = "sv1_"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "sv1_pc"; 	 parr17[nvar] = "sv1_"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "sv1_pu"; 	 parr17[nvar] = "sv1_"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;
parr19[nvar] = "sv1_llr"; 	 parr17[nvar] = "sv1"; 	 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "sv1_vtxx"; 	 parr17[nvar] = "sv1_"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "sv1_vtxy"; 	 parr17[nvar] = "sv1_"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "sv1_vtxz"; 	 parr17[nvar] = "sv1_"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15; nbin[nvar]= 50; nvar += 1;

//parr19[nvar] = "jf_pb";		parr17[nvar] = "jfit_";   	xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_pc";		parr17[nvar] = "jfit_";		xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_pu";  	parr17[nvar] = "jfit_";   	xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
/*parr19[nvar] = "jf_m"; 		parr17[nvar] = "jfit_mass"; 	  xmin[nvar] = 0.;   xmax[nvar] = 9000; nbin[nvar]= 50; nvar += 1;
parr19[nvar] = "jf_efc"; 	parr17[nvar] = "jfit_efrc"; 	  xmin[nvar] = 0.;   xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
//parr19[nvar] = "jf_deta";  	parr17[nvar] = "jfit_";   	xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_dphi";  	parr17[nvar] = "jfit_";  	xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
parr19[nvar] = "jf_ntrkAtVx";  	parr17[nvar] = "jfit_ntrkAtVx";   	xmin[nvar] = -0.5;   xmax[nvar] = 15.5;   nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
parr19[nvar] = "jf_nvtx";  	parr17[nvar] = "jfit_nvtx";   	xmin[nvar] = -0.5;   xmax[nvar] = 5.5;   nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
parr19[nvar] = "jf_sig3d";  	parr17[nvar] = "jfit_sig3d";   	xmin[nvar] = 0.;   xmax[nvar] = 100;   nbin[nvar]= 50; nvar += 1;
/*parr19[nvar] = "jf_nvtx1t";  	parr17[nvar] = "jfit_nvtx1t";  	xmin[nvar] = -0.5;   xmax[nvar] = 15.5;   nbin[nvar]= xmax[nvar] - xmin[nvar]; nvar += 1;
//parr19[nvar] = "jf_n2t";  	parr17[nvar] = "jfit_";   	xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_llr";  	parr17[nvar] = "jfit_"; xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_chi2";	parr17[nvar] = "jfit_"; 	xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_ndf"; 	parr17[nvar] = "jfit_ndof"; 	xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;

//parr19[nvar] = "jf_jfcombnn_pb"; 	 parr17[nvar] = ""; 	  xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_jfcombnn_pc"; 	 parr17[nvar] = "";	  xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_jfcombnn_pu";	 parr17[nvar] = ""; 	  xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;
//parr19[nvar] = "jf_jfcombnn_llr"; 	 parr17[nvar] = "combNN"; 	  xmin[nvar] = 0.;   xmax[nvar] = 15;   nbin[nvar]= 50; nvar += 1;

//parr19[nvar] = "sv1ip3d";parr17[nvar] = "";    xmin[nvar] = 0; xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
parr19[nvar] = "mv1";    parr17[nvar] = "mv1";    xmin[nvar] = 0; xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
parr19[nvar] = "mv1c";   parr17[nvar] = "mv1c";   xmin[nvar] = 0; xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
/*parr19[nvar] = "mv2c00"; parr17[nvar] = "mv2c00"; xmin[nvar] = 0; xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
parr19[nvar] = "mv2c10"; parr17[nvar] = "mv2c10"; xmin[nvar] = 0; xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
parr19[nvar] = "mv2c20"; parr17[nvar] = "mc2c20"; xmin[nvar] = 0; xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
parr19[nvar] = "mvb";    parr17[nvar] = "mvb";    xmin[nvar] = 0; xmax[nvar] = 1;    nbin[nvar]= 10; nvar += 1;
*/

if(npar != nvar){

  cout << "Incorrect number of variables - check npar value." << endl;
 gROOT->ProcessLine(".q");

}

for(int i = 0; i < npar; i++){

  TH1D* H_normr17 = new TH1D("histo_normr17", "track number", 1, -100, 2000);
  myT_1->Draw( ("jet_"+parr19[i]+">>histo_normr17").c_str(), "jet_pt>25000 && abs(jet_eta) < 2.5" );
  Double_t normr17 = H_normr17->GetEntries();
  cout << normr17 << endl;

  TH1D* H_normr19 = new TH1D("histo_normr19", "track number", 1, -100, 2000);
  myT_2->Draw( ("jet_"+parr19[i]+">>histo_normr19").c_str(), "jet_pt>25000 && abs(jet_eta) < 2.5 ");
  Double_t normr19 = H_normr19->GetEntries();
  cout << normr19 << endl;

  int j = 0;

  while(j < 6){

    string jettype = "light";
    if (j == 4){ jettype = "c";}
    if (j == 5){ jettype = "b";}
    string jettype2 = parr19[i] + "_" + jettype;

    cout << jettype2 << endl;

    TCanvas* can = new TCanvas( (jettype2).c_str(), ( jettype2 ).c_str(),800,600);
    TLegend *leg = new TLegend(0.70,0.65,0.92,0.92,NULL,"brNDC");

    TH1D* H_1r17 = new TH1D(("histo_r17_"+jettype2).c_str(), "0", nbin[i], xmin[i], xmax[i]);
    H_1r17->Sumw2();
    H_1r17->SetLineColor(2);
    H_1r17->SetTitle( (parr19[i]).c_str() );
    H_1r17->GetYaxis()->SetTitle("Normalised Jet Fraction");
    H_1r17->GetXaxis()->SetTitle( (parr19[i]).c_str() );
    leg->AddEntry(H_1r17,("r20.1.0.3 "+jettype+"-jets").c_str(),"L");

    TH1D* H_1r19 = new TH1D(("histo_r19_"+jettype2).c_str(), "0", nbin[i], xmin[i], xmax[i]);
    H_1r19->Sumw2();
    H_1r19->SetLineColor(3);
    H_1r19->SetLineStyle(2);
    H_1r19->SetTitle( (parr19[i]).c_str() );
    H_1r19->GetYaxis()->SetTitle("Normalised Jet Fraction");
    H_1r19->GetXaxis()->SetTitle( (parr19[i]).c_str() );
    leg->AddEntry(H_1r19,("r20.1.4.1 "+jettype+"-jets").c_str(),"L");

    if(xmax[i] - xmin[i] == nbin[i]){ H_1r17->SetMarkerSize(0);H_1r19->SetMarkerSize(0);}
    else{ H_1r17->SetMarkerSize(0.5); H_1r19->SetMarkerSize(0.5);}

    cout << j << endl;

    string jstr;
    ostringstream convert;
    convert << j;
    jstr = convert.str();

    myT_1->Draw( ("jet_"+parr19[i]+">>histo_r17_"+jettype2).c_str(), ("jet_GhostL_HadF=="+jstr+"&& jet_pt>25000 && abs(jet_eta) < 2.5").c_str());
    myT_2->Draw( ("jet_"+parr19[i]+">>histo_r19_"+jettype2).c_str(), ("jet_GhostL_HadF=="+jstr+"&& jet_pt>25000 && abs(jet_eta) < 2.5").c_str());

    H_1r17->Scale(1/normr17);
    H_1r19->Scale(1/normr19);

    H_1r19->Draw();
    H_1r17->Draw("SAME");
    leg->Draw("SAME");

    can->Print( (output+"/"+jettype+"/"+parr19[i]+".eps").c_str() );
    can->Print( (output+"/"+jettype+"/"+parr19[i]+".png").c_str() );
    can->Print( (output+"/"+jettype+"/"+parr19[i]+".C").c_str() );

    hList->Add(can);
    hList->Add(H_1r17);
    hList->Add(H_1r19);

    if(j == 0) { j = 3; }
    j += 1;

  }

/*  can->Print( ("plots/r17r19/"+parr19[i]+".eps").c_str() );
  can->Print( ("plots/r17r19/"+parr19[i]+".png").c_str() );
  can->Print( ("plots/r17r19/"+parr19[i]+".C").c_str() );*/

  }

hList->Write();
outfile->Close();

}

