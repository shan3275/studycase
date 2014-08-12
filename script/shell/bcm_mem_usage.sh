#!/bin/bash 

PID=$1
VmSizeOld=$(cat /proc/${PID}/status | grep 'VmSize:' | awk '{print $2}') 
echo `date "+%Y/%m/%d %H:%M:%S" ` VmSize: ${VmSizeOld}KB 
while(true) 
do 
	VmSizeNew=$(cat /proc/${PID}/status | grep 'VmSize:' | awk '{print $2}') 
	Add=`expr $VmSizeNew - $VmSizeOld`
	if [ $Add -ne 0 ];then
		echo `date "+%Y/%m/%d %H:%M:%S" ` VmSize: ${VmSizeNew}KB +${Add}KB
	fi
	VmSizeOld=$VmSizeNew
	sleep 1
done
