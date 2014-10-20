# echo "cleanup btagIBLAnalysis btagIBLAnalysis-00-00-00 in /afs/cern.ch/user/m/marx/btag/xAODAthena"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/cern.ch/sw/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtbtagIBLAnalysistempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtbtagIBLAnalysistempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/m/marx/btag/xAODAthena  $* >${cmtbtagIBLAnalysistempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/m/marx/btag/xAODAthena  $* >${cmtbtagIBLAnalysistempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtbtagIBLAnalysistempfile}
  unset cmtbtagIBLAnalysistempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtbtagIBLAnalysistempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtbtagIBLAnalysistempfile}
unset cmtbtagIBLAnalysistempfile
return $cmtcleanupstatus

