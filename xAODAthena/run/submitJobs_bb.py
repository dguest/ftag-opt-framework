import os
import sys

suffix    =".2016bbV1"
isOfficial=False
username  ="nwhallon"
inputFile ="mc_samples_bb.txt"

dsList=open(inputFile,'r')
lines=dsList.readlines()

def submitJob(ds) :
    com = "pathena jobOptions_Tag_bb.py" # nikola

    com += " --skipScout "

    if isOfficial: com += " --official --voms=atlas:/atlas/phys-exotics/Role=production "
    
    com += "--useNewCode "
    
    com += "--inDS " + ds + " "
    oDS=""
    if not isOfficial: oDS = "user."+username+"."+ds.replace("/","")+suffix
    else             : oDS = "group.phys-exotics."+ds.replace("/","")+suffix

    print oDS+" has length: "+str(len(oDS))
    while len(oDS) > 115 :
        print len(oDS)," too long!!!"
        splODS=oDS.split("_")
        splODS.pop(2)
        oDS="_".join(splODS)
        pass
    print "final: "+oDS+" has length: "+str(len(oDS))
    com += "--outDS "+ oDS + " "
  
    if "data" in ds:  com += "--nFilesPerJob 1 "
    else           :  com += "--nFilesPerJob 1 "

    com += " --extFile ilumicalc_histograms_None_276262-276954.root,mycool.db,BTagCalibRUN2-08-09_noIP.root "
    com += "--addPoolFC BTagCalibRUN2-08-09_noIP.root "

    #com += "--extFile mycool.db,BTagCalibRUN2Onl-08-05-Run1Special.root "
    #com += "--addPoolFC ,BTagCalibRUN2Onl-08-05-Run1Special.root "

    #com += "--destSE UNI-FREIBURG_SCRATCHDISK "
    #com += "--excludedSite=ANALY_CPPM,ANALY_IN2P3-CC,ANALY_RHUL_SL6 "

    return com

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
