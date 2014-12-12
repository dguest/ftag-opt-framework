

{
  gROOT->ProcessLine(".L Plotter_pt2.C++g");

  cout << "##############################################################################################" << endl;

  //gROOT->ProcessLine("Plotter_pt2(\" ../../../TestD3PDs/SlimmedNtuples_V10/ttbar.root\",\"8TeVd3pd\")");
 
  //gROOT->ProcessLine("Plotter_pt2(\"../../files/Ztt.root\",\"13TeVXAOD_z\")");
  //gROOT->ProcessLine("Plotter_pt2(\"../../files/Ztt_NEW.root\",\"13TeVXAOD_z_new\")");
  gROOT->ProcessLine("Plotter_pt2(\"../../files/Ztt_NEW_ref.root\",\"13TeVXAOD_z_newRef\")");

  gROOT->ProcessLine(".q");
}
