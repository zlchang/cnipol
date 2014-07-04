#!/bin/bash
#
# >run_alpha.sh <runlist>
#
# <runlist> is a file with a list of alpha runs. No .data suffix

source $CNIPOL_DIR/script/bgx_asym.sh

FILE_MEASLIST=$1

exec < $FILE_MEASLIST

while read meas_id
do
   arr=($meas_id)
   #echo ${arr[0]} --- ${arr[1]} ---
   if [ -z "${arr[1]}" ]; then
      #echo "polid NOT defined"
      #bgx_limit 10 $CNIPOL_DIR/build/asym --alpha --use-db -g --copy -r $meas_id --no-update-db
      bgx_limit 10 $CNIPOL_DIR/build/asym --alpha --use-db -g --copy -r $meas_id
   else
      #echo "polid defined" ${arr[1]}
      bgx_limit 10 $CNIPOL_DIR/build/asym --alpha --use-db -g --copy --pol-id=${arr[1]} -r ${arr[0]} --no-update-db
      #bgx_limit 10 $CNIPOL_DIR/build/asym --alpha --use-db -g --copy --alpha-sources=${arr[1]} -r ${arr[0]}
      #bgx_limit 10 $CNIPOL_DIR/build/asym --alpha --alpha-sources=${arr[1]} -r ${arr[0]}
   fi
done

# Wait until all processes are finished
bgx_wait
