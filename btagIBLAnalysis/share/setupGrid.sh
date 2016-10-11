localSetupPandaClient

#localSetupDQ2Client

# check for a valid proxy
echo "voms-proxy-info"
voms-proxy-info

# create a proxy if one does not exist
if [[ $? != 0 ]]; then
  good_proxy=0

  while [[ $good_proxy == 0 ]]; do
    echo "voms-proxy-init -voms atlas -valid 90:00"
    ###voms-proxy-init -voms atlas:/phys-higgs/Role=production  -valid 90:00
    voms-proxy-init -voms atlas:/atlas/perf-flavtag/Role=production -valid 90:00
    if [[ $? == 0 ]]; then
      good_proxy=1
    fi
  done
fi
