#!/bin/sh
# Program:
#   This program is used to start caputre packet by tshark.
#   这个程序用来启动tshark进行抓包，抓取get报文，可以扩展
#   使用程序前注意修改抓取报文的接口
# History:
# 2016/12/03         shan      First release
output_file_name=/home/shan/tshark/$(date "+%Y%m%d").log
#echo -e $output_file_name
killall -9 tshark
sleep 3
tshark -i eth0 -n -t a -R http.request -T fields -e "frame.time" -e "ip.src" -e "http.host" -e "http.request.method" -e "http.request.uri" >> $output_file_name 
#ls > $output_file_name

