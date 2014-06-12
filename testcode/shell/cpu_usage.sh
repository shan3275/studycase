
#!/bin/bash 
while(true) 
do 
	CPU_1=$(cat /proc/stat | grep 'cpu ' | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}') 
	SYS_IDLE_1=$(echo $CPU_1 | awk '{print $4}') 
	Total01=$(echo $CPU_1 | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}') 
	sleep 2 
	CPU_2=$(cat /proc/stat | grep 'cpu ' | awk '{print $2" "$3" "$4" "$5" "$6" "$7" "$8}') 
	SYS_IDLE_2=$(echo $CPU_2 | awk '{print $4}') 
	Total_2=$(echo $CPU_2 | awk '{printf "%.f",$1+$2+$3+$4+$5+$6+$7}') 
	SYS_IDLE=`expr $SYS_IDLE_2 - $SYS_IDLE_1` 
	Total=`expr $Total_2 - $Total01` 
	TT=`expr $SYS_IDLE \* 100` 
	SYS_USAGE=`expr $TT / $Total` 
	SYS_Rate=`expr 100 - $SYS_USAGE` 
	echo "The CPU Rate : $SYS_Rate%" 
	echo "------------------" 
done
