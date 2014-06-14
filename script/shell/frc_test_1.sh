#!/bin/sh
# Program:
#       This program is used to download data in Wenzhou for FS8000.
# History:
# 2012/09/27         shan      First release

REG=0
while [ $REG -lt 256 ]
do      
echo "\"vlan_id_$REG	  	  \"", #>> test.log        
#echo dtweak $fpga cnt $subcard $REG 32
REG=`expr $REG + 1`        
done         
