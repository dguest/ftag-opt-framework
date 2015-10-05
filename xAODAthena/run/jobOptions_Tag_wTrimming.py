##########################################################################################################################################################
##########################################################################################################################################################
### MAIN SWITCHES

ReduceInfo        =True   ## write minimal amount of info on the output file
DoMSV             =True   ## include variables for MSV tagger
doSMT             =False   ## include variables for SMT tagger
doRetag           =True    ## perform retagging
doComputeReference=False

#########################################################################################################################################################
#########################################################################################################################################################
### Define input xAOD and output ntuple file name
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(10)

jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/user/n/nwhallon/work/public/xAOD_samples/mc15_13TeV.301523.MadGraphPythia8EvtGen_A14NNPDF23LO_RS_G_hh_bbbb_c20_M2000.merge.AOD.e3820_s2608_s2183_r6630_r6264_tid05471453_00/AOD.05471453._000002.pool.root.1" ]

svcMgr += CfgMgr.THistSvc()



##########################################################################################################################################################
##########################################################################################################################################################
### you should normally not need to touch this part

doRecomputePV=False  ## do not touch unless you know what you are doing
if doSMT: doRetag=True
if doComputeReference:
  ReduceInfo   =True
  doRetag      =True
  doRecomputePV=False

##########################################################################################################################################################
##########################################################################################################################################################
### VD: put first all the RecExCommon things, then the B-tagging stuff
if doRecomputePV:
  from InDetRecExample.InDetJobProperties import InDetFlags
  InDetFlags.doVertexFinding.set_Value_and_Lock(True)
  from InDetRecExample.InDetKeys import InDetKeys
  InDetKeys.xAODVertexContainer.StoredValue='PrimaryVerticesValerio'

## from Anthony: needed to compute truth quantities of tracks
from AthenaCommon.DetFlags import DetFlags
DetFlags.BField_setOn()
DetFlags.ID_setOn()
DetFlags.Calo_setOff()
DetFlags.Muon_setOff()
if doSMT: DetFlags.Muon_setOn()

from RecExConfig.RecFlags import rec
rec.doTrigger.set_Value_and_Lock(True)
from RecExConfig.RecAlgsFlags  import recAlgs
recAlgs.doTrigger.set_Value_and_Lock(True)
from TriggerJobOpts.TriggerFlags import TriggerFlags
TriggerFlags.doTriggerConfigOnly.set_Value_and_Lock(True)

from RecExConfig.RecFlags import rec
rec.doESD.set_Value_and_Lock        (False)
rec.doWriteESD.set_Value_and_Lock   (False)
rec.doAOD.set_Value_and_Lock        (False)
rec.doWriteAOD.set_Value_and_Lock   (False)
rec.doWriteTAG.set_Value_and_Lock   (False)
rec.doDPD.set_Value_and_Lock        (False)
rec.doTruth.set_Value_and_Lock      (False)

#############rec.doApplyAODFix.set_Value_and_Lock(False)

include ("RecExCommon/RecExCommon_topOptions.py")

from AthenaCommon.AlgSequence import AlgSequence
algSeq = AlgSequence()

# attempt to add jet trimming
# Set up JetRec
from JetRec.JetRecFlags import jetFlags
from JetRec.JetRecConf import JetAlgorithm
jetFlags.useTruth = True

# Set up the tools for jet modifiers
from JetRec.JetRecStandard import jtm
jtm.modifiersMap[ "groomed" ] += [ jtm.ktsplitter, jtm.nsubjettiness, jtm.dipolarity, jtm.planarflow, jtm.angularity, jtm.width, jtm.comshapes, jtm.ktmassdrop, jtm.pull, jtm.encorr, jtm.charge ]
jtm.modifiersMap[ "groomed" ] += [ jtm.jetens ]

#Remake the jet container
akt10untrimmed = jtm.addJetFinder( "MyAntiKt10LCTopoJets", "AntiKt", 1.0, "lctopo", modifiersin="groomed", ghostArea=0.0, ptmin=2000 )

#Define the trimmed container
akt10trim = jtm.addJetTrimmer( "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets", rclus=0.2, ptfrac=0.05, input="MyAntiKt10LCTopoJets", modifiersin="groomed" )

# add jtm to jet algorithms
from JetRec.JetAlgorithm import addJetRecoToAlgSequence
addJetRecoToAlgSequence(algSeq)

# config jet algorithm #
from JetRec.JetRecConf import JetAlgorithm
jetalg = algSeq.jetalg
jetalg.OutputLevel = VERBOSE

JetCollections = [
  "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets"
  #"AntiKt10LCTopoJets"
  #'AntiKt4EMTopoJets', 
  #'AntiKt3PV0TrackJets',
  #'AntiKt2PV0TrackJets',
  #'AntiKt4LCTopoJets', 
  ]

for jet in JetCollections:
  shortJetName=jet.replace("AntiKt","Akt").replace("TopoJets","To").replace("TrackJets","Tr")
  svcMgr.THistSvc.Output += [ shortJetName+" DATAFILE='flav_"+shortJetName+".root' OPT='RECREATE'"]
#svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple.root' OPT='RECREATE'"]

