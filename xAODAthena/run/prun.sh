#!/bin/bash

inputs=mc_samples.txt

cat $inputs | while read inDataSet
  do
 
  if [[ "${inDataSet}" == "#"* ]];
      then
      #echo skipping ${inDataSet}
      continue
  fi
  outDataSet=user.vdao.flavntuplev4



  echo -e "\n \n"
  echo dataset= $inDataSet
  echo out=$outDataSet
 

##group version version
##  prun --exec "echo %IN | sed 's/,/\n/g' > input.txt; athena jobOptions.py" --athenaTag=AthAnalysisBase,2.0.15 --noBuild --inDS ${inDataSet} --outDS ${outDataSet} --nFilesPerJob 5
#--excludedSite ANALY_BHAM_SL6
#--site BNL-OSG2_DATADISK

  pathena  jobOptions.py  --inDS ${inDataSet} --outDS ${outDataSet} --nFilesPerJob 3

done





