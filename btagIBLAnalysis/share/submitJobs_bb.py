import os
import sys

suffix     = ".bbVRT5"
inputFile  = "mc_samples_bb.txt"

dsList = open(inputFile,'r')
lines = dsList.readlines()

def submitJob(ds) :
    com = "pathena jobOptions_Tag_bb.py"
    com += " --skipScout "
    com += " --official --voms=atlas:/atlas/phys-exotics/Role=production " # change or get rid of this line depending on your production role
    com += "--useNewCode "
    com += "--inDS " + ds + " "
    oDS = ""
    oDS = ds.replace("/", "") + suffix

    print oDS + " has length: " + str(len(oDS))
    while len(oDS) > 115 :
        print len(oDS), " too long!!!"
        splODS = oDS.split("_")
        splODS.pop(2)
        oDS = "_".join(splODS)
        pass
    print "final: " + oDS + " has length: " + str(len(oDS))

    com += "--outDS " + oDS + " "
    com += "--nFilesPerJob 1 "
    com += " --extFile ilumicalc_histograms_None_276262-276954.root,mycool.db,BTagCalibRUN2-08-09_noIP.root "
    com += "--addPoolFC BTagCalibRUN2-08-09_noIP.root "

    return com

for ds in lines :
    if "#" in ds : continue
    if "mc" not in ds and "valid" not in ds and "group" not in ds and "data" not in ds: continue

    print " "
    print "///////////////////////////////////////////////////////////////////////////////////////"
    print ds
    command = submitJob(ds.replace("\n",""))
    print command
    os.system(command)
    pass
