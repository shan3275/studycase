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
import sys
import re
import base64
import requests
import tarfile
from optparse import OptionParser
import datetime
import os
import subprocess

if __name__ == '__main__':
    print '开始时间：'
    print time.strftime('%Y-%m-%d %H:%M:%S')
    ##输入参数
    usage = "usage: %prog [options]"
    parser = OptionParser(usage)
    parser.add_option("-i", "--ipath", type="string", dest="ipath", help="read log from FILENAME")
    parser.add_option("-r", "--rpath", type="string", dest="rpath", help="remote upload path")

    (options, args) = parser.parse_args()

    if options.ipath == None :
        parser.error("Invaild param")

    if options.rpath == None :
        parser.error("Invaild param")

    ##获取当前的时间
    yesterday     =   datetime.date.today() - datetime.timedelta(days=1)
    yesterday_str =  yesterday.strftime('%Y%m%d')
    real_file = '%s%s' %(options.ipath, yesterday_str)
    print 'real_file:%s' %(real_file)
    tgz_file = '%s.tgz' %(real_file)
    print 'tgz_file:%s' %(tgz_file)

    ##判断文件是否存在
    if os.path.exists(real_file) != True:
        os._exit(0)

    if os.path.exists(tgz_file) == True:
        os._exit(0)

    ## real_file 存在；压缩文件不存在
    ##压缩
    tarHandle=tarfile.open(tgz_file,"w:gz")
    tarHandle.add(real_file)
    tarHandle.close()
    print '压缩完成'


    ##上传
    #cmd = '/usr/local/bin/bypy --config-dir /root/.bypy  -v upload %s %s' %(tgz_file, options.rpath)
    cmd = '/usr/local/bin/bypy  -v upload %s %s' %(tgz_file, options.rpath)
    p=subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while True:
        buff = p.stdout.readline()
        if buff != '':
            print buff
        if buff == '' and p.poll() != None:
            break
    print '上传完成,时间：'
    print time.strftime('%Y-%m-%d %H:%M:%S')

    tgz_file_name = tgz_file.split('/')[-1]
    print 'tgz_file_name: %s' %(tgz_file_name)

    cmd = '/usr/local/bin/bypy   list %s%s' %(options.rpath, tgz_file_name)
    content = ''
    p=subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while True:
        buff = p.stdout.readline()
        if buff == '' and p.poll() != None:
            break
        if buff != '':
            print buff
            content += buff

    ##删除文件
    if tgz_file_name in content :
        if os.path.exists(real_file):
            os.remove(real_file)
            print '删除日志文件：%s' %(real_file)
        else:
            print 'no such file:%s' %(real_file)
        if os.path.exists(tgz_file):
            os.remove(tgz_file)
            print '删除压缩文件：%s' %(tgz_file)
        else:
            print 'no such file:%s' %(tgz_file)
    print 'over时间：'
    print time.strftime('%Y-%m-%d %H:%M:%S')
