##########################################################################################################################################################
##########################################################################################################################################################
### MAIN SWITCHES

ONLYEssentialInfo =False  ## write minimal amount of info on the output file
DoMSV             =True   ## include variables for MSV tagger
doSMT             =True   ## include variables for SMT tagger

fatJetCollections = ['AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets']

#########################################################################################################################################################
#########################################################################################################################################################
### Define input xAOD and output ntuple file name
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(10)

# nikola's input file with VR jets
jp.AthenaCommonFlags.FilesInput = [
  '/afs/cern.ch/user/d/dguest/afswork/data/group.perf-flavtag.410000.ttbar_hdamp172p5_nonallhad.DAOD_FTAG5.e3698_s2608_s2183_r7725_r7676.v10_EXT0/group.perf-flavtag.9771960.EXT0._005939.DAOD_FTAG5.pool.root'
]
# name shortener function
from btagIBLAnalysis.configHelpers import get_short_name
svcMgr += CfgMgr.THistSvc()
for jet in fatJetCollections:
  shortJetName = get_short_name(jet)
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

#############rec.doApplyAODFix.set_Value_and_Lock(False)

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

print "Fat Jet Collection:", fatJetCollections


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
associated_subjets = {
  'jet_vrtrkjet_': 'GhostVR30Rmax4Rmin02PV0TrackJet',
}
for JetCollection in fatJetCollections:
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
    DumpCaloInfo=True,
    DumpTrackCovariance=True,
    DumpGATracks=True
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
  alg.ReduceInfo = True
  alg.EssentialInfo=ONLYEssentialInfo
  alg.DoMSV     =DoMSV
  alg.Rel20     =True
  alg.subjetCollections = associated_subjets
  # needed for some trimmed collections
  alg.CleanParentJet = True
  alg.JetCleaningTool.CutLevel= "LooseBad"
  alg.JetCleaningTool.DoUgly  = True

  ###print JetCollection
  calibfile        = "JES_Prerecommendation2015_Feb2015.config"
  collectionForTool="AntiKt4LCTopo"
  calSeg           ="JetArea_Residual_EtaJES"
  if "EM" in JetCollection:
    collectionForTool="AntiKt4EMTopo"
    calibfile  ="JES_MC15Prerecommendation_April2015.config"
    #########################calibfile  ="JES_Prerecommendation2015_AFII_Apr2015.config"
    calSeg     ="JetArea_Residual_Origin_EtaJES_GSC"
  elif "AntiKt10LCTopoTrimmedPtFrac5SmallR20" in JetCollection:
    collectionForTool = "AntiKt10LCTopoTrimmedPtFrac5SmallR20"
    calibfile = "JES_MC15recommendation_FatJet_June2015.config"
    calSeg = "EtaJES_JMS"

  print collectionForTool
  ToolSvc += CfgMgr.JetCalibrationTool(
    "BTagDumpAlg_"+JetCollection+"_JCalib",
    IsData=False,
    ConfigFile=calibfile,
    CalibSequence=calSeg,
    JetCollection=collectionForTool)


###########################################################################################################################################################################
