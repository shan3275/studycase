#!/bin/bash
IP=`ifconfig | grep 'inet ' | grep -v '127.0.0.1' | awk -F ' ' '{print $2}'| awk -F':' '{print $2}'| head -n 1`
MAX_mem=95
MAX_swap=20
memory=`free -m|grep Mem|awk '{print $3}'`
Men=`free | awk '/Mem/ {print int($3/$2*100)}'`
Mem=`free -m | awk '/-/ {print $3}'`
SWAP=`free | awk '/Swap/ {print int($3/$2*100)}'`
     if [ $Men -gt $MAX_mem -o $SWAP -gt $MAX_swap ];then
        echo "Men Warning,memory: $memory ,Mem: $Mem" | mutt -s "$IP Men Warning " rocdk@163.com
     fi