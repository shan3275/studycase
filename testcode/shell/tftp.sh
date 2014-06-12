#!/bin/sh
# Program:
#       This program is used to download data in Wenzhou for FS8000.
# History:
# 2012/09/27         shan      First release
COUNTER=0
while [ true ]
do
	COUNTER=`expr $COUNTER + 1`
	echo Loop $COUNTER
	tftp -g -r FlowOS.elf 192.168.2.34
	ls -lh FlowOS.elf
done

