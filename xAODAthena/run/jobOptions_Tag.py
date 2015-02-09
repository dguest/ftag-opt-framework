# ====================================================================
# MAIN SWITCHES
# ====================================================================
ReduceInfo   =False ##write minimal amount of info on the output file
DoMSV        =False ##include variables for MSV tagger
Rel20        =True ##switch between rel19 and rel20
#(only option that will work on original DC14 xAOD)
doRetag      =True  ## perform retagging
doRecomputePV=True  ## need to be true when re-tagging to recover JetFitter performance
doComputeReference=False

# ===================================================================
# ===================================================================
# ===================================================================
# ===================================================================
if doRetag==False:
  ReduceInfo=True

if doRecomputePV:
  doRetag=True

if doComputeReference:
  ReduceInfo=True
  doRetag   =True
  doRecomputePV=True

#if Rel20:
#  ReduceInfo=True

from BTagging.BTaggingFlags import BTaggingFlags
## chainging to other official calib file
##BTaggingFlags.CalibrationTag = 'BTagCalibALL-08-00'

## chainging to private calib file
#BTaggingFlags.CalibrationFromLocalReplica = True
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/'
#BTaggingFlags.CalibrationTag = 'k0803' ##'k0002'

#theApp.EvtMax = -1
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(-1)

## main test file: TTbar xAOD r19 mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad
## jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/v/vdao//public/AOD.01587947._004222.pool.root.1"]
jp.AthenaCommonFlags.FilesInput = [ "AOD.pool.root"]

## Frank's original
#jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/f/filthaut/public/AOD.01606245._000001.pool.root.1"]
## Frank's retag
#jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/f/filthaut/public/AOD_Retag.pool.root"]

#jp.AthenaCommonFlags.FilesInput = [ "../../AOD_Test.pool.root"]


# ====================================================================
# Define output ntuple file name
# ====================================================================
svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple.root' OPT='RECREATE'"]
# ====================================================================

##algSeq = CfgMgr.AthSequencer("AthAlgSeq")

## THIS IS A TEMPORARY solution to avoud Memleaks
from BTagging.BTaggingFlags import BTaggingFlags
BTaggingFlags.MVb=False

if  doComputeReference:
  BTaggingFlags.Runmodus = "reference"
  BTaggingFlags.SV0 = False
  BTaggingFlags.SV2 = False
  BTaggingFlags.GbbNNTag = False
  BTaggingFlags.JetFitterTag = False
  BTaggingFlags.JetFitterNN = False
  BTaggingFlags.MVb = False
  BTaggingFlags.MV1 = False
  BTaggingFlags.MV2 = False
  BTaggingFlags.MV1c = False
  BTaggingFlags.MV2c00 = False
  BTaggingFlags.MV2c10 = False
  BTaggingFlags.MV2c20 = False
  BTaggingFlags.MultiSVbb1 = False
  BTaggingFlags.MultiSVbb2 = False
  pass

if doRecomputePV:
  BTaggingFlags.PrimaryVertexCollectionName='PrimaryVerticesValerio'
  from InDetRecExample.InDetJobProperties import InDetFlags
  #InDetFlags.postProcessing.set_Value_and_Lock(True)
  InDetFlags.doVertexFinding.set_Value_and_Lock(True)
  from InDetRecExample.InDetKeys import InDetKeys
  InDetKeys.xAODVertexContainer.StoredValue='PrimaryVerticesValerio'

## from Anthony: needed to compute truth quantities of tracks
import MagFieldServices.SetupField
# --- disable error protection of RecExCommon
from AthenaCommon.DetFlags import DetFlags
DetFlags.ID_setOn()
DetFlags.Calo_setOff()
DetFlags.Muon_setOff()
from RecExConfig.RecFlags import rec
rec.doTrigger.set_Value_and_Lock(False)
rec.doESD.set_Value_and_Lock        (False)###
rec.doWriteESD.set_Value_and_Lock        (False)###
rec.doAOD.set_Value_and_Lock             (False)
rec.doWriteAOD.set_Value_and_Lock        (False)
rec.doWriteTAG.set_Value_and_Lock        (False)
rec.doDPD.set_Value_and_Lock             (False)

include ("RecExCommon/RecExCommon_topOptions.py")

from AthenaCommon.AlgSequence import AlgSequence
algSeq = AlgSequence()

