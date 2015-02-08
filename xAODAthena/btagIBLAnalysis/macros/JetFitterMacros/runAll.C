{
  gROOT->ProcessLine(".L Plotter_pt2.C++g");
  gROOT->ProcessLine(".L QuickComparison_type.C++g");
  gROOT->ProcessLine(".L QuickComparison_vtx.C++g");
  gROOT->ProcessLine(".L QuickComparison_2D.C++g");
  
  cout << "##############################################################################################" << endl;
  
  gROOT->ProcessLine("Plotter_pt2(\"/storage/users/vd1007/xAODAthena/files/reTag_MV2fix3_ttbar_RefZ/all.root\",\"JetFitter_MV2fix3_RefZ\")");

  gROOT->ProcessLine("QuickComparison_type(\"/storage/users/vd1007/xAODAthena/files/reTag_MV2fix3_ttbar_RefZ/all.root\",\"JetFitter_MV2fix3_RefZ\")");
 
  gROOT->ProcessLine("QuickComparison_vtx(\"/storage/users/vd1007/xAODAthena/files/reTag_MV2fix3_ttbar_RefZ/all.root\",\"JetFitter_MV2fix3_RefZ\")");
  
  gROOT->ProcessLine("QuickComparison_2D(\"/storage/users/vd1007/xAODAthena/files/reTag_MV2fix3_ttbar_RefZ/all.root\",\"JetFitter_MV2fix3_RefZ\")");

  gROOT->ProcessLine(".q");
}
