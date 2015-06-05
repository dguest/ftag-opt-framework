import os
import sys

dsList=open("mc_samples.txt",'r')
lines=dsList.readlines()
username="vdao"

###suffix=".BTAGNTUP_RetagRun1"
###suffix=".BTAGNTUP_RefFix2"

##suffix=".BTAGNTUP_Retag20146"
##suffix=".BTAGNTUP_OrigV2"
##suffix=".BTAGNTUP_OrigV3"
##suffix=".BTAGNTUP_OrigV5slim"
##suffix=".BTAGNTUP_OrigV6slim" ### improving jet calibration
##suffix=".BTAGNTUP_OrigV6full"
##suffix=".BTAGNTUP_OrigV7full" ### ltest jet calibration
##suffix=".BTAGNTUP_V7retag"    ### latest jet calibration+retag
##suffix=".BTAGNTUP_V8slim"       ### correct AODfix
suffix=".BTAGNTUP_V8full"       ### correct AODfix

#suffix=".BTAGNTUP_Retag20143_v2"
#suffix=".BTAGNTUP_RetagIPxDtrick"
#suffix=".BTAGNTUP_Refs2"
#suffix=".BTAGNTUP_Orig6"
#suffix=".BTAGNTUP_v20MV2trainTrkPt"
#suffix=".BTAGNTUP_DC14r20Retag"
#suffix=".BTAGNTUP_v20MV2trainTrkPt"
#suffix=".BTAGNTUP_v20MV2trainMoreMV2s"


def submitJob(ds) :
    com = "pathena  jobOptions_Tag.py "
    ###com = "pathena  jobOptions_Tag_8TeV.py "
    ###com = "pathena jobOptions_Tag_data.py "
    #######com = "pathena  jobOptions_ReTag_DC14.py "

    ##com += "--express "
    com += " --skipScout "
    ####################################################com += " --allowTaskDuplication "
    ##com += " --Debug "
    ##com += " --official --voms atlas:/atlas/perf-flavtag/Role=production "
    
    com += "--inDS " + ds + " "
    oDS = "user."+username+"."+ds.replace("/","")+suffix
    ##oDS="group.perf-flavtag."+ds.replace("/","")+suffix

    print oDS+"  has length: "+str(len(oDS))
    while len(oDS) > 115 :
        print len(oDS)," too long!!!"
        splODS=oDS.split("_")
        splODS.pop(2)
        oDS="_".join(splODS)
        pass
    print "final: "+oDS+"  has length: "+str(len(oDS))
    com += "--outDS "+ oDS + " "
  
    com += "--nFilesPerJob 5 "

    #com += "--extFile mycool.db,BTagCalibRUN2Onl-08-05-Run1Special.root "
    #com += "--addPoolFC ,BTagCalibRUN2Onl-08-05-Run1Special.root "

    #com += "--destSE UNI-FREIBURG_SCRATCHDISK "
    com += "--excludedSite=ANALY_CPPM,ANALY_IN2P3-CC "

    return com


############################################################################################################################################
############################################################################################################################################
############################################################################################################################################
############################################################################################################################################
for ds in lines :
    if "#" in ds : continue
    if "mc" not in ds and "valid" not in ds and "group" not in ds and "data" not in ds: continue
    
    print " "
    print "///////////////////////////////////////////////////////////////////////////////////////"
    print ds
    command=submitJob(ds.replace("\n",""))
    print command
    os.system(command)
    pass

