basedir=/afs/cern.ch/work/l/lmcclymo/public/ #Directory name of interest

funcname=xAODValerio # New function name for running over this directory

dirs=`ls $basedir`

outfile1=Chainingvalerio.C
outfile2=Chainingvalerio.h

#Test Function - just the first file

echo -n 'TChain* ' >> $outfile1
echo -n $funcname >> $outfile1
echo $'Test(){ \n' >> $outfile1

echo $'chain = new TChain("bTag"); \n'>> $outfile1

echo -n 'chain->AddFile("' >> $outfile1
echo -n $basedir/$dir/$file >> $outfile1
echo '");' >> $outfile1

echo 'return chain;' >> $outfile1
echo $'} \n' >> $outfile1

#Full List Function

echo -n 'TChain* ' >> $outfile1
echo -n $funcname >> $outfile1
echo $'(){ \n' >> $outfile1

echo $'chain = new TChain("bTag"); \n'>> $outfile1

for dir in $dirs
do
  echo Checking $dir
  files=`ls $basedir/$dir`
  for file in $files
  do
    echo -n 'chain->AddFile("' >> $outfile1
    echo -n $basedir/$dir/$file >> $outfile1
    echo '");' >> $outfile1
  done
done

echo 'return chain;' >> $outfile1
echo '}' >> $outfile1 

echo -n 'TChain* ' >> $outfile2
echo -n $funcname >> $outfile2
echo '()' >> $outfile2

echo -n 'TChain* ' >> $outfile2
echo -n $funcname >> $outfile2
echo 'Test()' >> $outfile2
