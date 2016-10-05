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
  //gROOT->ProcessLine("Plotter_pt2(\"/afs/cern.ch/work/a/abell/public/user.vdao.mc15_13TeV.410000.PowhegPythiaEvtGen_nonallhad.merge.AOD.e3698_s2608_s2183_r6630_r6264.BTAGNTUP_V11slim_BTAGSTREAM\",\"13TeVXAODEM_Andy_PUB20_2\",\"70\")");

  //gROOT->ProcessLine("Plotter_pt2(\"/afs/cern.ch/work/v/vdao/xAODs/IBLonoff/ON/\",\"13TeVXAODEM_ttbar_IBLON\",\"70\")");
  gROOT->ProcessLine("Plotter_pt2(\"/afs/cern.ch/work/v/vdao/xAODs/IBLonoff/OFF/\",\"13TeVXAODEM_ttbar_IBLOFF\",\"70\")");

  gROOT->ProcessLine(".q");
}
