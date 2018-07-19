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
import logging
from logging import handlers

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
    filepath = '/Users/liudeshan/work/studycase/script/python/yd_tail/'
    logger = logging.getLogger('WB')
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    ch = logging.handlers.TimedRotatingFileHandler(
                    filename=filepath+'round.log',
                    when='midnight',
                    backupCount=3
                    )
    ch.setLevel(logging.DEBUG)
    ch.setFormatter(formatter)
    # add ch to logger
    logger.addHandler(ch)

    #控制台输出
    sh = logging.StreamHandler()
    sh.setFormatter(formatter)
    #logger.addHandler(sh)

    #读取hosts域名列表
    hosts_path = 'round-hosts.conf'
    kdicts = {}
    if hosts_path != None:
        hfile = open(hosts_path, "r")
        for LINE in hfile:
            LINE = LINE.strip('\n')
            kdicts[LINE]=0
    else:
        logger.error("没有发现hosts配置文件，退出")
        os._exit()
    for kdict in kdicts:
        logger.info("%s :%d", kdict,kdicts[kdict])

    ###业务开始
    keyword      = ('i','s','ua','aid','uid','from','gsid')
    for kw in keyword:
        logger.info(kw)

    ###uid dict 
    uid_dict = {}
    log_file = filepath + 'naga.log' + time.strftime('%Y%m%d')
    logger.info("log_file:%s", log_file)
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
                    logger.info("字段不全")
	            break;
	        if query['i'][0] == False or query['s'][0] == False or query['ua'][0] == False or query['aid'][0]==False or query['gsid'][0]==False or query['uid'][0]==False :
                    logger.info("字段有零")
		    break;
                if len(query['uid'][0]) != 10 :
                    logger.info("uid长度不为0")
                    break
                if query['uid'][0].isdigit() == False :
                    logger.info("uid不全是数值")
                    break
                logger.info("uid is number")
                if uid_dict.has_key(query['uid'][0]) == True:
                    uid_dict[query['uid'][0]] = uid_dict.get(query['uid'][0]) + 1
                    logger.info("uid has existed, uid:%s, number:%d", query['uid'][0], uid_dict.get(query['uid'][0]))
                    break;
                else:
                    uid_dict[query['uid'][0]] = 1
                    logger.info("new uid, uid:%s, number:%d", query['uid'][0], uid_dict.get(query['uid'][0]))
                #if len(query['aid'][0]) <= 50 :
                #    break
                #print 'aid is > 50 bytes'
                logger.info("match!")
		kdicts[kdict] = kdicts[kdict] + 1
                logger.info("%s :%d", kdict, kdicts[kdict])
                content_str =''
                for kw in keyword:
                    if query.has_key(kw) :
                        content_str += '%s=%s;' %(kw, query[kw][0])
                    else:
                        content_str += '%s=;'   %(kw)
                logger.info(content_str)
                f = file(filepath + time.strftime('%Y%m%d%H')+".txt", 'aw')
                f.write(content_str+'\n')
                f.flush()
                f.close()
                break