##########################################################################################################################################################
##########################################################################################################################################################
### GEO Business 
from AthenaCommon.GlobalFlags import globalflags
print "detDescr from global flags= "+str(globalflags.DetDescrVersion)
from AtlasGeoModel.InDetGMJobProperties import GeometryFlags as geoFlags
print "geoFlags.Run()   = "+geoFlags.Run()
print "geoFlags.isIBL() = "+str(  geoFlags.isIBL() )


##########################################################################################################################################################
##########################################################################################################################################################
### VD: this is if you want to re-tag with another calibration file
from BTagging.BTaggingFlags import BTaggingFlags

#### if the new file is already in the datatbase: simple edit the name
#BTaggingFlags.CalibrationTag = 'BTagCalibRUN12-08-07'

#### if you want to use your own calibration file use this part below
#BTaggingFlags.CalibrationFromLocalReplica = True
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/'
#BTaggingFlags.CalibrationTag = 'Run2DC14' ## '0801C' ##'k0002'

include("RetagFragment.py")

if doRetag:
  from BTagging.BTaggingConfiguration import getConfiguration
  BTagConf = getConfiguration()
  ip3d =BTagConf.getTool("IP3DTag", "BTagTrackToJetAssociator","AntiKt4EMTopo")
  #ip3d.OutputLevel=DEBUG
  #####ip3d.SortingMode="SortZ0D0"
  
  ip2d =BTagConf.getTool("IP2DTag", "BTagTrackToJetAssociator","AntiKt4EMTopo")
  #####ip2d.SortingMode="SortPt"

##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
### Additional Tools needed by the dumper
from TrackVertexAssociationTool.TrackVertexAssociationToolConf import CP__TightTrackVertexAssociationTool
ToolSvc+=CP__TightTrackVertexAssociationTool("TightVertexAssocTool",dzSinTheta_cut=3, doPV=True)

from InDetTrackSelectionTool.InDetTrackSelectionToolConf import InDet__InDetTrackSelectionTool
ToolSvc+=InDet__InDetTrackSelectionTool("InDetTrackSelTool", CutLevel="Loose", maxZ0SinTheta=3)

from TrkVertexFitterUtils.TrkVertexFitterUtilsConf import Trk__TrackToVertexIPEstimator
ToolSvc+=Trk__TrackToVertexIPEstimator("trkIPEstimator")

from TrigDecisionTool.TrigDecisionToolConf import Trig__TrigDecisionTool
ToolSvc += Trig__TrigDecisionTool( "TrigDecisionTool" )
from TrigEDMConfig.TriggerEDM import EDMLibraries
ToolSvc.TrigDecisionTool.Navigation.Dlls = EDMLibraries

jvt = CfgMgr.JetVertexTaggerTool('JVT')
ToolSvc += jvt

ToolSvc += CfgMgr.CP__PileupReweightingTool("prw",
                                            OutputLevel = INFO,
                                            UsePeriodConfig= "MC15"
                                            )

##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
### Main Ntuple Dumper Algorithm
for JetCollection in JetCollections:
  shortJetName=JetCollection.replace("AntiKt","Akt").replace("TopoJets","To").replace("TrackJets","Tr")
  alg = CfgMgr.btagIBLAnalysisAlg("BTagDumpAlg_"+JetCollection, 
                                  OutputLevel=INFO,
                                  Stream=shortJetName,
                                  InDetTrackSelectionTool   =ToolSvc.InDetTrackSelTool,
                                  TrackVertexAssociationTool=ToolSvc.TightVertexAssocTool,
                                  TrackToVertexIPEstimator  =ToolSvc.trkIPEstimator,
                                  JVTtool=ToolSvc.JVT,
                                  ) #DEBUG
  alg.JetCollectionName = JetCollection
  alg.doSMT = doSMT
  if "Track" in JetCollection or "Truth" in JetCollection:
    alg.JetPtCut = 5.e3
    alg.CleanJets = False
    alg.CalibrateJets = False
  else:
    alg.JetPtCut = 20.e3
  alg.doSMT     =doSMT
  alg.ReduceInfo=ReduceInfo
  alg.DoMSV     =DoMSV
  alg.Rel20     =True
  alg.JetCleaningTool.CutLevel= "LooseBad" 
  alg.JetCleaningTool.DoUgly  = True
  if not doComputeReference: algSeq += alg
  
  ###print JetCollection
  calibfile        = "JES_Prerecommendation2015_Feb2015.config"
  collectionForTool="AntiKt4LCTopo"
  calSeg           ="JetArea_Residual_EtaJES"
  if "EM" in JetCollection: 
    collectionForTool="AntiKt4EMTopo"
    calibfile  ="JES_MC15Prerecommendation_April2015.config"
    #########################calibfile  ="JES_Prerecommendation2015_AFII_Apr2015.config"
    calSeg     ="JetArea_Residual_Origin_EtaJES_GSC"
  print collectionForTool
  ToolSvc += CfgMgr.JetCalibrationTool("BTagDumpAlg_"+JetCollection+"_JCalib",
                                       IsData=False,
                                       ConfigFile=calibfile,
                                       CalibSequence=calSeg,
                                       JetCollection=collectionForTool) 


###########################################################################################################################################################################
