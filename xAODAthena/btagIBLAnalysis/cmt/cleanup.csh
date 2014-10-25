# echo "cleanup btagIBLAnalysis btagIBLAnalysis-00-00-00 in /afs/cern.ch/user/v/vdao/xAODDumper/xAODAthena"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/atlas.cern.ch/repo/sw/software/AthAnalysisBase/x86_64-slc6-gcc47-opt/2.0.14/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtbtagIBLAnalysistempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtbtagIBLAnalysistempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/v/vdao/xAODDumper/xAODAthena  $* >${cmtbtagIBLAnalysistempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/v/vdao/xAODDumper/xAODAthena  $* >${cmtbtagIBLAnalysistempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtbtagIBLAnalysistempfile}
  unset cmtbtagIBLAnalysistempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtbtagIBLAnalysistempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtbtagIBLAnalysistempfile}
unset cmtbtagIBLAnalysistempfile
exit $cmtcleanupstatus

