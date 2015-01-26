#import argparse
from glob import glob
import os, sys
from array import *
from ROOT import *#TCanvas,TLegend,gROOT
import math
import time

ROOT.gROOT.LoadMacro("AtlasStyle.C")
ROOT.gROOT.LoadMacro("AtlasUtils.C")
SetAtlasStyle()


tmpVale=sys.argv
def Helper():
    print " "
    print " Usage: python PrintROC.py file1 file2 leg1 leg2 outfolder jetType"
    print " "
    sys.exit(1)
if len(sys.argv) !=7: Helper()

#odir = sys.argv[1]
#if ".root" in odir:
#    print " ..... outputfolder is a file .... please correct"
#    sys.exit(1)

odir= sys.argv[5]

########################################################################################
#########################################################################################
gSystem.Exec("mkdir -p "+odir)

#infile1="14TeV/output.root"
##infile1="8TeV/output.root"
#infile2="8TeVd3pd/output.root"
#infile1="8TeV/output.root"
#infile2="test/output.root"

infile1=sys.argv[1]
infile2=sys.argv[2]

leg1=sys.argv[3]
leg2=sys.argv[4]

f1=TFile(infile1,"R")
f2=TFile(infile2,"R")

###################################################################################
### set by the user
taggerList=["MV1","MV2c00","IP3D","SV1","JetFitter"]
jetType= sys.argv[6]
if jetType!="b" and jetType!="c" and jetType!="l":
    print "jetType not recognized (b,c,l) .... aborting "
    Helper()
###################################################################################

plotList=[]
histoList= f1.GetListOfKeys()
iterator=TIter(histoList)
obj=iterator()
while obj!=None:
    name=obj.GetName()
    if "Base__" not in name:
        pass
    else:
        plotList.append( name )
    obj=iterator()
plotList.sort()
plotList=list(set(plotList))
print plotList
#print taggerList



canvases=[]
for var in plotList:
    myC=TCanvas( var.replace("Base__","") , var.replace("Base__",""), 800,600);
    myC.SetGridy()
    myC.SetGridx()

    f1.cd()
    base=f1.Get(var)
    base.GetYaxis().SetTitle("70% eff.")
    if jetType=="c": base.SetMaximum(0.35)
    if jetType=="l": base.SetMaximum(0.05)
    base.Draw("HIST")
    legend4=TLegend(0.75,0.65,0.92,0.95)
    legend4.SetTextFont(42)
    legend4.SetTextSize(0.030)
    legend4.SetFillColor(0)
    legend4.SetLineColor(0)
    legend4.SetFillStyle(0)
    legend4.SetBorderSize(0)
    count=1
    myText(0.20,0.24,1,jetType+"-jets",0.045)
    for tag in taggerList:
        count+=1
        if count==5: count=6
        f1.cd()
        curve=f1.Get( "Eff_"+jetType+"__Eff__"+var.replace("Base__","")+"__"+tag+"_70")
        print curve
        #f1.ls()
        curve.SetLineColor(count);
        curve.SetMarkerColor(count);
        legend4.AddEntry(curve ,tag, "L")
        curve.Draw("PE")
    legend4.Draw()
    myC.Update()
    canvases.append(myC)
    myC.Print(odir+"/"+myC.GetName()+"__"+jetType+".eps")
    ##time.sleep(2)

#time.sleep(1000)

#myC.Print(odir+"/bVSlightALL.eps")


