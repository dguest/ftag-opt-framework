# ====================================================================
# Specify input files
# ====================================================================
theApp.EvtMax = -1
import AthenaPoolCnvSvc.ReadAthenaPool
mainfolder = "/afs/cern.ch/work/m/marx/btag/"
foldername = mainfolder + "ttbar/13TeV/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5720_tid01529511_00/"
#foldername = mainfolder + "Zprime/13TeV/mc14_13TeV.110903.Pythia8_AU2MSTW2008LO_zprime1000_tt.merge.AOD.e2743_s1982_s2008_r5787_r5853_tid01598032_00/"
svcMgr.EventSelector.InputCollections = [
    #"/afs/cern.ch/user/m/marx/btag/xAODAthena/run/myxAOD.pool.root",
    #foldername + "AOD.01598032._000007.pool.root.1",
    #foldername + "AOD.01598032._000008.pool.root.1",
    #foldername + "AOD.01598032._000097.pool.root.1",
    #foldername + "AOD.01598032._000151.pool.root.1",
    foldername + "AOD.01529511._000075.pool.root.1",
#foldername + "AOD.01529511._000109.pool.root.1",
    #foldername + "AOD.01529511._000112.pool.root.1",
    #foldername + "AOD.01529511._000183.pool.root.2",
    #foldername + "AOD.01529511._000226.pool.root.1",
    #foldername + "AOD.01529511._000302.pool.root.1",
    #foldername + "AOD.01529511._000338.pool.root.1",
    #foldername + "AOD.01529511._000356.pool.root.1",
    #foldername + "AOD.01529511._000450.pool.root.3",
    #foldername + "AOD.01529511._000551.pool.root.2",
    #foldername + "AOD.01529511._000592.pool.root.1",
    #foldername + "AOD.01529511._000603.pool.root.1",
    #foldername + "AOD.01529511._000659.pool.root.1",
    #foldername + "AOD.01529511._000699.pool.root.1",
    #foldername + "AOD.01529511._000747.pool.root.1",
    #foldername + "AOD.01529511._000762.pool.root.1",
    #foldername + "AOD.01529511._000836.pool.root.1",
    #foldername + "AOD.01529511._000856.pool.root.1",
    #foldername + "AOD.01529511._000858.pool.root.1",
    #foldername + "AOD.01529511._000875.pool.root.1",
    #foldername + "AOD.01529511._000961.pool.root.2",
    #foldername + "AOD.01529511._001032.pool.root.1",
    #foldername + "AOD.01529511._001070.pool.root.5",
    #foldername + "AOD.01529511._001181.pool.root.1",
    #foldername + "AOD.01529511._001297.pool.root.1",
    #foldername + "AOD.01529511._001394.pool.root.1",
    #foldername + "AOD.01529511._001456.pool.root.1",
    #foldername + "AOD.01529511._001493.pool.root.2",
    #foldername + "AOD.01529511._001553.pool.root.1",
    #foldername + "AOD.01529511._001594.pool.root.2",
    #foldername + "AOD.01529511._001614.pool.root.2",
    #foldername + "AOD.01529511._001663.pool.root.1",
    #foldername + "AOD.01529511._001664.pool.root.2",
    #foldername + "AOD.01529511._001895.pool.root.1",
    #foldername + "AOD.01529511._001944.pool.root.2",
    #foldername + "AOD.01529511._001985.pool.root.1"
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
#algSeq += CfgMgr.btagIBLAnalysisAlg("MyTry", RootStreamName = streamName, RootDirName = "/Histograms", OutputLevel=INFO) #DEBUG
alg = CfgMgr.btagIBLAnalysisAlg(OutputLevel=INFO) #DEBUG
alg.JetCleaningTool.CutLevel = "LooseBad" # options: "VeryLooseBad","LooseBad","MediumBad","TightBad"
algSeq += alg
# ====================================================================
# Define an output root file for histograms
# ====================================================================
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
