#!/usr/bin/python
# coding:utf-8
#Filename:tail.py
# 用途：将日志进行域名匹配之后，加密发送到指定的服务器
#
import time
import json
from collections import OrderedDict
import sys
import re
import base64
import requests

def follow(thefile):
    thefile.seek(0,2)
    while True:
        line = thefile.readline()
        if not line:
            time.sleep(0.1)
            continue
        yield line

def field_compare(pstr, vstr):
    #print  "pstr=%s, vstr=%s" %(pstr, vstr)
    result = re.search(pstr, vstr)
    if result:
        return True
    return False

def uri_compare(pstr, vstr):
    uri = vstr
    npos = vstr.find('?')
    if npos > 0:
        uri = vstr[0:npos]
    return field_compare(pstr, uri)

if __name__ == '__main__':
    #读取hosts域名列表
    hosts_path = 'hosts.conf'
    kdicts = []
    if hosts_path != None:
        hfile = open('hosts.conf', "r")
        for LINE in hfile:
            LINE = LINE.strip('\n')
            kdicts.append(LINE)
    else:
        print '没有发现hosts配置文件，退出'
        os._exit()
    for kdict in kdicts:
        print kdict

    ###业务开始
    tmp_dict = OrderedDict()
    logfile = open("access-log","r")
    loglines = follow(logfile)
    for line in loglines:
        print line
        array = line.split('|')
        RUF = array[9]
        for kdict in kdicts:
            #print kdict
            if uri_compare(kdict, RUF) == True:
                print 'match!'
                tmp_dict['time']=array[0]
                tmp_dict['srcIp']=array[3]
                tmp_dict['Url']=array[9]
                json_str = json.dumps(tmp_dict)
                print json_str
                ##发送加密内容
                encryed_str = base64.b64encode(json_str)
                print encryed_str
                tmp_dict.clear()
                ##加密host
                encryed_host_str = base64.b64encode(kdict)
                print encryed_host_str

                ##发送
                server_url='http://127.0.0.1:5000/log?data=%s' %(encryed_host_str)
                print 'server_url:%s' %(server_url)
                r = requests.post(server_url, data=encryed_str)
                print r.status_code
                print r.text

                break
