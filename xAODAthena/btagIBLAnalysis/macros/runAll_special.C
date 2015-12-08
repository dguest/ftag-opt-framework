{
  //gROOT->ProcessLine(".L Plotter_pt2.C++g");
  //gROOT->ProcessLine(".L Plotter_pt3_cjets.C++g");
  gROOT->ProcessLine(".L Plotter_pt3.C++g");

  cout << "##############################################################################################" << endl;

  // gROOT->ProcessLine("Plotter_pt3(\"../../files/DC14_ttbar/\",\"13TeVXAOD_DC14_tt\")");
  //gROOT->ProcessLine("Plotter_pt3(\"../../files/reTag_base2_ttbar/\",\"13TeVXAOD_Base2_tt\")");
  
  
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/a/abell/public/user.vdao.mc15_13TeV.410000.PowhegPythiaEvtGen_nonallhad.merge.AOD.e3698_s2608_s2183_r6630_r6264.BTAGNTUP_V11slim_BTAGSTREAM\",\"13TeVXAODEM_Andy_PUB20_2\",0.70)");

  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao//xAODs/Rel207_prep\",\"13TeVXAODEM_IPxDtest_PUB\",0.70)");

  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/Rel207_prep/OffMVb/\",\"13TeVXAODEM_MVB_PUB\",0.70)");
  
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/user/a/amiucci/work/public/Zprimett_new_light/\",\"13TeVXAODEM_Z27tt_ORIG\",0.70)");
  
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/Rel207_prep/newIPxD_Z27tt//\",\"13TeVXAODEM_Z27tt_NEW\",0.70)");
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/Rel207_prep/newIPxD_Z27tt_Sign//\",\"13TeVXAODEM_Z27tt_NEWsign\",0.70)");
  
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/Rel207_prep/newIPxD_Z27tt_anto/\",\"13TeVXAODEM_Z27tt_anto\",0.70)");

  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/Rel207_prep/newIPxD_Z27tt_TrkSelNewRef/\",\"13TeVXAODEM_Z27tt_TrkSelRef\",0.70)");

  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/IBLonoff/OFF/\",\"13TeVXAODEM_ttbar_IBLOFF\",0.70)");
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/IBLonoff/ON/\",\"13TeVXAODEM_ttbar_IBLON\",0.70)");

  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao//xAODs/firstRel2072/\",\"13TeVXAODEM_ttbar_2072OFF\",0.70)");
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao//xAODs/mc15aRef/\",\"13TeVXAODEM_ttbar_mc15a\",0.70)");
  
  //gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/Valid/r7377/\",\"13TeVXAODEM_ttbar_r7377_OR\",0.70)");
  gROOT->ProcessLine("Plotter_pt3(\"/afs/cern.ch/work/v/vdao/xAODs/Valid/r7377/\",\"13TeVXAODEM_ttbar_r7377\",0.70)");

  gROOT->ProcessLine(".q");
}
