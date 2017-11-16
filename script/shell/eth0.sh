#!/bin/sh
while [ 1 ]
do
        a=$(ifconfig em1 | grep 'RX pac' | awk '{print $2}' | awk -F: '{print $NF}')
        echo -ne "$a\r" #不换行刷新数据
done
