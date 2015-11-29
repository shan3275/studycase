#!/bin/sh

warninglimit=80000000
lowlimit=50000000

filesystems="/ /home "

for fs in $filesystems
do
        size=`df -k $fs|grep $fs|awk '{ print $3; }' |grep [0-9] `
        if [ $size -le $lowlimit ]
        then
		date > /root/cron/info.log
                echo "URGENT: Low disk space for $fs ($size)" >> /root/cron/info.log
		df -lh >> //root/cron/info.log
		mutt -s "Warining" shan275@163.com  < /root/cron/info.log
                break
        fi
        if [ $size -le $warninglimit ]
        then
		date > /root/cron/info.log
                echo "URGENT: Low disk space for $fs ($size)" >> /root/cron/info.log
		df -lh >> /root/cron/info.log
		mutt -s "Warining" shan275@163.com  < /root/cron/info.log
fi
done
