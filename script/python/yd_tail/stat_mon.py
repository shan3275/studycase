#!/usr/bin/python
# coding:utf-8
#Filename:tail.py
# 用途：将naga统计进行监控，出现失败后邮件告警
#
import time
import json
from collections import OrderedDict
import sys
import re
import base64
import requests
import urlparse
import os
import psutil # pip install psutil

def follow(thefile):
    thefile.seek(0,2)
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        yield line

def send_simple_message(message):
    return requests.post(
            "https://api.mailgun.net/v3/mg.oncompress.com/messages",
            auth=("api", "key-6c77894e772490822d55b4e4b6651894"),
            data={"from": "Admin <postmaster@mg.oncompress.com>",
                "to": ["shan275@qq.com"],
                "subject": "日志监控报警",
                "text": message})


if __name__ == '__main__':
    ##过滤关键字
    keyword = 'ACL_URLPUSH_TX_SUCCESS'

    value_file = 'value.txt'
    value = 0
    last_value = 0

    ##打开日志文件
    #log_file = 'stat-log' + time.strftime('%Y%m%d')
    log_file = 'stat-log'
    print 'log_file:%s' %(log_file)
    logfile = open(log_file,"r")
    loglines = follow(logfile)
    for line in loglines:
        #print line
        array = line.split()
        NAME  = array[0]
        VALUE = array[-1]
        #print 'NAME  : %-40s VALUE :%s' %(NAME, VALUE)
        if keyword in NAME:
            value = int(VALUE)
            print 'value:%d' %(value)
            print 'NAME  : %-40s VALUE :%s' %(NAME, VALUE)
            ##读取上一次统计
            if os.path.exists(value_file) == True:
                fo = open(value_file, "r")
                last_value_str = fo.read()
                fo.close()
                last_value = int(last_value_str)
                if value > last_value :
                    print 'last_value:%d value:%d' %(last_value, value)
                    alert_str = '%s 发送失败告警,last_value:%d, now_value' %(time.strftime('%Y-%m-%d %H:%M:%S'), last_value, value)
                    print alert_str
                    send_simple_message(alert_str)
            if value != last_value:
                fo = open(value_file, "w")
                fo.write(VALUE)
                fo.close()
