# ====================================================================
# MAIN SWITCHES
# ====================================================================
ReduceInfo   =False ##write minimal amount of info on the output file 
#(only option that will work on original DC14 xAOD)
doRetag      =True  ## perform retagging
doRecomputePV=True  ## need to be true when re-tagging to recover JetFitter performance

if doRetag==False:
  ReduceInfo=True

if doRecomputePV:
  doRetag=True

#theApp.EvtMax = -1
import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(-1)

## main test file
jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/v/vdao//xAODs/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5787_tid01587947_00/AOD.01587947._004222.pool.root.1"]

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
  TruthDecor = DerivationFramework__TrackParametersForTruthParticles( name = "TruthTPDecor",
                                                                      ##OutputLevel = DEBUG,
                                                                      DecorationPrefix ="")
  ToolSvc +=TruthDecor
  augmentationTools = [TruthDecor]
  from DerivationFrameworkCore.DerivationFrameworkCoreConf import DerivationFramework__CommonAugmentation
  algSeq += CfgMgr.DerivationFramework__CommonAugmentation("MyDFTSOS_KERN",
                                                           AugmentationTools = augmentationTools,
                                                           OutputLevel = DEBUG )

### to change the references

#if BTaggingFlags.Active: 
#BTaggingFlags.CalibrationFromLocalReplica = True 
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/' 
#BTaggingFlags.CalibrationTag = 'k0001' 

if doRetag:
  JetCollectionList = ['AntiKt4LCTopoJets' ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt4EMTopo->AntiKt4TopoEM,AntiKt4H1Topo" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt4LCTopo->AntiKt4LCTopo,AntiKt4TopoEM,AntiKt4H1Topo" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt10LCTopo->AntiKt6LCTopo,AntiKt6TopoEM,AntiKt4TopoEM,AntiKt4H1Topo" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt10Truth->AntiKt6TopoEM,AntiKt4TopoEM,AntiKt4H1Topo" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt10TruthWZ->AntiKt6TopoEM,AntiKt4TopoEM,AntiKt4H1Topo" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt4Truth->AntiKt4TopoEM,AntiKt4H1Topo" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt4TruthWZ->AntiKt4TopoEM,AntiKt4H1Topo" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt4Track->AntiKt4TopoEM" ]
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt3Track->AntiKt4TopoEM" ]

  BTaggingFlags.Jets = [ name[:-4] for name in JetCollectionList]
  btag = "BTagging_"
  AuthorSubString = [ btag+name[:-4] for name in JetCollectionList]
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
      btagger = setupJetBTaggerTool(ToolSvc, JetCollection=jet[:-4], AddToToolSvc=True,
                                    Verbose=BTaggingFlags.OutputLevel < 3,
                                    options={"name"         : AuthorSubString[i].lower(),
                                             "BTagName"     : AuthorSubString[i],
                                             "BTagJFVtxName": tmpJFVxname,
                                             "BTagSVName"   : tmpSVname,
                                             }
                                    )
      jet = jet.replace("Track", "PV0Track") #It is important this line is here, and not above the btagger = line, for reference mode (since it needs to set the stream name correctly).
      SAbtagger = StandAloneJetBTaggerAlg(name=SA + AuthorSubString[i].lower(),
                                          JetBTaggerTool=btagger,
                                          JetCollectionName = jet,
                                          #OutputLevel = DEBUG
                                          )
      algSeq += SAbtagger
      print SAbtagger
      print "Vale2 " 
    except AttributeError as error:
      print '#BTAG# --> ' + str(error)
      print '#BTAG# --> ' + jet
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


####################################################################################
####################################################################################
####################################################################################

# ====================================================================
# Add own algorithm and tools
# ====================================================================
alg = CfgMgr.btagIBLAnalysisAlg(OutputLevel=INFO) #DEBUG
alg.ReduceInfo=ReduceInfo
alg.JetCleaningTool.CutLevel = "LooseBad" # options: "VeryLooseBad","LooseBad",
algSeq += alg
ToolSvc += CfgMgr.JetCalibrationTool("JetCalibrationTool", 
                                     IsData=False,
                                     ConfigFile="JetCalibTools/data/CalibrationConfigs/JES_Full2012dataset_Preliminary_MC14.config", 
                                     CalibSequence="EtaJES",
                                     JetCollection="AntiKt4LCTopo")
