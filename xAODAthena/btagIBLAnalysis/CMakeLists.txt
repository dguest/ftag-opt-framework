################################################################################
# Package: btagIBLAnalysis
################################################################################

# Declare the package name:
atlas_subdir( btagIBLAnalysis )

# Declare the package's dependencies:
atlas_depends_on_subdirs( PUBLIC
                          GaudiKernel
                          PRIVATE
                          Control/AthenaBaseComps
                          DataQuality/GoodRunsLists
                          Event/xAOD/xAODBTagging
                          Event/xAOD/xAODEventInfo
                          Event/xAOD/xAODEventShape
                          Event/xAOD/xAODJet
                          Event/xAOD/xAODMuon
                          Event/xAOD/xAODTracking
                          Event/xAOD/xAODTrigger
                          Event/xAOD/xAODTruth
                          InnerDetector/InDetRecTools/InDetTrackSelectionTool
                          InnerDetector/InDetRecTools/TrackVertexAssociationTool
                          PhysicsAnalysis/AnalysisCommon/ParticleJetTools
                          PhysicsAnalysis/AnalysisCommon/PileupReweighting
                          Reconstruction/Jet/JetCalibTools
                          Reconstruction/Jet/JetInterface
                          Reconstruction/RecoTools/ITrackToVertex
			  Reconstruction/Jet/JetSubStructureUtils
                          Tracking/TrkVertexFitter/TrkVertexFitterInterfaces
                          Trigger/TrigAnalysis/TrigDecisionTool )

# External dependencies:
find_package( ROOT COMPONENTS Core Tree MathCore Hist RIO pthread )

# Component(s) in the package:
atlas_add_component( btagIBLAnalysis
                     src/*.cxx
                     src/components/*.cxx
                     INCLUDE_DIRS ${ROOT_INCLUDE_DIRS}
                     LINK_LIBRARIES ${ROOT_LIBRARIES} GaudiKernel AthenaBaseComps GoodRunsListsLib xAODBTagging xAODEventInfo xAODEventShape xAODJet xAODMuon xAODTracking xAODTrigger xAODTruth TrackVertexAssociationToolLib ParticleJetToolsLib JetSubStructureUtils PileupReweightingLib JetInterface ITrackToVertex TrkVertexFitterInterfaces TrigDecisionToolLib )

