#!/bin/sh
# Program:
#       This program is used to download data in Wenzhou for FS8000.
# History:
# 2012/09/27         shan      First release
SUBCARD='up'
for subcard in $SUBCARD
do
        FPGA='0 1 2 3 4 5 6 7'
        for fpga in $FPGA
        do
                REG=0
                while [ $REG -lt 250 ]
                do      
                #echo dtweak $fpga cnt $subcard $REG 32
		echo -e "@10.${fpga}.0.${REG} 20.0.2.3 30:30 50:50" >> set2.txt
		REG=`expr $REG + 1`
                done         
        done
done
