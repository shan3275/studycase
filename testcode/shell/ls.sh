#!/bin/sh
for ((COUNTER=0; COUNTER<10; ++COUNTER))
do
    echo The counter is $COUNTER
done    
PATH=`ls`
echo $PATH
for i in $PATH;
do
	echo $i
done
