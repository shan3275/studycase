#!/bin/sh
#whilecount 
declare -i COUNTER
COUNTER=$1 
#does the counter > 0
while [ $COUNTER -gt 0 ]
do
        COUNTER=$COUNTER-1
	./ssctest test -R -a 23 
	sleep 1
done

