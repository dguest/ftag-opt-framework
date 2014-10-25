# echo "setup btagIBLAnalysis btagIBLAnalysis-00-00-00 in /afs/cern.ch/user/v/vdao/xAODDumper/xAODAthena"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/atlas.cern.ch/repo/sw/software/AthAnalysisBase/x86_64-slc6-gcc47-opt/2.0.14/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtbtagIBLAnalysistempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtbtagIBLAnalysistempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/v/vdao/xAODDumper/xAODAthena  -no_cleanup $* >${cmtbtagIBLAnalysistempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=btagIBLAnalysis -version=btagIBLAnalysis-00-00-00 -path=/afs/cern.ch/user/v/vdao/xAODDumper/xAODAthena  -no_cleanup $* >${cmtbtagIBLAnalysistempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtbtagIBLAnalysistempfile}
  unset cmtbtagIBLAnalysistempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtbtagIBLAnalysistempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtbtagIBLAnalysistempfile}
unset cmtbtagIBLAnalysistempfile
return $cmtsetupstatus

