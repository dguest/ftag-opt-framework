##########################################################################################################################################################
##########################################################################################################################################################
### MAIN SWITCHES

ONLYEssentialInfo =False   ## write minimal amount of info on the output file
ReduceInfo        =False    ## write minimal amount of info on the output file
DoMSV             =True   ## include variables for MSV tagger
doSMT             =False   ## include variables for SMT tagger
doRetag           =True   ## perform retagging
JetCollections = [
  ##"AntiKt10LCTopoJets"
  'AntiKt4EMTopoJets',
  # 'AntiKt4PV0TrackJets',
  # 'AntiKt3PV0TrackJets',
  # 'AntiKt2PV0TrackJets',
  #'AntiKt4LCTopoJets',
  ]

#### automatic AF2 switch
#### reading from outside the ReduceInfo

### 0= SLIM, 1=FULL
ContentMode=  vars().get('CONTENT',-1)
if ContentMode!=-1:
  if ContentMode==0: ReduceInfo=True
  if ContentMode==1: ReduceInfo=False
  if ContentMode==2: ONLYEssentialInfo=True

#########################################################################################################################################################
#########################################################################################################################################################
### Define input xAOD and output ntuple file name
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
#jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock( vars().get('EVTMAX', -1) )
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(10)

jp.AthenaCommonFlags.FilesInput = [
  "/afs/cern.ch/work/m/malanfer/public/training/AOD.root"
  ]

##svcMgr.EventSelector.InputCollections = jp.AthenaCommonFlags.FilesInput()
from PyUtils import AthFile
af = AthFile.fopen( jp.AthenaCommonFlags.FilesInput()[0] )
##svcMgr.EventSelector.InputCollections[0] )

### AF2 or FS
from btagIBLAnalysis.configHelpers import is_af2
isAF2=is_af2(af)

evtPrintoutInterval = vars().get('EVTPRINT', 5000)
svcMgr += CfgMgr.AthenaEventLoopMgr( EventPrintoutInterval=evtPrintoutInterval )

from btagIBLAnalysis.configHelpers import get_short_name
svcMgr += CfgMgr.THistSvc()
for jet in JetCollections:
  shortJetName=get_short_name(jet)
  svcMgr.THistSvc.Output += [ shortJetName+" DATAFILE='flav_"+shortJetName+".root' OPT='RECREATE'"]


##########################################################################################################################################################
##########################################################################################################################################################
### VD: put first all the RecExCommon things, then the B-tagging stuff

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

if "AtlasDerivation" in af.fileinfos["metadata"]["/TagInfo"]["AtlasRelease"]:
    rec.doApplyAODFix.set_Value_and_Lock(False)

include ("RecExCommon/RecExCommon_topOptions.py")

from AthenaCommon.AlgSequence import AlgSequence
algSeq = AlgSequence()

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
###############BTaggingFlags.CalibrationTag = 'BTagCalibRUN12-08-10'

#### if you want to use your own calibration file use this part below
#BTaggingFlags.CalibrationFromLocalReplica = True
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/'
#BTaggingFlags.CalibrationTag = 'BTagCalibRUN2-test'

# some globals have to be defined for this to work
doComputeReference = False
doRecomputePV = False
include("RetagFragment.py")

##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
### Additional Tools needed by the dumper
from btagIBLAnalysis.configHelpers import setupTools
setupTools(ToolSvc, CfgMgr)

##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################

### Main Ntuple Dumper Algorithm
for JetCollection in JetCollections:
  shortJetName=get_short_name(JetCollection)
  alg = CfgMgr.btagIBLAnalysisAlg(
    "BTagDumpAlg_"+JetCollection,
    OutputLevel=INFO,
    Stream=shortJetName,
    InDetTrackSelectionTool   =ToolSvc.InDetTrackSelTool,
    CPTrackingLooseLabel = ToolSvc.CPTrackingLooseLabel,
    TrackVertexAssociationTool=ToolSvc.TightVertexAssocTool,
    TrackToVertexIPEstimator  =ToolSvc.trkIPEstimator,
    JVTtool=ToolSvc.JVT,
                                  ) #DEBUG
  alg.JetCollectionName = JetCollection
  alg.doSMT = doSMT
  if "AntiKt2PV0TrackJets" in JetCollection or "Truth" in JetCollection:
    alg.JetPtCut = 10.e3
    alg.CleanJets     = False
    alg.CalibrateJets = False
  elif "AntiKt3PV0TrackJets" in JetCollection or "AntiKt4PV0TrackJets" in JetCollection:
    alg.JetPtCut = 7.e3
    alg.CleanJets     = False
    alg.CalibrateJets = False
  else:
    alg.JetPtCut = 20.e3
  alg.doSMT     =doSMT
  alg.ReduceInfo=ReduceInfo
  alg.EssentialInfo=ONLYEssentialInfo
  alg.DoMSV     =DoMSV
  alg.Rel20     =True
  alg.JetCleaningTool.CutLevel= "LooseBad"
  alg.JetCleaningTool.DoUgly  = True
  algSeq += alg

  ###print JetCollection
  from btagIBLAnalysis.configHelpers import get_calibration_tool
  ToolSvc += get_calibration_tool(CfgMgr, JetCollection, isAF2)


###########################################################################################################################################################################
