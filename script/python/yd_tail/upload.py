#!/usr/bin/python
# coding:utf-8
#Filename:cup.py
# 用途：将日志自动上传到百度云盘中
# 1. 文件自动压缩
# 2. 当天将前一天的日志全部上传
# 3. 压缩日志上传之后，源文件及压缩文件自动删除
import time
import json
from collections import OrderedDict
import sys,os
import re
import base64
import requests
import tarfile
from optparse import OptionParser
import datetime
import subprocess
import hashlib,time,random, base64
import threading

global options
global method_rc4

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

        print string_lenth
        print box

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

def upload():
    ##获取当前的时间
    lasthour     =   datetime.datetime.today() - datetime.timedelta(hours=1)
    lasthour_str =  lasthour.strftime('%Y%m%d%H')
    print lasthour_str
    real_file = '%s%s.txt' %(options.ipath, lasthour_str)
    print 'real_file:%s' %(real_file)
    ency_file = '%sency%s.txt' %(options.opath, lasthour_str)
    print 'ency_file:%s' %(ency_file)

    ##判断文件是否存在
    if os.path.exists(real_file) != True:
        return

    if os.path.exists(ency_file) == True:
        return

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
    print '压缩完成'


    ##上传
    #cmd = '/usr/bin/curl  -T "%s" http://107.150.125.240/we/api/upload?plateform=weibo&type=normal&src=hehe' %(ency_file )
    cmd = '/usr/bin/curl  -T "%s" http://0.0.0.0:5000/log\?plateform\=weibo\&type\=normal\&src\=hehe' %(ency_file )
    p=subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while True:
        buff = p.stdout.readline()
        if buff != '':
            print buff
        if buff == '' and p.poll() != None:
            break
    print '上传完成,时间：'
    print time.strftime('%Y-%m-%d %H:%M:%S')

    ency_file_name = ency_file.split('/')[-1]
    print 'ency_file_name: %s' %(ency_file_name)


    ###删除文件
    if os.path.exists(real_file):
        os.remove(real_file)
        print '删除日志文件：%s' %(real_file)
    else:
        print 'no such file:%s' %(real_file)
    if os.path.exists(ency_file):
        os.remove(ency_file)
        print '删除压缩文件：%s' %(ency_file)
    else:
        print 'no such file:%s' %(ency_file)
    print 'over时间：'
    print time.strftime('%Y-%m-%d %H:%M:%S')


def fun_timer():
    upload()
    print time.strftime('%Y-%m-%d %H:%M:%S')
    global timer
    timer = threading.Timer(5, fun_timer)
    timer.start()

if __name__ == '__main__':
    print '开始时间：'
    print time.strftime('%Y-%m-%d %H:%M:%S')
    ##输入参数
    usage = "usage: %prog [options]"
    parser = OptionParser(usage)
    parser.add_option("-i", "--ipath", type="string", dest="ipath", help="read log from FILENAME")
    parser.add_option("-o", "--opath", type="string", dest="opath", help="output FILENAME")

    (options, args) = parser.parse_args()

    if options.ipath == None :
        parser.error("Invaild param")

    if options.opath == None :
        parser.error("Invaild param")

    #create object
    method_rc4=rc4(key)

    timer = threading.Timer(5, fun_timer)
    timer.start()
