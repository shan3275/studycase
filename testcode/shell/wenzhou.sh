#!/bin/sh
# Program:
#       This program is used to download data in Wenzhou for FS8000.
# History:
# 2012/09/27         shan      First release

SUBCARD='up dn'
for subcard in $SUBCARD
do
        FPGA='im pp ep'
        for fpga in $FPGA
        do
                REG=0
                while [ $REG -lt 576 ]
                do      
                dtweak $fpga cnt $subcard $REG 32        
                #echo dtweak $fpga cnt $subcard $REG 32
                REG=`expr $REG + 32`        
                done         
        done
done

echo -e "sleep 5"
sleep 5 

SUBCARD='up dn'
for subcard in $SUBCARD
do
        FPGA='im pp ep'
        for fpga in $FPGA
        do
                REG=0
                while [ $REG -lt 576 ]
                do      
                dtweak $fpga cnt $subcard $REG 32        
                #echo dtwek $fpga cnt $subcard $REG 32        
                REG=`expr $REG + 32`        
                done         
        done
done


SUBCARD='up dn'
for subcard in $SUBCARD
do
        dtweak cs1999 print $subcard dev cfg
        dtweak cs1999 print $subcard dev stats
        dtweak cs1999 print $subcard dev status
        dtweak cs1999 print $subcard dev reg  
        dtweak cs1999 print $subcard port cfg
        dtweak cs1999 print $subcard port stats
        dtweak cs1999 print $subcard port status
done


echo -e "sleep 5"
sleep 5 
echo "Read Framer again"

SUBCARD='up dn'
for subcard in $SUBCARD
do
        dtweak cs1999 print $subcard dev cfg
        dtweak cs1999 print $subcard dev stats
        dtweak cs1999 print $subcard dev status
        dtweak cs1999 print $subcard dev reg 
        dtweak cs1999 print $subcard port cfg
        dtweak cs1999 print $subcard port stats
        dtweak cs1999 print $subcard port status
done


echo -e "Read Transponder"

echo -e "Read up Transponder"
dtweak transponder -m 0 -5 
dtweak transponder -m 0 -7
dtweak msa -m 0 -a 40 -c 81 -l 0 -r 2

echo -e "Read dn Transponder"
dtweak transponder -m 1 -5 
dtweak transponder -m 1 -7
dtweak msa -m 1 -a 40 -c 81 -l 0 -r 2

echo -e "check transponder"
transponder_stat=$(dtweak msa -m 0 -a 40 -c 81 -l 0 -r 2 |cut -c68-69)
echo "up transponder stat $transponder_stat"
case $transponder_stat in
    ff)
        echo -e "up transponder ok"
        ;;
    *)
        echo "up transponder $transponder_stat"
        echo "reset up transponder"
        dtweak cpld reg 0 0x9 feff
        sleep 1
        dtweak cpld reg 0 0x9 ffff
        ;;
esac

transponder_stat=$(dtweak msa -m 1 -a 40 -c 81 -l 0 -r 2 |cut -c68-69)
echo "dn transponder stat $transponder_stat"
case $transponder_stat in
    ff)
        echo -e "dn transponder ok"
        ;;
    *)
        echo "dn transponder $transponder_stat"
        echo "reset dn transponder"
	dtweak cpld reg 0 0xa 1f7
	sleep 1	
	dtweak cpld reg 0 0xa 1ff
        ;;
esac

##open framer switch
echo "reopen framer loop switch"
dtweak cs1999 pktsize dn 1541 16
dtweak cs1999 learn up enable   
dtweak cs1999 learn dn enable

## rest imm
echo "reset im"
dtweak im up 0x10020 0x5a
sleep 1
dtweak im up 0x90048 0x0
dtweak im dn 0x10020 0x5a
sleep 1
dtweak im dn 0x90048 0x0

echo -e "DONE!"
