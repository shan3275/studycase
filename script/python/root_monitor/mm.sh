#!/bin/sh
LOCATION="/home/df_user/.mm"
IP1="119.6.239.21"
IP2="119.6.238.51"
while [ True ];
do
    python ${LOCATION}/root_monitor.py -u fw_user -p 7RRz/xO%wHB0.20oj~1@ -i ${IP1}
    python ${LOCATION}/root_monitor.py -u fw_user -p 7RRz/xO%wHB0.20oj~1@ -i ${IP2}
done
