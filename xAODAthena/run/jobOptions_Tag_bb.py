##########################################################################################################################################################
##########################################################################################################################################################
### MAIN SWITCHES

ONLYEssentialInfo =False  ## write minimal amount of info on the output file
ReduceInfo        =False  ## write minimal amount of info on the output file
DoMSV             =True   ## include variables for MSV tagger
doSMT             =True   ## include variables for SMT tagger
doComputeReference=False
subJetCollections = ['AntiKt2PV0TrackJets', 'AntiKtVR50Rmax4Rmin0TrackJets']
fatJetCollections = ['AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets']
# JetCollections = ['AntiKt2PV0TrackJets', 'AntiKtVR50Rmax4Rmin0TrackJets', 'AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets']
#JetCollections = ['AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets']


#########################################################################################################################################################
#########################################################################################################################################################
### Define input xAOD and output ntuple file name
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(10)

# nikola's input file with VR jets
jp.AthenaCommonFlags.FilesInput = [
  "/afs/cern.ch/work/d/dguest/data/group.phys-exotics.301488.RS_G_hh_bbbb_c10_M300.e3820_s2608_s2183_r7772_r7676.FTAG5.T3_EXT0/group.phys-exotics.9012214.EXT0._000003.DAOD_FTAG5.pool.root" ]

# name shortener function
_short_jet_names = [
  ("AntiKt","Akt"), ("TrackJets","Tr"), ("TopoJets","To"),
  ("TrimmedPtFrac5SmallR20", "Trm")]
def get_short_name(jet_collection_name):
  for pair in _short_jet_names:
    jet_collection_name.replace(*pair)
  return jet_collection_name

svcMgr += CfgMgr.THistSvc()
for jet in fatJetCollections:
  shortJetName = get_short_name(jet)
  svcMgr.THistSvc.Output += [ shortJetName+" DATAFILE='flav_"+shortJetName+".root' OPT='RECREATE'"]
#svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple.root' OPT='RECREATE'"]



##########################################################################################################################################################
##########################################################################################################################################################
### you should normally not need to touch this part

doRecomputePV=False  ## do not touch unless you know what you are doing
if doComputeReference:
  ReduceInfo   =True
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
### VD: this is if you want to re-tag with another calibration file
from BTagging.BTaggingFlags import BTaggingFlags

#### if the new file is already in the datatbase: simple edit the name
BTaggingFlags.CalibrationTag = 'BTagCalibRUN12-08-18'

#### if you want to use your own calibration file use this part below
#BTaggingFlags.CalibrationFromLocalReplica = True
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/'
#BTaggingFlags.CalibrationTag = 'Run2DC14' ## '0801C' ##'k0002'

defaultTaggers = ['IP2D', 'IP3D', 'SV0', 'MultiSVbb1', 'MultiSVbb2', 'SV1', 'BasicJetFitter', 'JetFitterTag', 'JetFitterNN', 'GbbNNTag', 'MV2c00', 'MV2c10', 'MV2c20', 'MV2c100', 'MV2m']

### setup calibration aliases ###
aliased_collecions = [
  'AntiKt10LCTopoTrimmedPtFrac5SmallR20',
  ]
aliases = ['AntiKt10LCTopo', 'AntiKt6LCTopo', 'AntiKt6TopoEM',
           'AntiKt4LCTopo', 'AntiKt4TopoEM', 'AntiKt4EMTopo']
for aliased in aliased_collecions:
  BTaggingFlags.CalibrationChannelAliases.append(
    "{}->{}".format(aliased, ','.join(aliases)))

# For debugging
###BTaggingFlags.OutputLevel = 1

from DerivationFrameworkFlavourTag.FlavourTagCommon import FlavorTagInit
FlavorTagInit(myTaggers      = defaultTaggers,
              JetCollections = subJetCollections + fatJetCollections,
              Sequencer      = algSeq)

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
for JetCollection in fatJetCollections:
  shortJetName=get_short_name(JetCollection)
  alg = CfgMgr.btagIBLAnalysisAlg(
    "BTagDumpAlg_"+JetCollection,
    OutputLevel=INFO,
    Stream=shortJetName,
    InDetTrackSelectionTool   =ToolSvc.InDetTrackSelTool,
    TrackVertexAssociationTool=ToolSvc.TightVertexAssocTool,
    TrackToVertexIPEstimator  =ToolSvc.trkIPEstimator,
    JVTtool=ToolSvc.JVT,
    #DumpCaloInfo=True,
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
  alg.EssentialInfo=ONLYEssentialInfo
  alg.DoMSV     =DoMSV
  alg.Rel20     =True
  alg.SubjetInfo = True
  #alg.CleanParentJets = True
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
