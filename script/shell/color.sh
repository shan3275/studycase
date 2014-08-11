#!/bin/sh 
############################################################ 

# Nico Golde Homepage: http://www.ngolde.de 

# Last change: Mon Feb 16 16:24:41 CET 2004 

############################################################ 
for attr in 0 1 4 5 7 ; do 
echo "----------------------------------------------------------------" 
printf "ESC[%s;Foreground;Background - " $attr 
for fore in 30 31 32 33 34 35 36 37; do 
for back in 40 41 42 43 44 45 46 47; do 
printf '3[%s;%s;%sm %02s;%02s ' $attr $fore $back $fore $back 
done 
printf ' ' 
done 
printf '3[0m' 
done 
