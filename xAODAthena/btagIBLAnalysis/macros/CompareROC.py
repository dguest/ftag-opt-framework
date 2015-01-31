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
    print " Usage: python PrintROC.py file1 file2 leg1 leg2 outfolder "
    print " "
    sys.exit(1)
if len(sys.argv) !=6: Helper()

#odir = sys.argv[1]
#if ".root" in odir:
#    print " ..... outputfolder is a file .... please correct"
#    sys.exit(1)

odir= sys.argv[5]

########################################################################################
#########################################################################################
gSystem.Exec("mkdir -p "+odir)

def findStart(bin,target,curve):
    start=bin-10
    Tpx2=Double(0)
    Tpy2=Double(0)
    curve2.GetPoint(start,Tpx2,Tpy2)
    while Tpx2<target and Tpx2!=0 and start>-10:
        #print "      mah: "+str(start)+"  with: "+str(Tpx2)
        start-=10
        curve2.GetPoint(start,Tpx2,Tpy2)
    if start<0: start=0
    #print " ---> start is: "+str(start)+"  with: "+str(Tpx2)
    return start

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


taggerList=[] ### getting it from the first file
histoList= f1.GetListOfKeys()
iterator=TIter(histoList)
obj=iterator()
while obj!=None:
    name=obj.GetName()
    if "---bc" in name:
        pass
    else:
        if "MVb" not in name and "MV1" not in name: 
            taggerList.append( name.split("---")[0] )
    obj=iterator()
print taggerList
##taggerList=["SV1"]

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
myLumi= "t#bar{t} simulation"
myLumi2= "jet p_{T}>25 GeV, |#eta|<2.5"
myText(0.20,0.24,1,myLumi,0.045)
myText(0.20,0.19,1,myLumi2,0.045)
legend4=TLegend(0.48,0.53,0.92,0.95)
legend4.SetTextFont(42)
legend4.SetTextSize(0.030)
legend4.SetFillColor(0)
legend4.SetLineColor(0)
legend4.SetFillStyle(0)
legend4.SetBorderSize(0)
count=1
for tag in taggerList:
    print tag
    myC.cd()
    f1.cd()
    curve=f1.Get(tag+"---bl")
    f2.cd()
    curve2=f2.Get(tag+"---bl")
    if curve2==None: continue
    print curve
    print curve2
    count+=1
    if count==5: count+=1
    if count==9: count=kOrange
    curve2.SetLineStyle(2)
    curve.SetLineColor(count)
    curve2.SetLineColor(count)
    curve.SetLineWidth(3)
    curve2.SetLineWidth(3)
    curve.Draw("C")
    curve2.Draw("C")
    legend4.AddEntry(curve ,tag.replace("_","+")+" "+leg1,"L")
    legend4.AddEntry(curve2,tag.replace("_","+")+" "+leg2,"L")

    myCx=TCanvas( "bVSl"+tag, "bVSl"+tag,800,800);
    pad_1=TPad("pad_1", "up", 0., 0.35, 1., 1.);
    pad_1.SetBottomMargin(0);
    pad_1.Draw();   
    pad_1.SetGridy()
    pad_1.SetGridx()
    pad_1.SetLogy()
    pad_2=TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35);
    pad_2.SetTopMargin(0);
    pad_2.SetBottomMargin(0.28);
    pad_2.Draw();
    pad_1.cd();

    light.Draw()
    myText(0.20,0.24,1,myLumi,0.045)
    myText(0.20,0.19,1,myLumi2,0.045)
    curve.Draw("C")
    curve2.Draw("C")
    legend5=TLegend(0.55,0.77,0.92,0.90)
    legend5.SetTextFont(42)
    legend5.SetTextSize(0.042)
    legend5.SetFillColor(0)
    legend5.SetLineColor(0)
    legend5.SetFillStyle(0)
    legend5.SetBorderSize(0)
    legend5.AddEntry(curve ,tag.replace("_","+")+" "+leg1,"L")
    legend5.AddEntry(curve2,tag.replace("_","+")+" "+leg2,"L")
    legend5.Draw("SAME")

    pad_2.cd()
    ratio=light.Clone("ratio")
    ratio.GetYaxis().SetTitle("rel. diff.")
    ratio.GetYaxis().SetTitleOffset(1.1)
    ratio.GetXaxis().SetLabelSize(0.10)
    ratio.GetXaxis().SetTitleSize(0.10)
    ratio.GetYaxis().SetTitleOffset(0.7)
    ratio.GetYaxis().SetLabelSize(0.09)
    ratio.GetYaxis().SetTitleSize(0.09)
    ratio.SetMaximum(0.8)
    ratio.SetMinimum(-0.8)
    ratio.SetLineColor(kGray)
    ratio.Draw("HIST")

    ratC=curve2.Clone("ratioC")
    for bin in xrange(1,ratC.GetN()+1):
        px1=Double(0)
        py1=Double(0)
        curve.GetPoint(bin-1,px1,py1)
        if bin%500==0: print str(bin)+" ....: "+str(px1)
        if py1==0: continue
        clo_py2=100000
        clo_px2=100000
        endV=0
        start=findStart(bin,px1,curve2)
        for bin2 in xrange(start,ratC.GetN()+1):
            #print "       bin2: "+str(bin2) 
            px2=Double(0)
            py2=Double(0)
            curve2.GetPoint(bin2-1,px2,py2)
            if fabs(px2-px1)>0.05 : continue
            if fabs(px2-px1)<fabs(clo_px2-px1):
                clo_py2=py2
                clo_px2=px2
                endV=bin2
            elif px2<px1:  break
        #print "  ---> end is: "+str(endV)+"  with: "+str(clo_px2)
        if clo_py2!=100000 : ratC.SetPoint(bin-1,px1,clo_py2/py1-1)
        else               : ratC.SetPoint(bin-1,px1,-1)
            
    ratC.SetLineWidth(4)
    ratC.SetLineColor(1)
    ratC.Draw("L")
    
    myCx.Update()
    myCx.Print(odir+"/bVSlight__"+tag+".eps")

