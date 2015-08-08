#!/bin/sh
echo -n "请输入需要查询的网卡的接口:"
read eth
echo "你要查询的网卡接口为"$eth
byte=1024
while [ true ]
do
    infirst=$(awk '/'$eth'/{print $1 }' /proc/net/dev |sed 's/'$eth'://')
    outfirst=$(awk '/'$eth'/{print $10 }' /proc/net/dev)
    sumfirst=$(($infirst+$outfirst))
    sleep 1
    inend=$(awk '/'$eth'/{print $1 }' /proc/net/dev |sed 's/'$eth'://')
    outend=$(awk '/'$eth'/{print $10 }' /proc/net/dev)
    sumend=$(($inend+$outend))
    sum=$(($sumend-$sumfirst))
    sum=`expr $sum / $byte / $byte`
    echo "平均流量为:"$sum"Mbytes/sec"
done
