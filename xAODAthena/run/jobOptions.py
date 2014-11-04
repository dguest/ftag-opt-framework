# ====================================================================
# Specify input files
# ====================================================================

theApp.EvtMax = -1
import AthenaPoolCnvSvc.ReadAthenaPool
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
svcMgr.EventSelector.InputCollections = glob.glob(foldername+"*.root*")

svcMgr.EventSelector.InputCollections = [
     "/afs/cern.ch/work/v/vdao//xAODs/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5787_tid01587947_00/AOD.01587947._004222.pool.root.1",
     ]
# ====================================================================
# Define output ntuple file name
# ====================================================================
svcMgr += CfgMgr.THistSvc()
svcMgr.THistSvc.Output += ["BTAGSTREAM DATAFILE='flavntuple_110401_ttbar_13TeV_r5787.root' OPT='RECREATE'"]
# ====================================================================
# Fetch the AthAlgSeq, i.e., one of the existing master sequences where one should attach all algorithms
# ====================================================================
algSeq = CfgMgr.AthSequencer("AthAlgSeq")
# ====================================================================
# Add own algorithm and tools
# ====================================================================
#algSeq += CfgMgr.btagIBLAnalysisAlg("MyTry", RootStreamName = streamName, RootDirName = "/Histograms", OutputLevel=INFO) #DEBUG
alg = CfgMgr.btagIBLAnalysisAlg(OutputLevel=INFO) #DEBUG
######alg.JetCleaningTool.CutLevel = "LooseBad" # options: "VeryLooseBad","LooseBad","MediumBad","TightBad"
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
