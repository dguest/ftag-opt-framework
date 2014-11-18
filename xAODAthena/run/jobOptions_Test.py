# ====================================================================
# Specify input files
# ====================================================================

#theApp.EvtMax = -1
#import AthenaPoolCnvSvc.ReadAthenaPool
# define folders where samples are located
mainfolder = "/afs/cern.ch/work/m/marx/btag/"
ttbar8folder = mainfolder + "ttbar/8TeV/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/"
ttbar13folder1 = mainfolder + "ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5787_tid01587947_00/"
ttbar13folder2 = mainfolder + "ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5787_tid01587949_00/"
ttbar13folder3 = mainfolder + "ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5787_tid01587950_00/"
Zprime13folder = mainfolder + "Zprime/13TeV/mc14_13TeV.110903.Pythia8_AU2MSTW2008LO_zprime1000_tt.merge.AOD.e2743_s1982_s2008_r5787_r5853_tid01598032_00/"
# define input collection

mainfolder = "/afs/cern.ch/work/v/vdao/xAODs/"
foldername = mainfolder + "mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/"

import glob
#svcMgr.EventSelector.InputCollections = glob.glob(foldername+"*.root*")

from AthenaCommon.AthenaCommonFlags import jobproperties as jp
jp.AthenaCommonFlags.EvtMax.set_Value_and_Lock(-1)
jp.AthenaCommonFlags.FilesInput = [ "/afs/cern.ch/work/v/vdao//xAODs/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5787_tid01587947_00/AOD.01587947._004222.pool.root.1"]
#jp.AthenaCommonFlags.FilesInput= glob.glob(foldername+"*.root*")
#jp.AthenaCommonFlags.FilesInput=[
#    "/afs/cern.ch/work/v/vdao//xAODs/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/AOD.01571944._010005.pool.root.2"
#]
#    "/afs/cern.ch/work/v/vdao//xAODs/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/AOD.01571944._010007.pool.root.1",
#    "/afs/cern.ch/work/v/vdao//xAODs/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/AOD.01571944._010012.pool.root.1",
#    "/afs/cern.ch/work/v/vdao//xAODs/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/AOD.01571944._010020.pool.root.1",
#    "/afs/cern.ch/work/v/vdao//xAODs/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/AOD.01571944._010026.pool.root.1",
#    "/afs/cern.ch/work/v/vdao//xAODs/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/AOD.01571944._010033.pool.root.2",
#    "/afs/cern.ch/work/v/vdao//xAODs/mc14_8TeV.117050.PowhegPythia_P2011C_ttbar.merge.AOD.e1727_s1933_s1911_r5778_r5625_tid01571944_00/AOD.01571944._010038.pool.root.2",
#]
# ====================================================================
# Define output ntuple file name
# ====================================================================
svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple_110401_ttbar_13TeV_r5787.root' OPT='RECREATE'"]
# ====================================================================
# Fetch the AthAlgSeq, i.e., one of the existing master sequences where one should attach all algorithms
# ====================================================================
algSeq = CfgMgr.AthSequencer("AthAlgSeq")
#from AthenaCommon.AlgSequence import AlgSequence
#algSeq = AlgSequence()
# ====================================================================
# Add own algorithm and tools
# ====================================================================

##include ("BTagging_standAlone_Rel19_NewConf.py")
#from RecExConfig.RecFlags import rec
#from RecExConfig.RecAlgsFlags import recAlgs
#from AthenaCommon.AthenaCommonFlags import athenaCommonFlags

if not 'BTaggingFlags' in dir():
  from BTagging.BTaggingFlags import BTaggingFlags

test=True

#if BTaggingFlags.Active: 
#######BTaggingFlags.CalibrationFromLocalReplica = True 
#######BTaggingFlags.CalibrationFolderRoot = '/GLOBAL/BTagCalib/' 
#######BTaggingFlags.CalibrationTag = 'k0001' 
  
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
BTaggingFlags.PrimaryVertexCollectionName = "PrimaryVertices"
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

## from Anthony
import MagFieldServices.SetupField
# --- disable error protection of RecExCommon
##aathenaCommonFlags.AllowIgnoreConfigError.set_Value_and_Lock(False)
from AthenaCommon.DetFlags import DetFlags 
DetFlags.ID_setOn()
DetFlags.Calo_setOff()
DetFlags.Muon_setOff()

from RecExConfig.RecFlags import rec
rec.doESD.set_Value_and_Lock        (False)###
rec.doWriteESD.set_Value_and_Lock        (False)###
rec.doAOD.set_Value_and_Lock             (False)
rec.doWriteAOD.set_Value_and_Lock        (False)
#rec.doTAG.set_Value_and_Lock             (False)
rec.doWriteTAG.set_Value_and_Lock        (False)
rec.doDPD.set_Value_and_Lock             (False)

include ("RecExCommon/RecExCommon_topOptions.py")

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


# ====================================================================
# Add own algorithm and tools
# ====================================================================
alg = CfgMgr.btagIBLAnalysisAlg(OutputLevel=INFO) #DEBUG
alg.JetCleaningTool.CutLevel = "LooseBad" # options: "VeryLooseBad","LooseBad","MediumBad","TightBad"
algSeq += alg
ToolSvc += CfgMgr.JetCalibrationTool("JetCalibrationTool", IsData=False,ConfigFile="JetCalibTools/data/CalibrationConfigs/JES_Full2012dataset_Preliminary_MC14.config", CalibSequence="EtaJES", JetCollection="AntiKt4LCTopo")


# ====================================================================
# Define an output root file for histograms
# ====================================================================
#streamName = "HistoStream"
#fileName = "histos.root"
#from OutputStreamAthenaPool.MultipleStreamManager import MSMgr
#HistoXAODStream = MSMgr.NewRootStream( streamName, fileName )
# ====================================================================
# Create a new xAOD
# ====================================================================
#XAODStream = MSMgr.NewPoolRootStream( "xAODStream", "xAOD.pool.root" )
## XAODStream.AddItem(['xAOD::EventInfo_v1#EventInfo'])
## XAODStream.AddItem(['xAOD::EventInfo_v1#EventInfoAux.'])
#XAODStream.AddItem(['xAOD::JetContainer_v1#AntiKt4LCTopoJets'])
#XAODStream.AddItem(['xAOD::JetAuxContainer_v1#AntiKt4LCTopoJetsAux.'])
#XAODStream.AddItem(['xAOD::BTaggingContainer_v1#BTagging_AntiKt4LCTopo'])
#XAODStream.AddItem(['xAOD::BTaggingAuxContainer_v1#BTagging_AntiKt4LCTopoAux.'])
#XAODStream.AddItem(['xAOD::TruthEventContainer_v1#TruthEvent'])
#XAODStream.AddItem(['xAOD::TruthEventAuxContainer_v1#TruthEventAux.'])
#XAODStream.AddItem(['xAOD::TruthParticleContainer_v1#TruthParticle'])
#XAODStream.AddItem(['xAOD::TruthParticleAuxContainer_v1#TruthParticleAux.'])
#XAODStream.AddItem(['xAOD::TruthVertexContainer_v1#TruthVertex'])
#XAODStream.AddItem(['xAOD::TruthVertexAuxContainer_v1#TruthVertexAux.'])
