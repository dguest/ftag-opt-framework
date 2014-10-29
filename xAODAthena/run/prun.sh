#!/bin/bash

inputs=mc_samples.txt

cat $inputs | while read inDataSet
  do
 
  if [[ "${inDataSet}" == "#"* ]];
      then
      #echo skipping ${inDataSet}
      continue
  fi
  outDataSet=user.marx.test.flavntuplev9



  echo -e "\n \n"
  echo dataset= $inDataSet
  echo out=$outDataSet
 

##group version version
  prun --exec "echo %IN | sed 's/,/\n/g' > input.txt; athena jobOptions.py" --athenaTag=AtlasProduction,19.1.4.1 --noBuild --inDS ${inDataSet} --outDS ${outDataSet} --nFilesPerJob 1
#--excludedSite ANALY_BHAM_SL6
#--site BNL-OSG2_DATADISK



done





