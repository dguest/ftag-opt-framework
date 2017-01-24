# include the info flags
from AthenaCommon.Constants import *
from subprocess import call
from glob import glob

# short names for jet collections
_short_jet_names = [
  ("AntiKt","Akt"), ("TrackJets","Tr"), ("TopoJets","To"),
  ("TrimmedPtFrac5SmallR20", "Trm")]
def get_short_name(jet_collection_name):
  for pair in _short_jet_names:
    jet_collection_name = jet_collection_name.replace(*pair)
  return jet_collection_name

def is_af2(af):
    if af.fileinfos.has_key("evt_type"):
        eventTypeList = af.fileinfos["evt_type"]
        if "IS_SIMULATION" in eventTypeList:
            simType = af.fileinfos['metadata']['/Simulation/Parameters']['SimulationFlavour']
            if simType  == 'default' :
                print "VALERIO SAYS: THIS IS FS"
                return False
            elif simType  == 'atlfast' :
                print "VALERIO SAYS: THIS IS AF2"
                return True


def setupTools(ToolSvc, CfgMgr):

    from TrackVertexAssociationTool.TrackVertexAssociationToolConf import CP__TightTrackVertexAssociationTool
    ToolSvc+=CP__TightTrackVertexAssociationTool("TightVertexAssocTool",dzSinTheta_cut=3, doPV=True)

    from InDetTrackSelectionTool.InDetTrackSelectionToolConf import InDet__InDetTrackSelectionTool
    ToolSvc+=InDet__InDetTrackSelectionTool("InDetTrackSelTool", CutLevel="Loose", maxZ0SinTheta=3)
    ToolSvc+=InDet__InDetTrackSelectionTool(
        "CPTrackingLooseLabel", CutLevel="Loose")

    from TrkVertexFitterUtils.TrkVertexFitterUtilsConf import Trk__TrackToVertexIPEstimator
    ToolSvc+=Trk__TrackToVertexIPEstimator("trkIPEstimator")

    from TrigDecisionTool.TrigDecisionToolConf import Trig__TrigDecisionTool
    ToolSvc += Trig__TrigDecisionTool( "TrigDecisionTool" )
    from TrigEDMConfig.TriggerEDM import EDMLibraries
    ToolSvc.TrigDecisionTool.Navigation.Dlls = EDMLibraries

    jvt = CfgMgr.JetVertexTaggerTool('JVT')
    ToolSvc += jvt

    ToolSvc += CfgMgr.CP__PileupReweightingTool(
        "prw",
        OutputLevel = INFO,
        UsePeriodConfig= "MC15"
    )


def get_calibration_tool(CfgMgr, JetCollection, isAF2):
    calibfile        = "JES_Prerecommendation2015_Feb2015.config"
    collectionForTool="AntiKt4LCTopo"
    calSeg           ="JetArea_Residual_EtaJES"
    if "EM" in JetCollection:
        collectionForTool="AntiKt4EMTopo"
        calibfile  ="JES_MC15Prerecommendation_April2015.config"
    if isAF2: calibfile  ="JES_MC15Prerecommendation_AFII_June2015.config"
    calSeg     ="JetArea_Residual_Origin_EtaJES_GSC"
    print collectionForTool
    tool = CfgMgr.JetCalibrationTool(
        "BTagDumpAlg_"+JetCollection+"_JCalib",
        IsData=False,
        ConfigFile=calibfile,
        CalibSequence=calSeg,
        JetCollection=collectionForTool)
    return tool


def add_to_pfc(pattern='BTag*.root'):
    calib_files = glob('BTag*.root')
    assert len(calib_files) < 2
    if calib_files:
        print 'adding {} to pool file catalog'.format(calib_files[0])
        call('coolHist_insertFileToCatalog.py {}'.format(calib_files[0]),
             shell=True)
