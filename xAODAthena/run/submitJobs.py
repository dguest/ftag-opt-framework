import os
import sys

dsList=open("mc_samples.txt",'r')
lines=dsList.readlines()

suffix=".BTAGNTUP_V1"

def submitJob(ds) :
    com = "pathena  jobOptions.py "
    com += "--inDS " + ds + " "

    oDS = "user.vdao."+ds.replace("/","")+suffix
    print oDS+"  has length: "+str(len(oDS))
    while len(oDS) > 115 :
        print len(oDS)," too long!!!"
        splODS=oDS.split("_")
        splODS.pop(2)
        oDS="_".join(splODS)
        pass
    #oDS=oDS.replace("_AUET2B_MSTW2008LO_","")
    #oDS=oDS.replace("ProtosPythia","PP")
    #oDS=oDS.replace("group.perf-idtracking","perf-id")
    print "final: "+oDS+"  has length: "+str(len(oDS))
    com += "--outDS "+ oDS + " "
  
    com += "--nFilesPerJob 3 "
    com += "--destSE UNI-FREIBURG_SCRATCHDISK "
    return com

for ds in lines :
    if "#" in ds : continue
    if "mc" not in ds: continue

    print " "
    print "///////////////////////////////////////////////////////////////////////////////////////"
    print ds
    command=submitJob(ds.replace("\n",""))
    print command
    os.system(command)
    pass
