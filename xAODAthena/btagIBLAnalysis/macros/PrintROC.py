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

if len(sys.argv) !=3: Helper()

    
########################################################################################
#class MyParser(argparse.ArgumentParser):
#    def error(self, message):
#        sys.stderr.write('error: %s\n' % message)
#        self.print_help()
#        sys.exit(2)
#parser = argparse.ArgumentParser(description='Debugging info in yields and shapes.')
#parser.add_argument("-i","--idir",
#                    help="Directory containing input files",
#                    dest="idir",
#                    default="")
#parser.add_argument("-o","--odir",
#                    help="Directory containing all the outputs",
#                    dest="odir",
#                    default="testOoutput")
#parser.add_argument("-f","--file",
#                    help="file to run on",
#                    dest="file",
#                    default="a.root")

#options= parser.parse_args()
#print parser.parse_args()

odir = sys.argv[1]
if ".root" in odir:
    print " ..... outputfolder is a file .... please correct"
    sys.exit(1)

second=sys.argv[2]
fileList=[]
if ".root" in second:
    test=TFile(second)
    if test==None:
        print " ..... input file: "+test+" ... NOT FOUND"
        sys.exit(1)
    fileList.append(second)
else:
    fileList=glob(second+"/*.root")
    
if len(fileList)==0:
    print " ..... no files selected .... please check"
    sys.exit(1)

gROOT.SetBatch(True) 
########################################################################################

taggers=[]

is8TeV=False
if "8TeV" in odir:
    is8TeV=True

if not is8TeV:
    taggers=[ ["MV1"     , "mv1"     ,   0.0  ,  0.9945  , 20000, 1 ],   #20000
              ["MV1c"    , "mv1c"    ,   0  ,  1  ,  2000, 3 ],
              ["MV2c00"  , "mv2c00"  ,  -0.5,  0.5,  2000, 2 ],
              ["MV2c10"  , "mv2c10"  ,  -0.5,  0.5,  2000, 4 ],
              ["MV2c20"  , "mv2c20"  ,  -0.5,  0.5,  2000, 7 ],
              ["IP3D"    , "ip3d_llr", -12. ,   30,  2000, 8 ],
              ["SV1"     , "sv1_llr" , -4. ,   13,  2000, 6 ],
              ["IP3D+SV1", "sv1ip3d" , -10. ,   35,  2000, 797 ],
              ["MVb"   , "mvb"   ,  -1.05,  0.8,  2000, 920 ],
              ]
else:
    taggers=[ ["MV1"     , "mv1"     ,   0.0  ,  0.9945  , 20000, 1 ],   #20000
              ["IP3D"    , "ip3d"    , -12.   ,   30,  2000, 8 ],
              ["SV1"     , "sv1"     ,  -4.   ,   13,  2000, 6 ],
              ["MVb"   , "mvb"       ,  -1.05 ,  0.8,  2000, 920 ],
              ]
effThreshold=0.7


def GetHisto(tag, intree, val):
    tmpH=TH1F(tag[1]+str(val),tag[1]+str(tag[5]),tag[4],tag[2],tag[3])
    tmpH.Sumw2()
    var="jet_"+tag[1]+">>"+tmpH.GetName()
    cut=""
    if not is8TeV: cut="jet_truthflav=="+str(val)+" && jet_pt>25e3"
    else:          cut="jet_trueFlav=="+str(val)+" && jet_pt>25e3 && (abs(jet_jvf>0.5) || jet_pt>50e3) "
    intree.Draw( var, cut,"goof",100000)
    tmpH.SetBinContent(1,tmpH.GetBinContent(1)+tmpH.GetBinContent(0))
    tmpH.SetBinError(1,sqrt(pow(tmpH.GetBinError(1),2)+pow(tmpH.GetBinError(0),2)))
    tmpH.SetBinContent(0,0.0)
    tmpH.SetBinError(0,0.0)
    tmpH.SetBinContent(tmpH.GetNbinsX(),tmpH.GetBinContent(tmpH.GetNbinsX())+tmpH.GetBinContent(tmpH.GetNbinsX()+1))
    tmpH.SetBinError(tmpH.GetNbinsX(),sqrt(pow(tmpH.GetBinError(tmpH.GetNbinsX()),2)+pow(tmpH.GetBinError(tmpH.GetNbinsX()+1),2)))
    tmpH.SetBinContent(tmpH.GetNbinsX()+1,0.0)
    tmpH.SetBinError(tmpH.GetNbinsX()+1,0.0)
    if val!=4:
        c = TCanvas("_Discr","_Discr",800,600)    
        tmpH.GetXaxis().SetTitle(tag[0])
        tmpH.Draw("E")
        c.Update()
        c.Print(odir+"/test.eps")
        time.sleep(0.5)
    tmpH.Scale(1./tmpH.Integral())
    return tmpH

    
