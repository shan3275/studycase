#!/bin/sh
para1=$(pwd |cut -c41-45)
if [ -n "$para1" ]; then
	echo "NOT NULL"
else
	echo "IS NULL"
fi 
