##########################################################################################################################################################
##########################################################################################################################################################
### MAIN SWITCHES

ReduceInfo        =False   ## write minimal amount of info on the output file
DoMSV             =False   ## include variables for MSV tagger
doSMT             =False   ## include variables for SMT tagger
doRetag           =True    ## perform retagging
doComputeReference=False
JetCollections = [
  'AntiKt4EMTopoJets', 
  #'AntiKt3PV0TrackJets',
  #'AntiKt2PV0TrackJets',
  #'AntiKt4LCTopoJets', 
  ]


#########################################################################################################################################################
#########################################################################################################################################################
### Define input xAOD and output ntuple file name
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(-1)

jp.AthenaCommonFlags.FilesInput = [
  "/afs/cern.ch/work/v/vdao//xAODs/data15_13TeV.00276330.physics_Main.merge.DAOD_FTAG1.f620_m1480_p2411_tid06320446_00/DAOD_FTAG1.06320446._000001.pool.root.1",
  ]
svcMgr += CfgMgr.THistSvc()
for jet in JetCollections:
  shortJetName=jet.replace("AntiKt","Akt").replace("TopoJets","To").replace("TrackJets","Tr")
  svcMgr.THistSvc.Output += [ shortJetName+" DATAFILE='flav_"+shortJetName+".root' OPT='RECREATE'"]
#svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple.root' OPT='RECREATE'"]

from PyUtils import AthFile
##print jp.AthenaCommonFlags.FilesInput
af = AthFile.fopen( "/afs/cern.ch/work/v/vdao//xAODs/data15_13TeV.00276330.physics_Main.merge.DAOD_FTAG1.f620_m1480_p2411_tid06320446_00/DAOD_FTAG1.06320446._000001.pool.root.1",) #opens the first file from the InputCollections list
af.fileinfos #this is a dict of dicts, take a look at what's available! Below are some examples:

#print af.fileinfos
derivation=af.fileinfos['EventStreamInfo']
if "StreamDAOD_" in derivation: derivation=derivation.split("DAOD_")[1]
print "DERIVATION IS: "+derivation
#isMC = 'IS_SIMULATION' in af.fileinfos['evt_type']
#beam_energy = af.fileinfos['beam_energy']
#conditions_tag = af.fileinfos['conditions_tag'] #useful for figuring out which mc production this is
#isFullSim = af.fileinfos['metadata']['/Simulation/Parameters']['SimulationFlavour']=='default' #full sim or atlfast
exit(-1)

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

include("RecExCommon/RecExCommon_topOptions.py")

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
#BTaggingFlags.CalibrationTag = 'BTagCalibRUN12-08-07'

#### if you want to use your own calibration file use this part below
#BTaggingFlags.CalibrationFromLocalReplica = True
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/'
#BTaggingFlags.CalibrationTag = 'Run2DC14' ## '0801C' ##'k0002'

include("RetagFragment.py")


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
                                            UsePeriodConfig= "MC15",
                                            LumiCalcFiles=["ilumicalc_histograms_None_276262-276954.root"]
                                            )

##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
### Main Ntuple Dumper Algorithm

triggerLogic="HLT_j[0-9]+|L1_MBTS_1_1|L1_RD0_FILLED"

for JetCollection in JetCollections:
  shortJetName=JetCollection.replace("AntiKt","Akt").replace("TopoJets","To").replace("TrackJets","Tr")
  alg = CfgMgr.btagIBLAnalysisAlg("BTagDumpAlg_"+JetCollection, 
                                  OutputLevel=INFO,
                                  Stream=shortJetName,
                                  InDetTrackSelectionTool   =ToolSvc.InDetTrackSelTool,
                                  TrackVertexAssociationTool=ToolSvc.TightVertexAssocTool,
                                  TrackToVertexIPEstimator  =ToolSvc.trkIPEstimator,
                                  JVTtool=ToolSvc.JVT,
                                  GRLname = "data15_13TeV.periodAllYear_DetStatus-v64-pro19_DQDefects-00-01-02_PHYS_StandardGRL_All_Good.xml",
                                  TriggerLogic=triggerLogic
                                  ) #DEBUG
  alg.JetCollectionName = JetCollection
  alg.doSMT = doSMT
  if "AntiKt2PV0TrackJets" in JetCollection or "Truth" in JetCollection:
    alg.JetPtCut = 10.e3
    alg.CleanJets     = False
    alg.CalibrateJets = False
  else if "AntiKt3PV0TrackJets" in JetCollection or "AntiKt4PV0TrackJets" in JetCollection:
    alg.JetPtCut = 7.e3
    alg.CleanJets     = False
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
                                       IsData=True,
                                       ConfigFile=calibfile,
                                       CalibSequence=calSeg,
                                       JetCollection=collectionForTool) 


###########################################################################################################################################################################
