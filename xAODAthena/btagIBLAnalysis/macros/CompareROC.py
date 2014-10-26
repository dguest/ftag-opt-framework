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
    print " Usage: python PrintROC.py <outFolder>  <fileName> or <folderName>"
    print " "
    sys.exit(1)
#if len(sys.argv) !=3: Helper()

#odir = sys.argv[1]
#if ".root" in odir:
#    print " ..... outputfolder is a file .... please correct"
#    sys.exit(1)

odir="outComp"

########################################################################################
#########################################################################################
gSystem.Exec("mkdir -p "+odir)

#infile1="14TeV/output.root"
##infile2="8TeV/output.root"
infile1="8TeV/output.root"
infile2="test/output.root"

f1=TFile(infile1,"R")
f2=TFile(infile2,"R")


taggerList=[] ### getting it from the first file
histoList= f1.GetListOfKeys()
iterator=TIter(histoList)
obj=iterator()
while obj!=None:
    name=obj.GetName()
    if "---bc" in name:
        pass
    else:
        taggerList.append( name.split("---")[0] )
    obj=iterator()
print taggerList



light=TH1F("b VS light","b VS light",100,0.3,1);
light.SetTitle(";b-jet efficiency;light-jet rejection;")
lightCurve=[]

cj=TH1F("b VS c","b VS c",100,0.3,1);
cj.SetTitle(";b-jet efficiency;c-jet rejection;")
cCurve=[]


myC=TCanvas( "bVSl", "bVSl",900,900);
myC.SetLogy()
myC.SetGridy()
myC.SetGridx()
light.SetMinimum(1)
light.SetMaximum(1e5)
light.Draw()
myLumi= "t#bar{t} simulation, 13 TeV"
#myText(0.20,0.22,1,myLumi,0.05)
legend4=TLegend(0.60,0.60,0.92,0.92)
legend4.SetTextFont(42)
legend4.SetTextSize(0.038)
legend4.SetFillColor(0)
legend4.SetLineColor(0)
legend4.SetFillStyle(0)
legend4.SetBorderSize(0)
count=1
for tag in taggerList:
    f1.cd()
    curve=f1.Get(tag+"---bl")
    f2.cd()
    curve2=f2.Get(tag+"---bl")
    if curve2==None: continue
    count+=1
    curve2.SetLineStyle(2)
    curve.SetLineColor(count)
    curve2.SetLineColor(count)
    curve.SetLineWidth(3)
    curve2.SetLineWidth(3)
    curve.Draw("C")
    curve2.Draw("C")
    legend4.AddEntry(curve ,tag.replace("_","+")+" rel19 13TeV","L")
    legend4.AddEntry(curve2,tag.replace("_","+")+" rel17 8TeV","L")
    
legend4.Draw()
myC.Update()
myC.Print(odir+"/bVSlight.eps")

myC2=TCanvas( "cVSl", "cVSl",900,900);
myC2.SetLogy()
myC2.SetGridy()
myC2.SetGridx()
cj.SetMinimum(1)
cj.SetMaximum(1e3)
cj.Draw()
count=1
for tag in taggerList:
    f1.cd()
    curve=f1.Get(tag+"---bc")
    f2.cd()
    curve2=f2.Get(tag+"---bc")
    print curve
    if curve2==None: continue
    count+=1
    curve2.SetLineStyle(2)
    curve.SetLineColor(count)
    curve2.SetLineColor(count)
    curve.SetLineWidth(3)
    curve2.SetLineWidth(3)
    curve.Draw("C")
    curve2.Draw("C")
 

legend4.Draw()
#myText(0.20,0.22,1,myLumi,0.05)
myC2.Update()
myC2.Print(odir+"/cVSlight.eps")

time.sleep(111111)
