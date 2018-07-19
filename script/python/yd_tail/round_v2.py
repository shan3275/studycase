#!/usr/bin/python
# coding:utf-8
#Filename:tail.py
# 用途：将日志进行域名匹配之后，保存日志文件,一个小时一个文件
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
    keyword      = ('i','s','ua','aid','uid','from','gsid')
    keyword_main = ('i','s','ua','aid','uid','gsid')
    for kw in keyword:
        print(kw)

    content_dict = dict()

    filepath = '/Users/liudeshan/work/studycase/script/python/yd_tail/'
    log_file = filepath + 'naga.log' + time.strftime('%Y%m%d')
    print 'log_file:%s' %(log_file)
    logfile = open(log_file, "r")
    loglines = follow(logfile)
    for line in loglines:
        #print line
        array = line.split('|')
	if len(array) < 10:
            continue
        RUF = array[9]
        #print "URL:%s" %(RUF)
        HOST = RUF.split('/',1)[0]
        #print "HOST:%s" %(HOST)
        for kdict in kdicts:
            #print kdict
            if uri_compare(kdict, HOST) == True:
                durl = 'http://' + RUF
                r = urlparse.urlsplit(durl)
		#print 'host: %s' %(r.netloc)
                path = r.path
                #print 'path: %s\n' %(path)
                query = dict(urlparse.parse_qs(r.query))
                #query = urlparse.parse_qs(r.query)
                #print query
                #print '\n'

                ##组装content
	        if query.has_key('i') == False or query.has_key('s') == False or query.has_key('ua') == False or query.has_key('aid')==False or query.has_key('gsid')==False or query.has_key('uid')==False :
		    print '字段不全'
	            break;
	        if query['i'][0] == False or query['s'][0] == False or query['ua'][0] == False or query['aid'][0]==False or query['gsid'][0]==False or query['uid'][0]==False :
		    print '字段有零'
		    break;
                print 'not null, not 0'
                if len(query['uid'][0]) != 10 :
		    print 'uid长度不为10'
                    break
                print 'uid len = 10'
                if query['uid'][0].isdigit() == False :
		    print 'uid不全是数值'
                    break
                print 'uid is number '
                #if len(query['aid'][0]) <= 50 :
                #    break
                #print 'aid is > 50 bytes'
                print '\n'
                print 'match!'
		kdicts[kdict] = kdicts[kdict] + 1
		print '%s :%d' %(kdict, kdicts[kdict])
                content_str =''
                for kw in keyword:
                    if query.has_key(kw) :
                        content_str += '%s=%s;' %(kw, query[kw][0])
                    else:
                        content_str += '%s=;'   %(kw)
                print content_str
                print '\n'
                f = file(filepath + time.strftime('%Y%m%d%H')+".txt", 'aw')
                f.write(content_str+'\n')
                f.flush()
                f.close()
                break
