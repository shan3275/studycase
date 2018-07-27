#!/usr/bin/python
# coding:utf-8
#Filename:tail.py
# 用途：1.将之前小时的文件加密，然后上传到服务器
#       2.每天8点和下午1点传两次。
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
    logger.addHandler(sh)

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
    cmd = '/usr/bin/curl  -T "%s" http://192.168.1.13:5000/log\?plateform\=weibo\&type\=normal\&src\=hehe' %(ency_file )
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


    logger.info('over')
    return True

def business_process(input_path, output_path):
    skip_file = time.strftime('%Y%m%d%H')+".txt"

    #获取当前文件夹中的文件名称列表
    filenames=os.listdir(input_path)
    #先遍历文件名
    for filename in filenames:
        if filename == skip_file:
            logger.info('skip_file:%s', filename)
            continue
        input_file = input_path+filename
        logger.info('input_file : %s',input_file)
        ency_file = output_path + 'ency' + filename

        ##判断文件是否存在
        if os.path.exists(input_file) != True:
            logger.error('temp日志文件不存在')
            continue

        if os.path.exists(ency_file) == True:
            logger.error('压缩文件已存在')
            continue

        ency_result  = encode_file(input_file,ency_file)
        if ency_result == False:
            logger.error('压缩文件错误退出')
            continue
        upload_result = upload_file(ency_file)
        if upload_result == False:
            logger.error('上传文件错误退出')
            continue
        ##删除文件
        #if os.path.exists(input_file):
        #    os.remove(input_file)
        #    logger.info('删除日志文件：%s', input_file)
        #else:
        #    logger.info('no such file:%s', input_file)
        #if os.path.exists(ency_file):
        #    os.remove(ency_file)
        #    logger.info('删除压缩文件：%s', ency_file)
        #else:
        #    logger.info('no such file:%s' ,ency_file)

if __name__ == '__main__':
    conf = load_yaml('upload.yaml')
    input_path    = conf['output_path']
    output_path   = conf['ency_path']
    log_file      = conf['log']['log_file']
    log_app       = conf['log']['log_app']
    if input_path == None or output_path == None:
        print 'configuration file error, input_file or output_path is None!!'
        os._exit()
    if log_file == None or log_app == None:
        print 'configuration file error, log_file or log_app is None!!'
        os._exit()

    logger = log_init(log_app, log_file)
    logger.info('input_path   :%s', input_path)
    logger.info('outpath      :%s', output_path)
    logger.info('log_file     :%s', log_file)
    logger.info('log_app      :%s', log_app)
    h1 = 8
    m1 = 5
    h2 = 13
    m2 = 5
    while True:
        # 判断是否达到设定时间，例如0:00
        while True:
            now = datetime.datetime.now()
            # 到达设定时间，结束内循环
            if now.hour==h1 and now.minute==m1:
                break
            if now.hour==h2 and now.minute==m2:
                break
            # 不到时间就等20秒之后再次检测
            logger.info('keepalive, wait for 50秒')
            time.sleep(50)
        #做正事，一天做一次
        business_process(input_path, output_path)
