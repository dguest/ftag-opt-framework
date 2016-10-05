if doSMT: 
    BTaggingFlags.SoftMu = True

if doComputeReference:
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


##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
##########################################################################################################################################################
### THIS is the full retagging configuration
if doRetag:
  JetCollectionList = [ (JetCollection,
                         JetCollection.replace('ZTrack', 'Track').replace('PV0Track', 'Track'))
                        for JetCollection in JetCollections ]
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
    except AttributeError as error:
      print '#BTAG# --> ' + str(error)
      print '#BTAG# --> ' + jet[1]
      print '#BTAG# --> ' + AuthorSubString[i]
      NotInJetToolManager.append(AuthorSubString[i])

  if len(NotInJetToolManager) > 0:
    AuthorSubString = list(set(AuthorSubString) - set(NotInJetToolManager))

  # Both standard and aux container must be listed explicitly. For release 19, the container version must be explicit.
  BaseName    = "xAOD::BTaggingContainer_v1#"
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
  BaseNameSecVtx    = "xAOD::VertexContainer_v1#"
  BaseAuxNameSecVtx = "xAOD::VertexAuxContainer_v1#"
  BTaggingFlags.btaggingAODList += [ BaseNameSecVtx + author + tmpSVname for author in AuthorSubString]
  BTaggingFlags.btaggingAODList += [ BaseAuxNameSecVtx + author + tmpSVname + 'Aux.-vxTrackAtVertex' for author in AuthorSubString]
  #ESD list
  BTaggingFlags.btaggingESDList += [ BaseNameSecVtx + author + tmpSVname for author in AuthorSubString]
  BTaggingFlags.btaggingESDList += [ BaseAuxNameSecVtx + author + tmpSVname + 'Aux.-vxTrackAtVertex' for author in AuthorSubString]
  #AOD list JFSeCVert
  BaseNameJFSecVtx    = "xAOD::BTagVertexContainer_v1#"
  BaseAuxNameJFSecVtx = "xAOD::BTagVertexAuxContainer_v1#"
  BTaggingFlags.btaggingAODList += [ BaseNameJFSecVtx + author + tmpJFVxname for author in AuthorSubString]
  BTaggingFlags.btaggingAODList += [ BaseAuxNameJFSecVtx + author + tmpJFVxname + 'Aux.' for author in AuthorSubString]
  #ESD list
  BTaggingFlags.btaggingESDList += [ BaseNameJFSecVtx + author + tmpJFVxname for author in AuthorSubString]
  BTaggingFlags.btaggingESDList += [ BaseAuxNameJFSecVtx + author + tmpJFVxname + 'Aux.' for author in AuthorSubString]
