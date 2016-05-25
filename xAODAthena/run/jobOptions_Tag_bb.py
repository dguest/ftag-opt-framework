##########################################################################################################################################################
##########################################################################################################################################################
### MAIN SWITCHES

ONLYEssentialInfo =False  ## write minimal amount of info on the output file
ReduceInfo        =False  ## write minimal amount of info on the output file
DoMSV             =True   ## include variables for MSV tagger
doSMT             =True   ## include variables for SMT tagger
doRetag           =False  ##False    ## perform retagging
doComputeReference=False
JetCollections = [
  #"AntiKt10LCTopoJets"
  #'AntiKt4EMTopoJets', 
  #'AntiKt3PV0TrackJets',
  'AntiKt2PV0TrackJets',
  #'AntiKt4LCTopoJets',
  'AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets'
  ]


#########################################################################################################################################################
#########################################################################################################################################################
### Define input xAOD and output ntuple file name
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(10)

# nikola's input file with VR jets
# (while we wait for a new derivation with these included)
#jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/n/nwhallon/public/VRDerivation/DAOD_VRJM.VRJM.pool.root" ]
jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/user/n/nwhallon/work/public/DxAOD_samples/group.phys-exotics.301523.RS_G_hh_bbbb_c20_M2000.e3820_s2608_s2183_r7772_r7676.VRT5_EXT0/group.phys-exotics.8418217.EXT0._000001.DAOD_FTAG5.pool.root" ]

svcMgr += CfgMgr.THistSvc()
for jet in JetCollections:
  if "AntiKt2PV0T" in jet: continue
  shortJetName=jet.replace("AntiKt","Akt").replace("TopoJets","To").replace("TrackJets","Tr").replace("TrimmedPtFrac5SmallR20", "Trm")
  svcMgr.THistSvc.Output += [ shortJetName+" DATAFILE='flav_"+shortJetName+".root' OPT='RECREATE'"]
#svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple.root' OPT='RECREATE'"]



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
### Qi: Jet Business

# Actually build AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets here
#from DerivationFrameworkJetEtMiss.ExtendedJetCommon import addDefaultTrimmedJets
#addDefaultTrimmedJets(algSeq, "WhoCares")

# make exkt subjet finding tool
def buildExclusiveSubjets(JetCollectionName, nsubjet, ToolSvc = ToolSvc):
    from JetSubStructureMomentTools.JetSubStructureMomentToolsConf import SubjetFinderTool
    from JetSubStructureMomentTools.JetSubStructureMomentToolsConf import SubjetRecorderTool

    SubjetContainerName = "%sExKt%iSubJets" % (JetCollectionName.replace("Jets", ""), nsubjet)

    subjetrecorder = SubjetRecorderTool("subjetrecorder%i_%s" % (nsubjet, JetCollectionName))
    ToolSvc += subjetrecorder

    subjetlabel = "ExKt%iSubJets" % (nsubjet)

    subjetrecorder.SubjetLabel = subjetlabel
    subjetrecorder.SubjetContainerName = SubjetContainerName

    from JetTagTools.JetTagToolsConf import Analysis__ExKtbbTagTool
    ExKtbbTagToolInstance = Analysis__ExKtbbTagTool(
      name = "ExKtbbTagTool%i_%s" % (nsubjet, JetCollectionName),
      JetAlgorithm = "Kt",
      JetRadius = 10.0,
      PtMin = 5000,
      ExclusiveNJets = 2,
      SubjetContainerName = SubjetContainerName,
      # SubjetFinder = subjetfinder,
      SubjetRecorder = subjetrecorder,
      SubjetLabel = subjetlabel,
      SubjetAlgorithm_BTAG = "AntiKt",
      SubjetRadius_BTAG = 0.4
    )
    ToolSvc += ExKtbbTagToolInstance

    return (ExKtbbTagToolInstance, SubjetContainerName)

# build exkt subjets here
JetCollectionExKtSubJetList = []
for JetCollectionExKt in JetCollections:
  if "AntiKt2PV0T" in JetCollectionExKt: continue
  # build ExKtbbTagTool instance
  (ExKtbbTagToolInstance, SubjetContainerName) = buildExclusiveSubjets(JetCollectionExKt, 2)
  JetCollectionExKtSubJetList += [SubjetContainerName]
  
  # approach 2: existing JetRecTool
  from JetRec.JetRecConf import JetRecTool
  jetrec = JetRecTool(
                       name = "JetRecTool_ExKtbb_%s" % (JetCollectionExKt),
                       OutputContainer = JetCollectionExKt,
                       InputContainer = JetCollectionExKt,
                       JetModifiers = [ExKtbbTagToolInstance],
                     )
  ToolSvc += jetrec
  from JetRec.JetRecConf import JetAlgorithm
  algSeq += JetAlgorithm(
                          name = "JetAlgorithm_ExKtbb_%s" % (JetCollectionExKt),
                          Tools = [jetrec],
                        )


