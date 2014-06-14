#!/bin/sh
#whilecount 
declare -i COUNTER
COUNTER=$1 
#does the counter > 0
while [ $COUNTER -gt 0 ]
do
	date
	COUNTER=$COUNTER-1
	echo $COUNTER
	sleep 1
done

