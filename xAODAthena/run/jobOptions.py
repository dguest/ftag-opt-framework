# ====================================================================
# Specify input files
# ====================================================================
theApp.EvtMax = -1
import AthenaPoolCnvSvc.ReadAthenaPool
svcMgr.EventSelector.InputCollections = [
    #"/afs/cern.ch/user/m/marx/btag/xAODAthena/run/myxAOD.pool.root",
    "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._000183.pool.root.2",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._000356.pool.root.1",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._000551.pool.root.2",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._000592.pool.root.1",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._000603.pool.root.1",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._000858.pool.root.1",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._000961.pool.root.1",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._001032.pool.root.1",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._001663.pool.root.1",
    ## "/afs/cern.ch/work/m/marx/btag/ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/AOD.01529511._001985.pool.root.1"
     ]
# ====================================================================
# Define output file name and stream name
# ====================================================================
streamName = "HistoStream"
fileName = "histos.root"
# ====================================================================
# Fetch the AthAlgSeq, i.e., one of the existing master sequences where one should attach all algorithms
# ====================================================================
algSeq = CfgMgr.AthSequencer("AthAlgSeq")
# ====================================================================
# Add own algorithm
# ====================================================================
algSeq += CfgMgr.btagIBLAnalysisAlg("MyTry", RootStreamName = streamName, RootDirName = "/Histograms", OutputLevel=INFO) #DEBUG
# ====================================================================
# Define an output root file for histograms
# ====================================================================
from OutputStreamAthenaPool.MultipleStreamManager import MSMgr
HistoXAODStream = MSMgr.NewRootStream( streamName, fileName )
# ====================================================================
# Create a new xAOD
# ====================================================================
XAODStream = MSMgr.NewPoolRootStream( "xAODStream", "xAOD.pool.root" )
## XAODStream.AddItem(['xAOD::EventInfo_v1#EventInfo'])
## XAODStream.AddItem(['xAOD::EventInfo_v1#EventInfoAux.'])
XAODStream.AddItem(['xAOD::JetContainer_v1#AntiKt4LCTopoJets'])
XAODStream.AddItem(['xAOD::JetAuxContainer_v1#AntiKt4LCTopoJetsAux.'])
XAODStream.AddItem(['xAOD::BTaggingContainer_v1#BTagging_AntiKt4LCTopo'])
XAODStream.AddItem(['xAOD::BTaggingAuxContainer_v1#BTagging_AntiKt4LCTopoAux.'])
XAODStream.AddItem(['xAOD::TruthEventContainer_v1#TruthEvent'])
XAODStream.AddItem(['xAOD::TruthEventAuxContainer_v1#TruthEventAux.'])
XAODStream.AddItem(['xAOD::TruthParticleContainer_v1#TruthParticle'])
XAODStream.AddItem(['xAOD::TruthParticleAuxContainer_v1#TruthParticleAux.'])
XAODStream.AddItem(['xAOD::TruthVertexContainer_v1#TruthVertex'])
XAODStream.AddItem(['xAOD::TruthVertexAuxContainer_v1#TruthVertexAux.'])