myC.cd()
legend4.Draw()
myText(0.20,0.24,1,myLumi,0.045)
myText(0.20,0.19,1,myLumi2,0.045)
myC.Update()
##myC.Print(odir+"/bVSlightALL.eps")


################################################################
myC2=TCanvas( "cVSl", "cVSl",900,900);
myC2.SetLogy()
myC2.SetGridy()
myC2.SetGridx()
cj.SetMinimum(1)
cj.SetMaximum(1e3)
cj.Draw()
myText(0.20,0.24,1,myLumi,0.045)
myText(0.20,0.19,1,myLumi2,0.045)
count=1
for tag in taggerList:
    myC2.cd()
    f1.cd()
    curve=f1.Get(tag+"---bc")
    f2.cd()
    curve2=f2.Get(tag+"---bc")
    print curve
    if curve2==None: continue
    count+=1
    if count==5: count+=1
    if count==9: count=kOrange
    curve2.SetLineStyle(2)
    curve.SetLineColor(count)
    curve2.SetLineColor(count)
    curve.SetLineWidth(3)
    curve2.SetLineWidth(3)
    curve.Draw("C")
    curve2.Draw("C")
    
    myCx=TCanvas( "bVSc"+tag, "bVSc"+tag,800,800);
    pad_1=TPad("pad_1", "up", 0., 0.35, 1., 1.);
    pad_1.SetBottomMargin(0);
    pad_1.Draw();   
    pad_1.SetGridy()
    pad_1.SetGridx()
    pad_1.SetLogy()
    pad_2=TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35);
    pad_2.SetTopMargin(0);
    pad_2.SetBottomMargin(0.28);
    pad_2.Draw();
    pad_1.cd();
    
    cj.Draw()
    myText(0.20,0.24,1,myLumi,0.045)
    myText(0.20,0.19,1,myLumi2,0.045)
    curve.Draw("C")
    curve2.Draw("C")
    legend5=TLegend(0.55,0.77,0.92,0.90)
    legend5.SetTextFont(42)
    legend5.SetTextSize(0.038)
    legend5.SetFillColor(0)
    legend5.SetLineColor(0)
    legend5.SetFillStyle(0)
    legend5.SetBorderSize(0)
    legend5.AddEntry(curve ,tag.replace("_","+")+" "+leg1,"L")
    legend5.AddEntry(curve2,tag.replace("_","+")+" "+leg2,"L")
    legend5.Draw("SAME")

    pad_2.cd()
    ratio=cj.Clone("ratio")
    ratio.GetYaxis().SetTitle("rel. diff.")
    ratio.GetYaxis().SetTitleOffset(1.1)
    ratio.GetXaxis().SetLabelSize(0.10)
    ratio.GetXaxis().SetTitleSize(0.10)
    ratio.GetYaxis().SetTitleOffset(0.7)
    ratio.GetYaxis().SetLabelSize(0.09)
    ratio.GetYaxis().SetTitleSize(0.09)
    ratio.SetMaximum(0.8)
    ratio.SetMinimum(-0.8)
    ratio.SetLineColor(kGray)
    ratio.Draw("HIST")

    ratC=curve2.Clone("ratioC")
    for bin in xrange(1,ratC.GetN()+1):
        px1=Double(0)
        py1=Double(0)
        curve.GetPoint(bin-1,px1,py1)
        if bin%500==0: print str(bin)+" ....: "+str(px1)
        if py1==0: continue
#        print str(bin)+" ..... : "+str(px1)#+" "+str(py1)
        endV=0
        start=findStart(bin,px1,curve2)
        for bin2 in xrange(start,ratC.GetN()+1):
            ##print "       bin2: "+str(bin2) 
            px2=Double(0)
            py2=Double(0)
            curve2.GetPoint(bin2-1,px2,py2)
            if fabs(px2-px1)>0.05 : continue
            if fabs(px2-px1)<fabs(clo_px2-px1):
                clo_py2=py2
                clo_px2=px2
                endV=bin2
            elif px2<px1:  break
        ##print "  ---> end is: "+str(endV)+"  with: "+str(clo_px2)
        if clo_py2!=100000 : ratC.SetPoint(bin-1,px1,clo_py2/py1-1)
        else               : ratC.SetPoint(bin-1,px1,-1)
        
    ratC.SetLineWidth(4)
    ratC.SetLineColor(1)
    ratC.Draw("L")
    myCx.Update()
    
    myCx.Print(odir+"/bVSc__"+tag+".eps")

myC2.cd()
legend4.Draw()
myText(0.20,0.24,1,myLumi,0.045)
myText(0.20,0.19,1,myLumi2,0.045)
myC2.Update()
##myC2.Print(odir+"/bVScALL.eps")

