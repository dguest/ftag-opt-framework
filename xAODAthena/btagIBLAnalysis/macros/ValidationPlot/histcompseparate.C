#include "AtlasStyle.C"
#include "AtlasUtils.C"
#include "Chaining.C"

using namespace.std

void histcompseparate() {

  gStyle->SetOptStat(0);
  SetAtlasStyle();

  outfile = TFile::Open("$HOME/www/private/r19Validation/r17data2.root","RECREATE");
  TList* hList = new TList();

  myT_1 = NTUP();
  myT_2 = xAOD();

const int npar = 2;
string parr17[npar]; string parr19[npar]; double nbin[npar]; double xmin[npar]; double xmax[npar];


parr19[0] = "sv1_llr"; 	 parr17[0] = "sv1"; 	  xmin[0] = 0.;   xmax[0] = 15; nbin[0]= 50;
parr19[1] = "ip3d_llr"; 	 parr17[1] = "ip3d"; 	  xmin[1] = -15.;   xmax[1] = 30; nbin[1]= 50;
//parr19[0] = "E"; 	 parr17[0] = "E"; 	  xmin[0] = 0.;   xmax[0] = 700000; nbin[0]= 50;
//parr19[0] = "pt"; parr17[0] = "pt"; xmin[0] = 0.; xmax[0] = 500000; nbin[0]= 40;
/*parr19[0] = "sv0_ntrkv"; parr17[0] = "sv0_ntrkv"; xmin[0] = -0.5; xmax[0] = 10.5;    nbin[0]= xmax[0] - xmin[0];
parr19[1] = "sv0_efc";   parr17[1] = "sv0_efrc";  xmin[1] = 0.0;  xmax[1] = 1.;      nbin[1]= 50;
parr19[2] = "sv0_n2t";   parr17[2] = "sv0_n2t";	  xmin[2] = 0; 	  xmax[2] = 60;	     nbin[2]= xmax[2] - xmin[2];
parr19[3] = "pt"; 	 parr17[3] = "pt"; 	  xmin[3] = 0.;   xmax[3] = 500000;  nbin[3]= 40;
parr19[4] = "eta";	 parr17[4] = "eta"; 	  xmin[4] = -3.;  xmax[4] = 3.0;     nbin[4]= 40;
parr19[5] = "E"; 	 parr17[5] = "E"; 	  xmin[5] = 0.;   xmax[5] = 700000;  nbin[5]= 50;
//parr19[7] = "truthPt"; 	 parr17[7] = "pt"; 	  xmin[7] = 0.;   xmax[7] = 500000;  nbin[7]= 40;*/

for(int i = 0; i < npar; i++){

  TH1D* H_normr17 = new TH1D("histo_normr17", "track number", 1, -100, 2000);
  myT_1->Draw( ("jet_"+parr17[i]+">>histo_normr17").c_str(), "jet_pt>25000 && abs(jet_eta) < 2.5" );
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
    H_1r17->SetTitle( (parr17[i]).c_str() );
    H_1r17->GetYaxis()->SetTitle("Number of Events");
    H_1r17->GetXaxis()->SetTitle( (parr17[i]).c_str() );
    leg->AddEntry(H_1r17,("r17 "+jettype+"-jets").c_str(),"L");

    TH1D* H_1r19 = new TH1D(("histo_r19_"+jettype2).c_str(), "0", nbin[i], xmin[i], xmax[i]);
    H_1r19->Sumw2();
    H_1r19->SetLineColor(3);
    H_1r19->SetLineStyle(2);
    H_1r19->SetTitle( (parr19[i]).c_str() );
    H_1r19->GetYaxis()->SetTitle("Number of Events");
    H_1r19->GetXaxis()->SetTitle( (parr19[i]).c_str() );
    leg->AddEntry(H_1r19,("r19 "+jettype+"-jets").c_str(),"L");

    if(xmax[i] - xmin[i] == nbin[i]){ H_1r17->SetMarkerSize(0);H_1r19->SetMarkerSize(0);}
    else{ H_1r17->SetMarkerSize(0.5); H_1r19->SetMarkerSize(0.5);}

    cout << j << endl;

    string jstr;
    ostringstream convert;
    convert << j;
    jstr = convert.str();

    myT_1->Draw( ("jet_"+parr17[i]+">>histo_r17_"+jettype2).c_str(), ("jet_trueFlav=="+jstr).c_str() "&& jet_pt>25000 && abs(jet_eta) < 2.5" );
    myT_2->Draw( ("jet_"+parr19[i]+">>histo_r19_"+jettype2).c_str(), ("jet_truthflav=="+jstr).c_str() "&& jet_pt>25000 && abs(jet_eta) < 2.5");

    H_1r17->Scale(1/normr17);
    H_1r19->Scale(1/normr19);

    H_1r19->Draw();
    H_1r17->Draw("SAME");
    leg->Draw("SAME");

    can->Print( ("$HOME/www/private/r19Validation/"+jettype+"/"+parr19[i]+".eps").c_str() );
    can->Print( ("$HOME/www/private/r19Validation/"+jettype+"/"+parr19[i]+".png").c_str() );
    can->Print( ("$HOME/www/private/r19Validation/"+jettype+"/"+parr19[i]+".C").c_str() );

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