if ReduceInfo==False:
   from DerivationFrameworkInDet.DerivationFrameworkInDetConf import DerivationFramework__TrackParametersForTruthParticles
   TTPName = "TruthParticle"
   if Rel20:
     TTPName = "TruthParticles"
   TruthDecor = DerivationFramework__TrackParametersForTruthParticles( name = "TruthTPDecor",
                                                                       OutputLevel = DEBUG,
                                                                       DecorationPrefix ="",
                                                                       TruthParticleContainerName=TTPName)
   ToolSvc +=TruthDecor
   augmentationTools = [TruthDecor]
   from DerivationFrameworkCore.DerivationFrameworkCoreConf import DerivationFramework__CommonAugmentation
   algSeq += CfgMgr.DerivationFramework__CommonAugmentation("MyDFTSOS_KERN",
                                                            AugmentationTools = augmentationTools,
                                                            OutputLevel = DEBUG )

# Demonstrate to run on 2 jet collections at the same time
#JetCollections = ['AntiKt4PV0TrackJets', 'AntiKt4LCTopoJets' ]
JetCollections = ['AntiKt4LCTopoJets' ]

if doRetag:
  JetCollectionList = [ (JetCollection,
                         JetCollection.replace('ZTrack', 'Track').replace('PV0Track', 'Track'))
                        for JetCollection in JetCollections ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt4TopoEM->AntiKt4EMTopo" ]

  BTaggingFlags.Jets = [ name[1][:-4] for name in JetCollectionList]
  btag = "BTagging_"
  AuthorSubString = [ btag+name[1][:-4] for name in JetCollectionList]
  tmpSVname = "SecVtx"
  tmpJFVxname = "JFVtx"
  SA = 'standalone_'
  ######################## WARNING ########################################
  from BTagging.BTaggingConfiguration import doNotCheckForTaggerObstacles
  doNotCheckForTaggerObstacles()
  ####BTaggingFlags.PrimaryVertexCollectionName = "PrimaryVertices"
  #########################################################################

  from BTagging.BTaggingConfiguration import setupJetBTaggerTool, getJetCollectionTool
  from BTagging.BTaggingConf import Analysis__StandAloneJetBTaggerAlg as StandAloneJetBTaggerAlg
  NotInJetToolManager = [] # For jet collections

  for i, jet in enumerate(JetCollectionList):
    try:
      btagger = setupJetBTaggerTool(ToolSvc, JetCollection=jet[1][:-4], AddToToolSvc=True,
                                    Verbose=BTaggingFlags.OutputLevel < 3,
                                    options={"name"         : AuthorSubString[i].lower(),
                                             "BTagName"     : AuthorSubString[i],
                                             "BTagJFVtxName": tmpJFVxname,
                                             "BTagSVName"   : tmpSVname,
                                             }
                                    )
      SAbtagger = StandAloneJetBTaggerAlg(name=SA + AuthorSubString[i].lower(),
                                          JetBTaggerTool=btagger,
                                          JetCollectionName = jet[0],
                                          #OutputLevel = DEBUG
                                          )
      algSeq += SAbtagger
      print SAbtagger
      print "Vale2 "
    except AttributeError as error:
      print '#BTAG# --> ' + str(error)
      print '#BTAG# --> ' + jet[1]
      print '#BTAG# --> ' + AuthorSubString[i]
      NotInJetToolManager.append(AuthorSubString[i])

  if len(NotInJetToolManager) > 0:
    AuthorSubString = list(set(AuthorSubString) - set(NotInJetToolManager))

  # Both standard and aux container must be listed explicitly.
  # For release 19, the container version must be explicit.
  BaseName = "xAOD::BTaggingContainer_v1#"
  BaseAuxName = "xAOD::BTaggingAuxContainer_v1#"
  #AOD list
  BTaggingFlags.btaggingAODList += [ BaseName + author for author in AuthorSubString]
  BTaggingFlags.btaggingAODList += [ BaseAuxName + author + 'Aux.' for author in AuthorSubString]
  BTaggingFlags.btaggingAODList += [ BaseName + author + 'AOD' for author in AuthorSubString]
  BTaggingFlags.btaggingAODList += [ BaseAuxName + author + 'AODAux.' for author in AuthorSubString]
  #ESD list
  BTaggingFlags.btaggingESDList += [ BaseName + author for author in AuthorSubString]
  BTaggingFlags.btaggingESDList += [ BaseAuxName + author + 'Aux.' for author in AuthorSubString]
  #AOD list SeCVert
  BaseNameSecVtx = "xAOD::VertexContainer_v1#"
  BaseAuxNameSecVtx = "xAOD::VertexAuxContainer_v1#"
  BTaggingFlags.btaggingAODList += [ BaseNameSecVtx + author + tmpSVname for author in AuthorSubString]
  BTaggingFlags.btaggingAODList += [ BaseAuxNameSecVtx + author + tmpSVname + 'Aux.-vxTrackAtVertex' for author in AuthorSubString]
  #ESD list
  BTaggingFlags.btaggingESDList += [ BaseNameSecVtx + author + tmpSVname for author in AuthorSubString]
  BTaggingFlags.btaggingESDList += [ BaseAuxNameSecVtx + author + tmpSVname + 'Aux.-vxTrackAtVertex' for author in AuthorSubString]
  #AOD list JFSeCVert
  BaseNameJFSecVtx = "xAOD::BTagVertexContainer_v1#"
  BaseAuxNameJFSecVtx = "xAOD::BTagVertexAuxContainer_v1#"
  BTaggingFlags.btaggingAODList += [ BaseNameJFSecVtx + author + tmpJFVxname for author in AuthorSubString]
  BTaggingFlags.btaggingAODList += [ BaseAuxNameJFSecVtx + author + tmpJFVxname + 'Aux.' for author in AuthorSubString]
  #ESD list
  BTaggingFlags.btaggingESDList += [ BaseNameJFSecVtx + author + tmpJFVxname for author in AuthorSubString]
  BTaggingFlags.btaggingESDList += [ BaseAuxNameJFSecVtx + author + tmpJFVxname + 'Aux.' for author in AuthorSubString]


  ### Valerio: test for editing the tools configuration from here
  ### these are just some examples of the possibilities that Wouter's tool allows
  from BTagging.BTaggingConfiguration import getTool
  ip3dTRKsel=getTool("IP3DTrackSelector", "BTagTrackToJetAssociator")
  ip2dTRKsel=getTool("IP2DTrackSelector", "BTagTrackToJetAssociator")
  ip3dTRKsel.nHitBLayer=0
  ip2dTRKsel.nHitBLayer=0
  #value="NoJetFitterCombNN"
  value="Default"
  mv2c00Tool=getTool("MV2c00Tag","BTagTrackToJetAssociator","AntiKt4LCTopo")
  #mv2c00Tool.trainingConfig=value
  ####mv2c00Tool.OutputLevel=DEBUG
  mv2c10Tool=getTool("MV2c10Tag","BTagTrackToJetAssociator","AntiKt4LCTopo")
  #mv2c10Tool.trainingConfig=value
  mv2c20Tool=getTool("MV2c20Tag","BTagTrackToJetAssociator","AntiKt4LCTopo")
  #mv2c20Tool.trainingConfig=value
  print mv2c00Tool

####################################################################################
####################################################################################
####################################################################################

# ====================================================================
# Add own algorithm and tools
# ====================================================================
for JetCollection in JetCollections:
  alg = CfgMgr.btagIBLAnalysisAlg("BTagAlg_"+JetCollection, OutputLevel=INFO) #DEBUG
  alg.JetCollectionName = JetCollection
  if "Track" in JetCollection:
    alg.JetPtCut = 5.e3
    alg.CleanJets = False
    alg.CalibrateJets = False
  else:
    alg.JetPtCut = 20.e3
  alg.ReduceInfo=ReduceInfo
  alg.DoMSV=DoMSV
  alg.Rel20=Rel20
  alg.JetCleaningTool.CutLevel = "LooseBad" # options: "VeryLooseBad","LooseBad",
  if not doComputeReference: algSeq += alg
calibfile = "JetCalibTools/data/CalibrationConfigs/JES_Full2012dataset_Preliminary_MC14.config"
if Rel20: calibfile = "JES_Full2012dataset_Preliminary_MC14.config"
ToolSvc += CfgMgr.JetCalibrationTool("JetCalibrationTool",
                                     IsData=False,
                                     ConfigFile=calibfile,
                                     CalibSequence="EtaJES",
                                     JetCollection="AntiKt4LCTopo")
