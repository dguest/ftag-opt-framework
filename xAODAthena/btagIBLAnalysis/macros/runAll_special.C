{
  //gROOT->ProcessLine(".L Plotter_pt2.C++g");
  //gROOT->ProcessLine(".L Plotter_pt3_cjets.C++g");
  gROOT->ProcessLine(".L Plotter_pt3.C++g");

  cout << "##############################################################################################" << endl;

  // gROOT->ProcessLine("Plotter_pt3(\"../../files/DC14_ttbar/\",\"13TeVXAOD_DC14_tt\")");
  //gROOT->ProcessLine("Plotter_pt3(\"../../files/reTag_base2_ttbar/\",\"13TeVXAOD_Base2_tt\")");


  gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/a/abell/public/user.vdao.mc15_13TeV.410000.PowhegPythiaEvtGen_nonallhad.merge.AOD.e3698_s2608_s2183_r6630_r6264.BTAGNTUP_V11slim_BTAGSTREAM\",\"/afs/cern.ch/user/a/abell/www/private/taggerperformancemonitoring/mc15r20.1.4.7Orig11/Efficiency/20EMXAOD/WP70/Plotterpt3\",0.70)");
  
  gROOT->ProcessLine(".q");
}
