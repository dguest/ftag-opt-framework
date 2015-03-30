{
  gROOT->ProcessLine(".L histcompseparate.C++g");

  cout << "##############################################################################################" << endl;

  gROOT->ProcessLine("histcompseparate(\"/afs/cern.ch/work/l/lmcclymo/public/user.vdao_v20MV2trainMoreMV2s\",\"/afs/cern.ch/work/a/abell/public/user.vdao.mc14_13TeV.110401.PowhegPythia_ttbar_nonallhad.merge.AOD.e2928_s1982_s2008_r6205_r6223.BTAGNTUP_Orig2_BTAGSTREAM\",\"$HOME/www/private/taggerperformancemonitoring/r20.1.4.1/Kinematics\")");

 gROOT->ProcessLine(".q");
}
