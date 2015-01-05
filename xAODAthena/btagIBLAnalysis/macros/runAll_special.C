{
  //gROOT->ProcessLine(".L Plotter_pt2.C++g");
  gROOT->ProcessLine(".L Plotter_pt3.C++g");

  cout << "##############################################################################################" << endl;

  // gROOT->ProcessLine("Plotter_pt3(\"../../files/DC14_ttbar/\",\"13TeVXAOD_DC14_tt\")");
  //gROOT->ProcessLine("Plotter_pt3(\"../../files/reTag_base2_ttbar/\",\"13TeVXAOD_Base2_tt\")");


  gROOT->ProcessLine("Plotter_pt3(\"../../files/reTag_base2_ttbar_kRefs/\",\"13TeVXAOD_Base2_tt_kRef\")");
  
  gROOT->ProcessLine(".q");
}
