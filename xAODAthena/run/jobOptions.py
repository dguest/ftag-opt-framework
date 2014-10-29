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
svcMgr.EventSelector.InputCollections = [
    # ttbar 8TeV
    #ttbar8folder + "AOD.01571944._010058.pool.root.1",
    # ttbar 13TeV 5787
    ttbar13folder1 + "AOD.01587947._000270.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._000291.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._000420.pool.root.2",
    ## ttbar13folder1 + "AOD.01587947._000501.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._000673.pool.root.2",
    ## ttbar13folder1 + "AOD.01587947._000841.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._000907.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._001089.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._001193.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._001289.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._001663.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._001699.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002031.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002215.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002240.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002328.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002378.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002541.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002840.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002848.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._002891.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._003593.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._003650.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._003703.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._004120.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._004301.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._004608.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._004689.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._004736.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._004887.pool.root.1",
    ## ttbar13folder1 + "AOD.01587947._004933.pool.root.3",
    ## ttbar13folder1 + "AOD.01587947._004948.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._010051.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._010064.pool.root.3",
    ## ttbar13folder2 + "AOD.01587949._010164.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._010208.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._010269.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._010766.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._011022.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._011077.pool.root.4",
    ## ttbar13folder2 + "AOD.01587949._011409.pool.root.2",
    ## ttbar13folder2 + "AOD.01587949._011491.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._011695.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._011901.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012038.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012202.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012218.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012315.pool.root.2",
    ## ttbar13folder2 + "AOD.01587949._012496.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012541.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012543.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012625.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._012863.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._013416.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._013683.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._013815.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._014172.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._014202.pool.root.2",
    ## ttbar13folder2 + "AOD.01587949._014216.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._014310.pool.root.2",
    ## ttbar13folder2 + "AOD.01587949._014729.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._014773.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._014794.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._014843.pool.root.1",
    ## ttbar13folder2 + "AOD.01587949._014913.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._015046.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._015128.pool.root.2",
    ## ttbar13folder3 + "AOD.01587950._015139.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._015144.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._015558.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._015820.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._015993.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._016263.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._016341.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._016375.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._016494.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._016786.pool.root.2",
    ## ttbar13folder3 + "AOD.01587950._016908.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._016932.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._016962.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017065.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017095.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017096.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017265.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017347.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017380.pool.root.5",
    ## ttbar13folder3 + "AOD.01587950._017556.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017745.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._017765.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._018320.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._018504.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._018575.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._018805.pool.root.2",
    ## ttbar13folder3 + "AOD.01587950._019162.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._019297.pool.root.3",
    ## ttbar13folder3 + "AOD.01587950._019365.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._019387.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._019692.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._019712.pool.root.1",
    ## ttbar13folder3 + "AOD.01587950._019856.pool.root.2",
    ## ttbar13folder3 + "AOD.01587950._019927.pool.root.1"
    # Zprime
    ## Zprime13folder + "AOD.01598032._000007.pool.root.1",
    ## Zprime13folder + "AOD.01598032._000008.pool.root.1",
    ## Zprime13folder + "AOD.01598032._000097.pool.root.1",
    ## Zprime13folder + "AOD.01598032._000126.pool.root.1",
    ## Zprime13folder + "AOD.01598032._000151.pool.root.1",
    ## Zprime13folder + "AOD.01598032._000197.pool.root.1",
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
#alg.JetCleaningTool.CutLevel = "LooseBad" # options: "VeryLooseBad","LooseBad","MediumBad","TightBad"
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
