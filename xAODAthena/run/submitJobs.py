import os
import sys

##suffix=".BTAGNTUP_OrigV6slim"    ### improving jet calibration
##suffix=".BTAGNTUP_OrigV6full"
##suffix=".BTAGNTUP_OrigV7full"    ### latest jet calibration
##suffix=".BTAGNTUP_V7retag"       ### latest jet calibration+retag
##suffix=".BTAGNTUP_V8slim"        ### correct AODfix
##suffix=".BTAGNTUP_V8full"        ### correct AODfix
##suffix=".BTAGNTUP_V9slim"        ### specific fix to run over FTAG
##suffix=".BTAGNTUP_V9full"        ### specific fix to run over FTAG + trigSelection + grl in data
##suffix=".BTAGNTUP_V10full"       ### finally fixing JVT
##suffix=".BTAGNTUP_V11slim"       ### (put 3 jets collections)
##suffix=".BTAGNTUP_V13slim"       ### (new jet cleaning, 2 jet collections)
##suffix=".BTAGNTUP_V14slim"       ### new trigger logic and cuts (eta)
##suffix=".BTAGNTUP_V15full"       ### only HLT single jet triggers 
##suffix=".BTAGNTUP_V16full"       ### same as 15 but fixing my stupidity with PUtool 

##suffix=".BTAGNTUP_V19full"       ### from 16 but updating tag of JetCalibration and implementing PUtool [also first test of split output]
##suffix=".BTAGNTUP_V20full"       ### fix in d02d var + latest calibration. I AM STUPID AND THIS IS INDEED SLIM!!!!!
#suffix=".BTAGNTUP_V21full"        ### same as V20 but this time really full
#suffix=".BTAGNTUP_V22tmp"         ### same as V21 but no GRL and no mu recalculation

#suffix=".BTAGNTUP_V23"            ### same as V21 but no GRL and no mu recalculation

suffix=".BTAGNTUP_VtestIPxDRef"       ### IPxD tests


###########################################################################################################################
###########################################################################################################################
###########################################################################################################################
isOfficial=False
username="vdao"


###########################################################################################################################
###########################################################################################################################
###########################################################################################################################
dsList=open("mc_samples.txt",'r')
lines=dsList.readlines()

def submitJob(ds) :
    com = ""
    if "data" in ds: com = "pathena jobOptions_Tag_data.py "
    else           : com = "pathena jobOptions_Tag.py "
    
    com += " --skipScout "
    ###########################################com += " --allowTaskDuplication "
    ##com += " --Debug "
    ##com += "--express "
    if isOfficial: com += " --official --voms atlas:/atlas/perf-flavtag/Role=production "
    
    
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
    else           :  com += "--nFilesPerJob 5 "

    com += " --extFile ilumicalc_histograms_None_276262-276954.root "
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

