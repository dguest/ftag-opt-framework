{
  gROOT->ProcessLine(".L Plotter_pt2.C++g");

  cout << "##############################################################################################" << endl;

  gROOT->ProcessLine("Plotter_pt2(\" ../../../TestD3PDs/SlimmedNtuples_V10/ttbar.root\",\"8TeVd3pd\")");
 

  gROOT->ProcessLine(".q");
}
