{
  gROOT->ProcessLine(".L Plotter_pt2.C++g");
  //gROOT->ProcessLine(".L Plotter_pt3.C++g");

  cout << "##############################################################################################" << endl;

  //gROOT->ProcessLine("Plotter_pt2(\" ../../../TestD3PDs/SlimmedNtuples_V10/ttbar.root\",\"8TeVd3pd\")");
 
  //gROOT->ProcessLine("Plotter_pt2(\"../../files/Ztt.root\",\"13TeVXAOD_z\")");
  //gROOT->ProcessLine("Plotter_pt2(\"../../files/Ztt_NEW.root\",\"13TeVXAOD_z_new\")");
  //gROOT->ProcessLine("Plotter_pt2(\"../../files/Ztt_NEW_ref.root\",\"13TeVXAOD_z_newRef\")");

  //gROOT->ProcessLine("Plotter_pt2(\"../../files/DC14_ttbar/\",\"13TeVXAOD_DC14_tt\")");
  //gROOT->ProcessLine("Plotter_pt2(\"../../files/reTag_base_ttbar/\",\"13TeVXAOD_Base_tt\")");
  gROOT->ProcessLine("Plotter_pt2(\"../../files/reTag_base2_ttbar/\",\"13TeVXAOD_Base2_tt\")");

  gROOT->ProcessLine(".q");
}
