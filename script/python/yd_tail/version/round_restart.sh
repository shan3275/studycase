#!/bin/sh
# Program:
#   This program is used to start caputre packet by tshark.
#   这个程序用来启动tshark进行抓包，抓取get报文，可以扩展
#   使用程序前注意修改抓取报文的接口
# History:
# 2016/12/03         shan      First release
killall -9 round.py
sleep 3
cd /usr/local/.wb
./round.py  &
