#!/bin/bash
# Program:
#       This program is used to download data in Wenzhou for FS8000.
# History:
# 2012/09/27         shan      First release

cmd0="restore"
cmd2="rule five-tuple commit"
IP="192.168.1.89"
user="root"
pwd="root"
COUNTER=0
while [  $COUNTER -lt 4 ]; do
        echo The counter is $COUNTER
            let "src=1000*(2**$COUNTER)"
            let COUNTER=COUNTER+1
            cmd1="rule five-tuple add 1 1.1.1.1+$src 2.2.2.2+$src 10000+$src 20000+$src tcp iifgrp 1 action forward oifgrp 1"
            echo $cmd1
            ./fc7000_cmd.py $IP $user $pwd "${cmd0}"
            ./fc7000_cmd.py $IP $user $pwd "${cmd1}"
            ./fc7000_cmd.py $IP $user $pwd "${cmd2}"
        done


