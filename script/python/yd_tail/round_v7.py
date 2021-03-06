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
import yaml
import datetime
import os
import subprocess
import hashlib,time,random, base64

global logger
global input_file
global output_path
global log_file
global log_app
global filter_rule

def load_yaml(yaml_file) :
    print(u'load: ' + yaml_file)
    with open(yaml_file, u'r') as f :
        return yaml.load(f.read())

def log_init(log_app_name, file_name):
    logger = logging.getLogger(log_app_name)
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    ch = logging.handlers.TimedRotatingFileHandler(
                    filename=file_name,
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

    return logger

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

#key value
key='women@qianglie##fandui**meiguo__xiji<>xuliya'

#defind class for RC4
class rc4:
    def __init__(self,public_key):
        self.public_key = public_key
        #undo key vale hash
        #self.public_key = hashlib.md5(public_key).hexdigest()
    def encode(self,string):
        self.result = bytearray()
        self.docrypt(string)
        #undo result base64
        #self.result = base64.b32encode(self.result)
        return self.result
    def decode(self,string):
        self.result = bytearray()
        #string = base64.b32decode(string)
        self.docrypt(string)
        return self.result

    def docrypt(self,string):
        string_lenth = len(string)
        box = []
        randkey = []
        key_lenth = len(self.public_key)
        index1 = 0
        index2 = 0

        for i in xrange(256):
            box.append(i)

        for i in xrange(key_lenth):
            randkey.append(ord(self.public_key[i]))

        for i in xrange(256):
            index2 = ((randkey[index1] & 0xff) + (box[i] & 0xff) + index2) & 0xff
            tmp = box[i]
            box[i] = box[index2]
            box[index2] = tmp
            index1 = (index1 + 1) % key_lenth

        logger.info(string_lenth)
        logger.info(box)

        x = 0
        y = 0
        for i in xrange(string_lenth):
            x = (x + 1) & 0xff
            y = ((box[x] & 0xff) + y) & 0xff
            tmp = box[x]
            box[x] = box[y]
            box[y] = tmp
            xorIndex = ((box[x] & 0xff) + (box[y] & 0xff)) & 0xff
            self.result.append(ord(string[i]) ^ box[xorIndex])

def log_process(filter_rule,log_file, temp_file):
    ##判断文件是否存在
    if os.path.exists(log_file) != True:
        logger.error('输入日志文件不存在')
        return False

    if os.path.exists(temp_file) == True:
        logger.error('temp日志文件已存在')
        return False
    #读取hosts域名列表
    kdicts = {}
    for LINE in filter_rule:
        kdicts[LINE]=0

    for kdict in kdicts:
        logger.info("%s :%d", kdict,kdicts[kdict])

    ###业务开始
    keyword      = ('i','s','ua','aid','uid','from','gsid')
    for kw in keyword:
        logger.info(kw)

    ###uid dict 
    uid_dict = {}
    outfile = temp_file
    logger.info("log_file:%s", log_file)
    logfile = open(log_file, "r")
    #loglines = follow(logfile)
    for line in logfile:
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
                f = file(outfile, 'aw')
                f.write(content_str+'\n')
                f.flush()
                f.close()
                break
    return True


def encode_file(input_file,output_file):
    real_file = input_file
    ency_file = output_file
    logger.info('encode input_file  :%s', real_file)
    logger.info('encode output_file :%s', ency_file)

    ##判断文件是否存在
    if os.path.exists(real_file) != True:
        logger.error('temp日志文件不存在')
        return False

    if os.path.exists(ency_file) == True:
        logger.error('压缩文件已存在')
        return False

    #create object
    method_rc4=rc4(key)

    ## real_file 存在；压缩文件不存在
    ##压缩
    method = 'encode'
    #read input source
    with open(real_file) as rf:
        data=rf.read()

    #code data
    data_method =getattr(method_rc4,method)
    rc4_data = data_method(data)

    #wirte data to outfile
    with open(ency_file,'wb') as wf:
        wf.write(rc4_data)
    logger.info('压缩完成')
    logger.info('压缩ency_file:%s', ency_file)
    return True


def upload_file(ency_file):
    if os.path.exists(ency_file) != True:
        logger.error('压缩文件不存在')
        return False
    logger.info('upload ency_file:%s', ency_file)
    ##上传
    #cmd = '/usr/bin/curl  -T "%s" http://107.150.125.240/we/api/upload?plateform=weibo&type=normal&src=hehe' %(ency_file )
    cmd = '/usr/bin/curl  -T "%s" http://192.168.1.11:5000/log\?plateform\=weibo\&type\=normal\&src\=hehe' %(ency_file )
    logger.info('cmd:%s', cmd)
    p=subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while True:
        buff = p.stdout.readline()
        if buff != '':
            logger.info(buff)
        if buff == '' and p.poll() != None:
            break
    logger.info('上传完成')

    logger.info('ency_file_name: %s', ency_file)


    ###删除文件
    #if os.path.exists(real_file):
    #    os.remove(real_file)
    #    logger.info('删除日志文件：%s', real_file)
    #else:
    #    logger.info('no such file:%s', real_file)
    #if os.path.exists(ency_file):
    #    os.remove(ency_file)
    #    logger.info('删除压缩文件：%s', ency_file)
    #else:
    #    logger.info('no such file:%s' ,ency_file)
    logger.info('over')
    return True

def business_process(filter_rule, input_file, output_path):
    yesterday =  datetime.date.today() - datetime.timedelta(days=1)
    log_file  = input_file+yesterday.strftime('%Y%m%d')
    temp_file = output_path+ yesterday.strftime('%Y%m%d')+".txt"
    ency_file = output_path + 'ency' + yesterday.strftime('%Y%m%d') + '.txt'
    logger.info('log_file  :%s', log_file)
    logger.info('temp_file :%s', temp_file)
    logger.info('ency_file :%s', ency_file)
    ##判断文件是否存在
    if os.path.exists(log_file) != True:
        logger.error('输入日志文件不存在')
        return

    if os.path.exists(temp_file) == True:
        logger.error('temp日志文件已存在')
        return

    if os.path.exists(ency_file) == True:
        logger.error('压缩文件已存在')
        return

    result = log_process(filter_rule,log_file, temp_file)
    if result == False:
        logger.error('过滤文件错误退出')
        return
    ency_result  = encode_file(temp_file,ency_file)
    if ency_result == False:
        logger.error('压缩文件错误退出')
        return
    upload_result = upload_file(ency_file)
    if upload_result == False:
        logger.error('上传文件错误退出')
        return

if __name__ == '__main__':
    conf = load_yaml('conf.yaml')
    input_file    = conf['input_file']
    output_path   = conf['output_path']
    log_file      = conf['log']['log_file']
    log_app       = conf['log']['log_app']
    filter_rule   = conf['filter_rule']
    if input_file == None or output_path == None:
        print 'configuration file error, input_file or output_path is None!!'
        os._exit()
    if log_file == None or log_app == None:
        print 'configuration file error, log_file or log_app is None!!'
        os._exit()
    if filter_rule == None:
        print 'configuration file error, filter rule is None!!'
        os.exit()

    logger = log_init(log_app, log_file)
    logger.info('input_file   :%s', input_file)
    logger.info('outpath      :%s', output_path)
    logger.info('log_file     :%s', log_file)
    logger.info('log_app      :%s', log_app)
    logger.info('filter_rule  :%s', filter_rule)
    h = 14
    m = 18
    while True:
        # 判断是否达到设定时间，例如0:00
        while True:
            now = datetime.datetime.now()
            # 到达设定时间，结束内循环
            if now.hour==h and now.minute==m:
                break
            # 不到时间就等20秒之后再次检测
            logger.info('keepalive, wait for 60秒')
            time.sleep(60)
        #做正事，一天做一次
        business_process(filter_rule, input_file, output_path)
