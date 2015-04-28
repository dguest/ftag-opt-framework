# ====================================================================
# MAIN SWITCHES
# ====================================================================
ReduceInfo   =False ##write minimal amount of info on the output file
DoMSV        =False ##include variables for MSV tagger
Rel20        =True  ##switch between rel19 and rel20 .... this is a bit dangerous at this point. Setitng to false will not revert jet calibration for instance
#(only option that will work on original DC14 xAOD)

doRetag      =True   ## perform retagging
doComputeReference=False
JetCollections = [ 'AntiKt4LCTopoJets', 'AntiKt4EMTopoJets' ]

# ===================================================================
# ===================================================================
# ===================================================================
# ===================================================================
doRecomputePV=False  ## do not touch unless you know what you are doing
if doComputeReference:
  ReduceInfo=True
  doRetag   =True
  doRecomputePV=False

import glob
from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(-1)

## main test file: TTbar xAOD r19 mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad
#jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/v/vdao//xAODs/Rel20/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.AOD.e2928_s1982_s2008_r6114_r6104_tid04859517_00/AOD.04859517._000001.pool.root.1"]
#jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/v/vdao//public/AOD.01587947._004222.pool.root.1"]
jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/v/vdao//xAODs/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.merge.AOD.e2928_s1982_s2008_r6205_r6223_tid05192995_00/AOD.05192995._000032.pool.root.1" ]
# ====================================================================
# Define output ntuple file name
# ====================================================================
svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple.root' OPT='RECREATE'"]
# ====================================================================


############ Valerio: put first all the RecExCommon things, then the B-tagging stuff
if doRecomputePV:
  from InDetRecExample.InDetJobProperties import InDetFlags
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

################################## GEO Business ##################################
from AthenaCommon.GlobalFlags import globalflags
print "detDescr from global flags= "+str(globalflags.DetDescrVersion)
from AtlasGeoModel.InDetGMJobProperties import GeometryFlags as geoFlags
print "geoFlags.Run() = "+geoFlags.Run()
print "geoFlags.isIBL() = "+str(  geoFlags.isIBL() )


from BTagging.BTaggingFlags import BTaggingFlags
## chainging to other official calib file
BTaggingFlags.CalibrationTag = 'BTagCalibRUN12-08-05'
#BTaggingFlags.CalibrationFromLocalReplica = True
#BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/'
#BTaggingFlags.CalibrationTag = 'Run2DC14' ## '0801C' ##'k0002'

if  doComputeReference:
  BTaggingFlags.Runmodus = "reference"
  BTaggingFlags.SV0 = False
  BTaggingFlags.SV2 = False
  BTaggingFlags.GbbNNTag = False
  BTaggingFlags.JetFitterTag  =False
  BTaggingFlags.JetFitterNN   =False
  BTaggingFlags.JetFitterCharm=False
  BTaggingFlags.MVb = False
  BTaggingFlags.MV1 = False
  BTaggingFlags.MV2 = False
  BTaggingFlags.MV1c = False
  BTaggingFlags.MV2c00 = False
  BTaggingFlags.MV2c10 = False
  BTaggingFlags.MV2c20 = False
  BTaggingFlags.MV2c100= False
  BTaggingFlags.MV2m   = False
  BTaggingFlags.MultiSVbb1 = False
  BTaggingFlags.MultiSVbb2 = False
  pass

if doRecomputePV:
  BTaggingFlags.PrimaryVertexCollectionName='PrimaryVerticesValerio'

# Demonstrate to run on 2 jet collections at the same time
if doRetag:
  JetCollectionList = [ (JetCollection,
                         JetCollection.replace('ZTrack', 'Track').replace('PV0Track', 'Track'))
                        for JetCollection in JetCollections ]
  print "  Valerio: "
  print JetCollectionList
  BTaggingFlags.CalibrationChannelAliases += [ "AntiKt4TopoEM->AntiKt4EMTopo" ]

  BTaggingFlags.Jets = [ name[1][:-4] for name in JetCollectionList]
  btag = "BTagging_"
  AuthorSubString = [ btag+name[1][:-4] for name in JetCollectionList]
  tmpSVname = "SecVtx"
  tmpJFVxname = "JFVtx"
  SA = 'standalone_'

  from BTagging.BTaggingConfiguration import getConfiguration
  BTagConf = getConfiguration()
  BTagConf.doNotCheckForTaggerObstacles()
  NotInJetToolManager = [] # For jet collections

  from BTagging.BTaggingConf import Analysis__StandAloneJetBTaggerAlg as StandAloneJetBTaggerAlg
  
  for i, jet in enumerate(JetCollectionList):
    try:
      btagger = BTagConf.setupJetBTaggerTool(ToolSvc, JetCollection=jet[1][:-4], AddToToolSvc=True,
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

  
####################################################################################
####################################################################################
####################################################################################

# ====================================================================
# Add own algorithm and tools
# ====================================================================
for JetCollection in JetCollections:
  alg = CfgMgr.btagIBLAnalysisAlg("BTagDumpAlg_"+JetCollection, OutputLevel=INFO) #DEBUG
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
  
  ###print JetCollection
  calibfile = "JES_Prerecommendation2015_Feb2015.config"
  collectionForTool="AntiKt4LCTopo"
  calSeg           ="JetArea_Residual_EtaJES"
  if "EM" in JetCollection: 
    collectionForTool="AntiKt4TopoEM"
    calibfile        ="JES_Prerecommendation2015_Feb2015_Internal.config"
    calSeg           ="JetArea_Residual_EtaJES_GSC"
  print collectionForTool
  ToolSvc += CfgMgr.JetCalibrationTool("BTagDumpAlg_"+JetCollection+"_JCalib",#"JCalib_"+JetCollection,
                                       IsData=False,
                                       ConfigFile=calibfile,
                                       CalibSequence=calSeg,
                                       JetCollection=collectionForTool) 

