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
odir= sys.argv[5]

########################################################################################
#########################################################################################
gSystem.Exec("mkdir -p "+odir)

infile1=sys.argv[1]
infile2=sys.argv[2]

leg1=sys.argv[3]
leg2=sys.argv[4]

f1=TFile(infile1,"R")
f2=TFile(infile2,"R")

###################################################################################
### set by the user
jetType= sys.argv[6]
special=False
if jetType=="lFLAT":
    special=True
    jetType="l"

if jetType=="cFLAT":
    special=True
    jetType="c"

if jetType!="b" and jetType!="c" and jetType!="l":
    print "jetType: "+jetType+" not recognized (b,c,l) .... aborting "
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
taggerList=["IP3D","SV1","MV2c00","MV2c20","MV1","JetFitter"]
taggerList=["MV2c20"]

canvases=[]
for tag in taggerList:
    for var in plotList:
        myC=TCanvas( var.replace("Base__","")+"_"+tag , var.replace("Base__","")+"_"+tag, 800,800);
        pad_1=TPad("pad_1", "up", 0., 0.35, 1., 1.);
        pad_1.SetBottomMargin(0);
        pad_1.Draw();   
        pad_1.SetGridy()
        pad_1.SetGridx()
        pad_2=TPad("pad_2", "down", 0.0, 0.00, 1.0, 0.35);
        pad_2.SetTopMargin(0);
        pad_2.SetBottomMargin(0.28);
        pad_2.Draw();
        pad_1.cd();
        
        f1.cd()
        base=f1.Get(var)
        if special:
            base.GetYaxis().SetTitle(jetType+" rej. @70% flat b-eff.")
        else :
            base.GetYaxis().SetTitle(tag+" 70% eff.")
        base.SetMinimum(0.001)
        if jetType=="c": base.SetMaximum(20)
        if jetType=="l": base.SetMaximum(1500)
        if special:  base.SetMaximum(0.20)
        base.Draw("HIST")
        if special: base.GetXaxis().SetRangeUser(25,600);
        legend4=TLegend(0.60,0.75,0.82,0.95)
        legend4.SetTextFont(42)
        legend4.SetTextSize(0.035)
        legend4.SetFillColor(0)
        legend4.SetLineColor(0)
        legend4.SetFillStyle(0)
        legend4.SetBorderSize(0)
        count=2
        myText(0.20,0.75,1,tag,0.06)
        curve1=None
        if special:
            curve1=f1.Get( "Eff_b__Eff__"+var.replace("Base__","")+"__"+tag+jetType+"rej_70")
        else:
            curve1=f1.Get( "Eff_"+jetType+"__Eff__"+var.replace("Base__","")+"__"+tag+"_70")
        curve1.SetLineColor(count);
        curve1.SetMarkerColor(count);
        legend4.AddEntry(curve1 ,leg1, "LPE")
        curve1.Draw("PE")

        count=4
        f2.cd()
        curve2=None
        if special:
            curve2=f2.Get( "Eff_b__Eff__"+var.replace("Base__","")+"__"+tag+jetType+"rej_70")
        else:
            curve2=f2.Get( "Eff_"+jetType+"__Eff__"+var.replace("Base__","")+"__"+tag+"_70")
        curve2.SetLineColor(count);
        curve2.SetMarkerColor(count);
        legend4.AddEntry(curve2 ,leg2, "LPE")
        curve2.Draw("PE")
        
        legend4.Draw()

        pad_2.cd()
        ratio=base.Clone("ratio")
        ratio.GetYaxis().SetTitle("Ratio")
        #print str(ratio.GetXaxis().GetTitleOffset())+" "+str(ratio.GetXaxis().GetTitleSize())+" "+str(ratio.GetXaxis().GetLabelSize())
        #print str(ratio.GetYaxis().GetTitleOffset())+" "+str(ratio.GetYaxis().GetTitleSize())+" "+str(ratio.GetYaxis().GetLabelSize())
        ratio.GetYaxis().SetTitleOffset(1.1)
        ratio.GetXaxis().SetLabelSize(0.10)
        ratio.GetXaxis().SetTitleSize(0.10)
        ratio.GetYaxis().SetTitleOffset(0.7)
        ratio.GetYaxis().SetLabelSize(0.09)
        ratio.GetYaxis().SetTitleSize(0.09)

        ratio.SetMaximum(4.99)
        ratio.SetMinimum(0.01)
        ratio.SetLineColor(kGray)
        ratio.Draw("HIST")

        ratioH=base.Clone("ratio")
        for bin in xrange(1,base.GetNbinsX()+1):
            px1=Double(0)
            py1=Double(0)
            py2=Double(0)
            curve1.GetPoint(bin-1,px1,py1)
            curve2.GetPoint(bin-1,px1,py2)
            if py1!=0:
                ratioH.SetBinContent(bin, py2/py1)
            else:
                ratioH.SetBinContent(bin, -1)
            ey1=curve1.GetErrorYhigh(bin-1)
            ey2=curve2.GetErrorYhigh(bin-1)
            if py1!=0 and py2!=0:
                ratioH.SetBinError(bin, py2/py1*sqrt( pow(ey1/py1,2) + pow(ey2/py2,2) ) )
            
        ratioH.SetLineColor(4)
        ratioH.SetLineWidth(3)
        ratioH.SetMarkerSize(1)
        ratioH.SetMarkerColor(4)
        ratioH.SetMarkerStyle(20)
        ratioH.Draw("SAMEE")
        
        myC.Update()
        canvases.append(myC)
        if special:
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+"_flat.eps")
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+"_flat.pdf")
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+"_flat.C")
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+"_flat.png")
        else:
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+".eps")
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+".pdf")
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+".C")
            myC.Print(odir+"/"+myC.GetName()+"__"+jetType+".png")