def GetROC(tag,intree, bVSlight):
    hsig=GetHisto(tag, intree,5)
    hbkgd=None
    if not bVSlight: hbkgd=GetHisto(tag, intree,4)
    else           : hbkgd=GetHisto(tag, intree,0)

    if bVSlight:
        found=False
        for bin in xrange(1,hsig.GetNbinsX()+1):
            partInt=hsig.Integral(bin,hsig.GetNbinsX()+1)
            #print str(bin)+"    "+str(partInt)
            if partInt<effThreshold and not found:
                print " CUT= "+str(hsig.GetBinCenter(bin))+" has eff: "+str(partInt)
                found=True
                
    myROC=TGraph();##hsig.GetNbinsX()-2 );
    maxRej=1
    count=-1
    for bin in xrange(2,hsig.GetNbinsX()):
        sigEff =hsig.Integral(bin,hsig.GetNbinsX()+1)
        bkgdEff=hbkgd.Integral(bin,hsig.GetNbinsX()+1)
        ##if bVSlight: print str(bkgdEff)+"   "+str(sigEff)+" CUT: "+str(hsig.GetBinCenter(bin))
        if bkgdEff!=0 and sigEff!=0 and sigEff<0.99:
            ##if bVSlight: print str(bkgdEff)+"   "+str(sigEff)
            count+=1
            myROC.SetPoint(count,sigEff,1/bkgdEff);
            if 1/bkgdEff>maxRej: maxRej=1/bkgdEff
    myROC.SetLineWidth(3)
    return myROC,maxRej
   
#########################################################################################
gSystem.Exec("mkdir -p "+odir)

intree=TChain("bTag")
for file in fileList:
    intree.Add(file)


countT=-1
light=TH1F("b VS light","b VS light",100,0.3,1);
light.SetTitle(";b-jet efficiency;light-jet rejection;")
lightCurve=[]

cj=TH1F("b VS c","b VS c",100,0.3,1);
cj.SetTitle(";b-jet efficiency;c-jet rejection;")
cCurve=[]

for tag in taggers:
    countT+=1
    print "<<<<<<<  "+tag[0]+"   >>>>>>>>>>>>>>"
    curve,Rej=GetROC(tag,intree,True)
    curve.SetLineColor(tag[5])
    lightCurve.append(curve)
    if Rej*2>light.GetMaximum(): light.SetMaximum(Rej*2)

    curve2,Rej=GetROC(tag,intree,False)
    curve2.SetLineColor(tag[5])
    cCurve.append(curve2)
    if Rej*2>cj.GetMaximum(): cj.SetMaximum(Rej*2)

ofile=TFile(odir+"/output.root","RECREATE")        

myC=TCanvas( "bVSl", "bVSl",900,900);
myC.SetLogy()
myC.SetGridy()
myC.SetGridx()
light.SetMinimum(1)
light.SetMaximum(1e5)
light.Draw()
myLumi= "t#bar{t} simulation, 13 TeV"
myText(0.20,0.22,1,myLumi,0.05)
legend4=TLegend(0.70,0.60,0.92,0.92)
legend4.SetTextFont(42)
legend4.SetTextSize(0.04)
legend4.SetFillColor(0)
legend4.SetLineColor(0)
legend4.SetFillStyle(0)
legend4.SetBorderSize(0)
count=-1
for curve in lightCurve:
    curve.Draw("C")
    count+=1
    legend4.AddEntry(curve,taggers[count][0],"L")
    ofile.WriteObject(curve,taggers[count][0].replace("+","_")+"---bl")
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
for curve in cCurve:
    curve.Draw("C")
    ofile.WriteObject(curve,taggers[count][0].replace("+","_")+"---bc")
legend4.Draw()
myText(0.20,0.22,1,myLumi,0.05)
myC2.Update()
myC2.Print(odir+"/cVSlight.eps")

ofile.Close()

##time.sleep(111111)