#!/bin/bash
MAX_mem=90
memory=`free -m|grep Mem|awk '{print $3}'`
Men=`free | awk '/Mem/ {print int($3/$2*100)}'`
echo "memory: $memory"
echo "Men: $Men"
     if [ $Men -gt $MAX_mem ];then
        echo "Men Warning,memory: $memory ,Men: $Men" 
     fi