print "Fat Jet Collection:",JetCollections
print "Fat Jet ExKt SubJet Collection:",JetCollectionExKtSubJetList


##########################################################################################################################################################
##########################################################################################################################################################
### VD: this is if you want to re-tag with another calibration file
from BTagging.BTaggingFlags import BTaggingFlags

#### if the new file is already in the datatbase: simple edit the name
BTaggingFlags.CalibrationTag = 'BTagCalibRUN12-08-17'

#### if you want to use your own calibration file use this part below
#BTaggingFlags.CalibrationFromLocalReplica = True
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/'
#BTaggingFlags.CalibrationTag = 'Run2DC14' ## '0801C' ##'k0002'

defaultTaggers = ['IP2D', 'IP3D', 'SV0', 'MultiSVbb1', 'MultiSVbb2', 'SV1', 'BasicJetFitter', 'JetFitterTag', 'JetFitterNN', 'GbbNNTag', 'MV2c00', 'MV2c10', 'MV2c20', 'MV2c100', 'MV2m']
specialTaggers = ['ExKtbb_Hbb_MV2Only', 'ExKtbb_Hbb_MV2andJFDRSig', 'ExKtbb_Hbb_MV2andTopos']

### setup calibration aliases ###
aliased_collecions = [
  'AntiKt10LCTopoTrimmedPtFrac5SmallR20',
  'AntiKt10LCTopoTrimmedPtFrac5SmallR20ExKt2Sub',
  ]
aliases = ['AntiKt10LCTopo', 'AntiKt6LCTopo', 'AntiKt6TopoEM',
           'AntiKt4LCTopo', 'AntiKt4TopoEM', 'AntiKt4EMTopo']
for aliased in aliased_collecions:
  BTaggingFlags.CalibrationChannelAliases.append(
    "{}->{}".format(aliased, ','.join(aliases)))

for JetCollectionExKtSubJet in JetCollectionExKtSubJetList:
  BTaggingFlags.CalibrationChannelAliases += [JetCollectionExKtSubJet[:-4]+"->AntiKt4LCTopo"]

# For debugging 
###BTaggingFlags.OutputLevel = 1

from DerivationFrameworkFlavourTag.FlavourTagCommon import FlavorTagInit
FlavorTagInit(myTaggers      = defaultTaggers,
              JetCollections = JetCollectionExKtSubJetList,
              Sequencer      = algSeq)
FlavorTagInit(myTaggers      = defaultTaggers + specialTaggers,
              JetCollections = JetCollections,
              Sequencer      = algSeq)

if doRetag:

  from BTagging.BTaggingConfiguration import getConfiguration
  btag_conf = getConfiguration()

  # Stuff from Dan: dump more information on vertices
  # replace JetFitter
  # TODO: figure out if this is running,
  #       and how to get the resulting vertices
  from BTagging.BTaggingConfiguration import cloneToolCollection, getToolCollections
  jfname = ("JetFitterCollection", "DG")
  cloneToolCollection(*jfname)
  for jc_long in JetCollections:
    jc = jc_long[:-4]
    btag_conf.addTool('JetFitterTagNNDG', ToolSvc, "BTagTrackToJetAssociator",jc)
    jfvx = btag_conf.getTool("NewJetFitterVxFinderDG", "BTagTrackToJetAssociator",jc)
    jfvx.VertexClusteringProbabilityCut = 1.0
    apple = btag_conf.getJetCollectionSecVertexingTool(jc)
    print "DANTAGGING!", jc
    print "DANTAGGING!", apple.SecVtxFinderxAODBaseNameList
    print "DANTAGGING!", apple.SecVtxFinderList
    print jfvx


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
  if "AntiKt2PV0T" in JetCollection: continue

  shortJetName=JetCollection.replace("AntiKt","Akt").replace("TopoJets","To").replace("TrackJets","Tr").replace("TrimmedPtFrac5SmallR20", "Trm")
  alg = CfgMgr.btagIBLAnalysisAlg("BTagDumpAlg_"+JetCollection, 
                                  OutputLevel=INFO,
                                  Stream=shortJetName,
                                  InDetTrackSelectionTool   =ToolSvc.InDetTrackSelTool,
                                  TrackVertexAssociationTool=ToolSvc.TightVertexAssocTool,
                                  TrackToVertexIPEstimator  =ToolSvc.trkIPEstimator,
                                  JVTtool=ToolSvc.JVT,
                                  DumpCaloInfo=True,
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
    calSeg = "EtaJES"

  print collectionForTool
  ToolSvc += CfgMgr.JetCalibrationTool("BTagDumpAlg_"+JetCollection+"_JCalib",
                                       IsData=False,
                                       ConfigFile=calibfile,
                                       CalibSequence=calSeg,
                                       JetCollection=collectionForTool) 


###########################################################################################################################################################################
