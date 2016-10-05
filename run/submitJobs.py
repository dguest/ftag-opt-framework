import os
import sys

##########################################################################################################################
###
### THINGS THAT SHOULD BE CHANGED BY THE USER
###
##### name to prepend to the production. IT MUST CONTAIN one of the following string:
#     * full : very detailed information (very large ntuples)
#     * slim : no tracking information, still good enough for MVA training
#     * minin: very essential information for the smallest possible ntuples (only good for efficiency determination)
suffix    =".BTAGNTUP_V32minim"         ### update on truth classification + nConstituent for jets
isOfficial=True    ### using official production role or just personal priority
username  ="vdao"  ### only needed if isOfficial=False
inputFile ="mc_samples.txt"
###
###
##########################################################################################################################


###########################################################################################################################
###########################################################################################################################
###########################################################################################################################
dsList=open(inputFile,'r')
lines=dsList.readlines()

def submitJob(ds) :
    com = ""
    if "data" in ds: com = "pathena jobOptions_Tag_data.py "
    else           : com = "pathena jobOptions_Tag.py "
    
    if "slim" in suffix   : com+=' -c "CONTENT=0" '
    elif "full" in suffix : com+=' -c "CONTENT=1" '
    elif "minim" in suffix: com+=' -c "CONTENT=2" '
    else:
        print " WARNING: suffix MUST contain one among: 'full', 'slim', 'minim' "
        return

    com += " --skipScout "
    ###########################################com += " --allowTaskDuplication "
    ###########################################com += " --Debug "
    ###########################################com += " --express "
    if isOfficial: com += " --official --voms atlas:/atlas/perf-flavtag/Role=production "
    
    com += "--useNewCode "
    
    com += "--inDS " + ds + " "
    oDS=""
    if not isOfficial: oDS = "user."+username+"."+ds.replace("/","")+suffix
    else             : oDS ="group.perf-flavtag."+ds.replace("/","")+suffix

    print oDS+"  has length: "+str(len(oDS))
    while len(oDS) > 115 :
        print len(oDS)," too long!!!"
        splODS=oDS.split("_")
        splODS.pop(2)
        oDS="_".join(splODS)
        pass
    print "final: "+oDS+"  has length: "+str(len(oDS))
    com += "--outDS "+ oDS + " "
  
    if "data" in ds:  com += "--nFilesPerJob 2 "
    ###else           :  com += "--nFilesPerJob 5 "
    else           :  com += "--nFilesPerJob 5 " ###--forceStaged "
    ##else           :  com += "--nFilesPerJob 3 "

    com += " --extFile ilumicalc_histograms_None_276262-276954.root,mycool.db,BTagCalibRUN2-08-09_noIP.root "
    com += "--addPoolFC BTagCalibRUN2-08-09_noIP.root "

    #com += "--extFile mycool.db,BTagCalibRUN2Onl-08-05-Run1Special.root "
    #com += "--addPoolFC ,BTagCalibRUN2Onl-08-05-Run1Special.root "

    #com += "--destSE UNI-FREIBURG_SCRATCHDISK "
    #com += "--excludedSite=ANALY_CPPM,ANALY_IN2P3-CC,ANALY_RHUL_SL6 "

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

