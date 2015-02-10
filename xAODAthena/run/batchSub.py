import os
import commands

### User Configuration starts here ###

# print a lot of output
verbose = False

# batch queue
queue = "1nd"

# input location on eos
#eosDir = "/eos/atlas/atlastier0/tzero/prod/mc14_13TeV/PowhegPythia_P2012_ttbar_nonallhad/110401/mc14_13TeV.110401.PowhegPythia_P2012_ttbar_nonallhad.recon.AOD.e2928_s1982_s2008_r5956_v114" 
eosDir = "/eos/atlas/atlastier0/tzero/prod/mc14_13TeV/Pythia8_AU2MSTW2008LO_zprime1000_tt/110903/mc14_13TeV.110903.Pythia8_AU2MSTW2008LO_zprime1000_tt.recon.AOD.e2743_s1982_s2008_r5956_v114"

# output location on eos
eosHome = "/eos/atlas/user/z/zaidan/btag/"

# number of files per job
nperjob = 20

# max number of jobs
njobs = 10

# submit only a single test job (recommended when code changes)
testJob = False

# don't submit, only print what is being done (recommended before a real submit)
printOnly = False

# job label and version
#jobLabel = "ttbar"
jobLabel = "zprime"
jobVersion = "10022015"

######################################
###  User configuration ends here  ###
######################################

## user name
analyzer = os.environ["USER"]

## set the test area
testArea = os.getenv("TestArea")
if testArea == None :
    raise "Setup not done properly!"
if verbose :
    print "testArea="+testArea

## this is where the code would run locally:
codeArea = testArea+"/xAODAthena/run"

## eos command
eosCom = "/afs/cern.ch/project/eos/installation/0.3.84-aquamarine/bin/eos.select"

## output path on eos
eosOutput = eosHome+"/outputs."+jobLabel+"."+jobVersion
if not printOnly:
    os.system(eosCom+" rm -r "+eosOutput)
    os.system(eosCom+" mkdir "+eosOutput)

## function to submit a single job
def submit_one_job( ijob, inputList) :
    print "submitting job: ",ijob
    if verbose :
        print "submit_one_job ( ",ijob," , ",inputList," )"

    jobName = "job."+jobLabel+"."+str(ijob)

    # create run directory
    rundir = codeArea+"/outputs."+jobLabel+"."+jobVersion+"/"+jobName
    jobOptions = rundir+"/jobOptions.py"
    output = "root://eosatlas/"+eosOutput+"/flavntuple."+jobName+".root"
    if not printOnly:
        os.system("rm -rf "+rundir)
        os.system("mkdir -p "+rundir)    
        os.system("cat "+codeArea+"/jobOptions_Tag.py | "+
                  "sed \'s|\"AOD.pool.root\"|"+inputList+"|g\' | "+
                  "sed \'s|flavntuple.root|/tmp/"+analyzer+"/flavntuple.root|g\' "+
                  " > "+jobOptions+" \n")

    # create wrapper run script
    runscript = rundir+"/run.sh"
    if not printOnly:
        f = open( runscript, "w" )
        f.write("#!/bin/bash\n")
        f.write("cd "+testArea+"\n")
        f.write("source setup.sh\n")
        f.write("set -e\n")
        f.write("cd "+rundir+"\n")
        f.write("athena "+jobOptions+" \n")
        f.write("xrdcp /tmp/"+analyzer+"/flavntuple.root "+output+"\n")
        f.close()
        os.system("chmod u+x "+runscript)
        
    # submit command
    com = ( "bsub -N "+
            "-J "+jobName+" "+
            "-q "+queue+" "+
            "-eo "+rundir+"/stderr.txt "+
            "-oo "+rundir+"/stdout.txt "+
            runscript )
    print com
    if not printOnly:
        os.system(com)

## make a list of input files
fileList = commands.getstatusoutput(eosCom+" ls "+eosDir)[1].rsplit("\n")

## loop on file list and submit jobs
filesSoFar = 0
inputList = ""
ijob = 0
for fname in fileList :
    inputFile = "\"root://eosatlas/"+eosDir+"/"+fname+"\""
    if verbose :
        print inputFile

    filesSoFar = filesSoFar + 1

    if inputList == "" :
        inputList = inputFile
    else :
        inputList = inputList+","+inputFile

    if filesSoFar == nperjob :

        submit_one_job ( ijob, inputList)
        inputList = ""
        filesSoFar = 0
        ijob = ijob + 1

        if ijob >= njobs :
            break

        if testJob :
            break

if filesSoFar > 0 :
    submit_one_job ( ijob, inputList)

