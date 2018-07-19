#!/bin/sh
# Program:
#   This program is used to start caputre packet by tshark.
#   这个程序用来启动tshark进行抓包，抓取get报文，可以扩展
#   使用程序前注意修改抓取报文的接口
# History:
# 2016/12/03         shan      First release
procID=`pgrep round_v2.py`
if [ "" == "$procID" ];
then
	cd /usr/local/.wb
	nohup ./round_v2.py >> round.log 2>&1 &
fi

procID=`pgrep upload.py`
if [ "" == "$procID" ];
then
	cd /usr/local/.wb
	nohup ./upload.py -i /usr/local/.log/  -o /usr/local/.log/  >> upload.log 2>&1 &
fi
