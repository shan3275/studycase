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
import urlparse

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
    hosts_path = 'round-hosts.conf'
    kdicts = {}
    if hosts_path != None:
        hfile = open(hosts_path, "r")
        for LINE in hfile:
            LINE = LINE.strip('\n')
            kdicts[LINE]=0
    else:
        print '没有发现hosts配置文件，退出'
        os._exit()
    for kdict in kdicts:
        print (kdict,kdicts[kdict])

    ###业务开始
    keyword = ('i','s','ua','aid','uid','from','gsid')
    for kw in keyword:
        print(kw)
    
    tmp_dict = OrderedDict()
    tmp_dict['channel']='AGJH-001'
    tmp_dict['platform']='sina'
    tmp_dict['rule']='weibo.cn'
    content_dict = dict()

    log_file = '/root/.log/naga.log' + time.strftime('%Y%m%d')
    print 'log_file:%s' %(log_file)
    logfile = open("access-log","r")
    loglines = follow(logfile)
    for line in loglines:
        print line
        array = line.split('|')
        RUF = array[9]
        if len(array) < 10:
            break;
        print 'len(array):%d' %(len(array))
        print "URL:%s" %(RUF)
        HOST = RUF.split('/',1)[0]
        print "HOST:%s" %(HOST)
        for kdict in kdicts:
            #print kdict
            if uri_compare(kdict, HOST) == True:
                print '\n'
                print 'match!'

                durl = 'http://' + RUF
                r = urlparse.urlsplit(durl)
                path = r.path
                print 'path: %s\n' %(path)
                query = dict(urlparse.parse_qs(r.query))
                #query = urlparse.parse_qs(r.query)
                print query
                print '\n'

                ##组装content
                content_str =''
                for kw in keyword:
                    if query.has_key(kw) :
                        content_str += '%s=%s|' %(kw, query[kw][0])
                    else:
                        content_str += '%s=|'   %(kw)
                if query.has_key('uid') == False and query.has_key('from') == False :
                    break;
                print content_str
                print '\n'
                
                ##base64加密
                encryed_content_str = base64.b64encode(content_str)
                print encryed_content_str
                print '\n'

                content_dict['content'] = encryed_content_str

                tmp_dict['datas'] = [content_dict]
                print tmp_dict

                post_json_str = json.dumps(tmp_dict)
                print post_json_str

                ##发送
                server_url='http://127.0.0.1:5000/log'
                #server_url='http://117.50.19.69/api/pushData'
                print 'server_url:%s' %(server_url)
                r = requests.post(server_url, data=post_json_str)
                print r.status_code
                print r.text

                break
