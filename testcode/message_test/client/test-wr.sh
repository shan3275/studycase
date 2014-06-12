#!/bin/sh
#whilecount 
declare -i COUNTER
COUNTER=$1 
#does the counter > 0
while [ $COUNTER -gt 0 ]
do
	COUNTER=$COUNTER-1
	./ssctest test -W -a 23 -v `date |awk '{ print $4 }'|cut -d: -f3`
	sleep 1
done

