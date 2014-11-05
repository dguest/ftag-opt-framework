#include <string>

#include "/home/vd1007/atlasstyle-00-03-05/AtlasStyle.C"
#include "/home/vd1007/atlasstyle-00-03-05/AtlasUtils.C"

//TTree* myT_1;
//TTree* myT_2;
TChain* myT_1;
TChain* myT_2;
TChain* myT_3;
TChain* myT_4;

float ev_1=0;
float ev_2=0;
float ev_3=0;
float ev_4=0;

bool normalize=true;//false;//true;//false;

bool use3=false;
bool use4=false;

string newCut1="";
string newCut2="";
string newCut3="";

bool doMuon=true;

void plotVariable(string varName, string varName2, string varLabel, string moreCuts, int nBin, float Max, float Min, bool bquark=true) {
  TCanvas* can=new TCanvas(varLabel.c_str(),varLabel.c_str(),800,600);
  
  TH1D* H_1  =new TH1D("histo_1", "1", nBin, Max, Min); H_1->Sumw2();
  TH1D* H_2  =new TH1D("histo_2", "2", nBin, Max, Min); H_2->Sumw2();
  TH1D* H_3;
  TH1D* H_4;
  if (use3) {H_3=new TH1D("histo_3", "3", nBin, Max, Min); H_3->Sumw2();}
  if (use4) {H_4=new TH1D("histo_4", "4", nBin, Max, Min); H_4->Sumw2();}
  
  string cut_incl=" "+moreCuts;
  string tmpCut1=newCut1;
  string tmpCut2=newCut2;

  if (moreCuts!="") {
    tmpCut1+= " && jet_trueFlav=="+moreCuts;
    tmpCut2+= " && jet_truthflav=="+moreCuts;
    cout << tmpCut1 << endl;
    cout << tmpCut2 << endl;
  }

  myT_1->Draw( (varName +">>histo_1").c_str(), tmpCut1.c_str(),"goff");
  myT_2->Draw( (varName2+">>histo_2").c_str(), tmpCut2.c_str(),"goff");
  if (use3) myT_3->Draw( (varName2+">>histo_3").c_str(), newCut3.c_str(),"goff");
  if (use4) myT_4->Draw( (varName2+">>histo_4").c_str(), newCut2.c_str(),"goff");
  
  ev_1= H_1->Integral();
  ev_2= H_2->Integral();
  if (use3) ev_3= H_3->Integral();
  if (use4) ev_4= H_4->Integral();
  
  TPad* pad_1=NULL;
  TPad* pad_2=NULL;
  pad_1 = new TPad("pad_1", "up", 0., 0.35, 1., 1.);
  pad_1->SetBottomMargin(0);
  pad_1->Draw();
  
  pad_2= new TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35);
  pad_2->SetTopMargin(0);
  pad_2->SetBottomMargin(0.25);
  pad_2->SetGridx();
  pad_2->SetGridy();
  pad_2->Draw();
  pad_1->cd();

  H_1->SetTitle( (";"+varLabel+";").c_str() );
  H_1->SetLineColor(1);
  H_1->SetLineWidth(4);
  H_1->SetMarkerSize(1.2);
  H_1->SetMarkerColor(1);
  float maxV=H_1->GetMaximum()/H_1->Integral()*1.2;
  H_1->SetBinContent(1, H_1->GetBinContent(0)+H_1->GetBinContent(1));
  H_1->SetBinError(1, sqrt(pow(H_1->GetBinError(0),2)+pow(H_1->GetBinError(1),2)));
  H_1->SetBinContent(H_1->GetNbinsX(), H_1->GetBinContent(H_1->GetNbinsX())+H_1->GetBinContent(H_1->GetNbinsX()+1));
  H_1->SetBinError(H_1->GetNbinsX(), sqrt(pow(H_1->GetBinError(H_1->GetNbinsX()),2)+pow(H_1->GetBinError(H_1->GetNbinsX()+1),2)));
  if (normalize) H_1->Scale( 1./H_1->Integral() );
  H_1->GetXaxis()->SetTitle(varLabel.c_str());
  H_1->SetMaximum(maxV);
  H_1->Draw("E");

  H_2->SetLineColor(2);
  H_2->SetLineWidth(3);
  H_2->SetMarkerSize(0.7);
  H_2->SetMarkerColor(2);
  H_2->SetBinContent(1, H_2->GetBinContent(0)+H_2->GetBinContent(1));
  H_2->SetBinError(1, sqrt(pow(H_2->GetBinError(0),2)+pow(H_2->GetBinError(1),2)));
  H_2->SetBinContent(H_2->GetNbinsX(), H_2->GetBinContent(H_2->GetNbinsX())+H_2->GetBinContent(H_2->GetNbinsX()+1));
  H_2->SetBinError(H_2->GetNbinsX(), sqrt(pow(H_2->GetBinError(H_2->GetNbinsX()),2)+pow(H_2->GetBinError(H_2->GetNbinsX()+1),2)));
  if (normalize) H_2->Scale( 1./H_2->Integral() );
  H_2->Draw("SAMEE");

  if (use3) {
    H_3->SetLineColor(8);
    H_3->SetLineWidth(3);
    H_3->SetMarkerSize(0.7);
    H_3->SetMarkerColor(8);
    H_3->SetBinContent(1, H_3->GetBinContent(0)+H_3->GetBinContent(1));
    H_3->SetBinError(1, sqrt(pow(H_3->GetBinError(0),2)+pow(H_3->GetBinError(1),2)));
    H_3->SetBinContent(H_3->GetNbinsX(), H_3->GetBinContent(H_3->GetNbinsX())+H_3->GetBinContent(H_3->GetNbinsX()+1));
    H_3->SetBinError(H_3->GetNbinsX(), sqrt(pow(H_3->GetBinError(H_3->GetNbinsX()),2)+pow(H_3->GetBinError(H_3->GetNbinsX()+1),2)));
    if (normalize) H_3->Scale( 1./H_3->Integral() );
    H_3->Draw("SAMEE");
  }

  if (use4) {
    H_4->SetLineColor(4);
    H_4->SetLineWidth(3);
    H_4->SetMarkerSize(0.7);
    H_4->SetMarkerColor(4);
    H_4->SetBinContent(1, H_4->GetBinContent(0)+H_4->GetBinContent(1));
    H_4->SetBinError(1, sqrt(pow(H_4->GetBinError(0),2)+pow(H_4->GetBinError(1),2)));
    H_4->SetBinContent(H_4->GetNbinsX(), H_4->GetBinContent(H_4->GetNbinsX())+H_4->GetBinContent(H_4->GetNbinsX()+1));
    H_4->SetBinError(H_4->GetNbinsX(), sqrt(pow(H_4->GetBinError(H_4->GetNbinsX()),2)+pow(H_4->GetBinError(H_4->GetNbinsX()+1),2)));
    if (normalize) H_4->Scale( 1./H_4->Integral() );
    
    H_4->Draw("SAMEE");
  }
  
  TLegend *leg = new TLegend(0.70,0.65,0.92,0.92,NULL,"brNDC");
  leg->SetBorderSize(1);
  leg->SetTextFont(62);
  leg->SetLineColor(0);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->AddEntry(H_1,"rel. 17","l");
  leg->AddEntry(H_2,"rel. 19","l");
  if (use3) leg->AddEntry(H_3,"WW->llqq","l");
  if (use4) leg->AddEntry(H_4,"Sherpa Masless","l");
  leg->Draw("SAME");

  pad_2->cd();
  TH1D* ratioH=new TH1D(*H_2);
  TH1D* ratioH3;
  TH1D* ratioH4;
  TH1D* ratioH5;
  if (use3) ratioH3=new TH1D(*H_3);
  if (use4) {
    ratioH4=new TH1D(*H_4);
    //ratioH5=new TH1D(*H_5);
  } 

  ratioH->Divide(H_1);
  if (use3) ratioH3->Divide(H_1);
  if (use4) {
    ratioH4->Divide(H_1);
    //ratioH5->Divide(H_1);
  }

  if ( varName.find("true")!=string::npos ) {
    ratioH->SetMinimum( 0.6 ); // 0.5
    ratioH->SetMaximum( 1.4 ); // 1.5
  } else {
    ratioH->SetMinimum( 0.7 ); // 0.8
    ratioH->SetMaximum( 1.3 ); // 1.2
  }
  ratioH->GetYaxis()->SetNdivisions(509);
  ratioH->GetYaxis()->SetTitle("1/black");
  ratioH->GetYaxis()->SetTitleOffset(0.45); //0.35
  ratioH->GetYaxis()->SetTitleSize(0.11);
  ratioH->GetYaxis()->SetLabelSize(0.10);   
  ratioH->GetXaxis()->SetTitle(varLabel.c_str());
  ratioH->GetXaxis()->SetTitleOffset(0.8);
  ratioH->GetXaxis()->SetTitleSize(0.12);
  ratioH->GetXaxis()->SetLabelSize(0.11);
  ratioH->SetLineWidth(3);
  ratioH->SetMarkerSize(1.2);
  ratioH->Draw("E");    
  ratioH->GetYaxis()->Draw();
    
  TLine* line1=new TLine(ratioH->GetXaxis()->GetXmin(),1.0,ratioH->GetXaxis()->GetXmax(),1.0);
  line1->SetLineColor(kGray+1);
  line1->SetLineWidth(2);
  line1->Draw("E");
  ratioH->Draw("SAMEE");    
  if (use3) ratioH3->Draw("SAMEE");    
  if (use4) {
    ratioH4->Draw("SAMEE");    
    //ratioH5->Draw("SAMEE");
  }

  string newName=varName;
  if ( newName.find("lep_pt")!=string::npos )  newName="lep_pt";
  if ( newName.find("jet_pt[0]" )!=string::npos ) newName="jet0_pt";
  if ( newName.find("jet_eta[0]")!=string::npos ) newName="jet0_eta";
  if ( newName.find("jet_pt[3]" )!=string::npos ) newName="jet3_pt";
  if ( newName.find("jet_eta[3]")!=string::npos ) newName="jet3_eta";

  if ( newName.find("met_et")!=string::npos ) newName="met";
  if ( newName.find("mwt"   )!=string::npos ) newName="mwt";

  //can->Print( ("WWPlots/"+newName+".eps").c_str() );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void QuickComparison() {

  gStyle->SetOptStat(0);
  SetAtlasStyle();

  myT_1=new TChain("bTag");
  myT_2=new TChain("bTag");

  myT_1->Add( "../../../TestD3PDs/SlimmedNtuples_V10/ttbar.root");
  myT_2->Add( "../../files/ttbar.root");
  //myT_3->Add( "/storage/groups/atljak/ValerioLNUJJ/SigSel/181971_/data-EventSelection/181971.root");


  newCut1=" jet_pt>25e3 && jet_truthmatched==1 ";
  newCut2=" jet_pt>25e3 && jet_truthMatch==1   ";
  newCut3="(DibosonDecay!=0 && nJets==2 && MET_RefFinal_et_corrected/1e3>30 && WlepCand_mT_corrected/1e3>=40)*evt_lumiWeight*evt_MCPileupWeight_allPeriods*evt_MCVxpZWeight";
  
  string newCut="";
  
  //plotVariable("jet_trueFlav","jet_truthflav", " jet trueFlav" , newCut ,  20,  0, 16);

  //plotVariable("jet_eta","jet_eta", " jet eta" , newCut ,  100,  -2.5, 2.5);
  
  plotVariable("jet_pt/1e3","jet_pt/1e3", "b-jet p_{T} (GeV)" , "5" ,  100,  20, 220);
  plotVariable("jet_pt/1e3","jet_pt/1e3", "light-jet p_{T} (GeV)" , "0" ,  100,  20, 220);

  //plotVariable("jet_sv1","jet_sv1_llr", " b-jet SV1 w" , "5" ,  100,  -5., 14.);
  //plotVariable("jet_sv1","jet_sv1_llr", " light-jet SV1 w" , "0" ,  100,  -5., 14.);

  //plotVariable("jet_ip3d","jet_ip3d_llr", " b-jet IP3D w" , "5" ,  100,  -12., 30.);
  //plotVariable("jet_ip3d","jet_ip3d_llr", " light-jet IP3D w" , "0" ,  100,  -12., 30.);

  /*
  plotVariable("jet_n"      ,"nJets", "# jets"          , newCut ,  3,  1.5, 4.5);
  //plotVariable("nJet_tagged","# tagged jets"   , newCut ,  4, -0.5, 3.5);

  plotVariable("lep_pt/1e3","l1Cand_pt/1e3", "lepton p_T (GeV)", newCut , 60, 20, 200);
  plotVariable("lep_eta"   ,"l1Cand_eta"   , "lepton #eta"     , newCut , 50,-2.5, 2.5); 
  //plotVariable("met_et/1e3","MET_RefFinal_et/1e3"          ,  "MET (GeV)"     , newCut , 80, 0, 200);
  plotVariable("met_et/1e3","MET_RefFinal_et_corrected/1e3",  "MET corr(GeV)" , newCut , 50, 0, 200);
  //plotVariable("mwt/1e3"   ,"WlepCand_mT/1e3"          , "Mwt (GeV)"     , newCut , 50, 0, 200);  
  plotVariable("mwt/1e3"   ,"WlepCand_mT_corrected/1e3", "Mwt corr(GeV)" , newCut , 50, 0, 200);  
  
  plotVariable("pT_jet1"  , "j1_pt/1e3", "lead. jet pt (GeV)"    , newCut , 70, 20, 250);
  plotVariable("jet0_eta" , "j1_eta"   , "lead. jet #eta"        , newCut , 50,-2.5, 2.5);
  plotVariable("pT_jet2"  , "j2_pt/1e3", "sub lead. jet pt (GeV)", newCut , 60, 20, 120);
  plotVariable("jet1_eta" , "j2_eta"   , "sub lead. jet #eta"    , newCut , 50,-2.5, 2.5);
  plotVariable("fabs(jet0_eta-jet1_eta)", "deltaEta_j1_j2", "dEta_{j1j2}"    , newCut , 40,0, 3.5);
  
  plotVariable("acos(cos(lep_phi-met_phi))", "WlCand_dPhiMET_corrected", "dPhi(l,MET) corr", newCut , 50,0, 3.14);
  
  plotVariable("acos(cos(lep_phi-met_phi))", "deltaR_j1_j2", "dR(j1,j2)", newCut , 50,0, 4.5);

  //plotVariable("acos(cos(lep_phi-met_phi))", "WlCand_dPhiMET", "dPhi(l,MET)", newCut , 50,0, 3.14);
  plotVariable("acos(cos(lep_phi-met_phi))", "deltaPhi_j1_j2", "dPhi(j1,j2)", newCut , 40,0, 3.14);
  ///////////plotVariable("mu", "averageIntPerXing", "<mu>", newCut , 80, 0, 40);
  
  plotVariable("mjj" , "mjj/1e3" , "m_{jj} (GeV) something" , newCut , 50,0, 250);
  plotVariable("pTjj" , "pTjj/1e3" , "pT_{jj} (GeV) something" , newCut , 30,0, 500);
  */

}
