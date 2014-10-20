# echo "setup btagIBLAnalysis btagIBLAnalysis-00-00-00 in /afs/cern.ch/user/m/marx/btag/xAODAthena"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/cern.ch/sw/contrib/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtbtagIBLAnalysistempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtbtagIBLAnalysistempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/m/marx/btag/xAODAthena  -no_cleanup $* >${cmtbtagIBLAnalysistempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/m/marx/btag/xAODAthena  -no_cleanup $* >${cmtbtagIBLAnalysistempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtbtagIBLAnalysistempfile}
  unset cmtbtagIBLAnalysistempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtbtagIBLAnalysistempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtbtagIBLAnalysistempfile}
unset cmtbtagIBLAnalysistempfile
exit $cmtsetupstatus

