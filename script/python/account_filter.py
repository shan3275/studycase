#!/usr/bin/python
# coding:utf-8
#Filename:readline.py
#从账号文件中读取账号，通过sed进行删除匹配的行
import time
import os,sys
file = open("/Users/liudeshan/test/accoutn.txt")
for line in file.readlines():
	str = line.strip("\n")
	cmd = ' LC_CTYPE=C && sed -i "" "/^%s/d" /Users/liudeshan/test/9.17.txt' %(str)
	os.system(cmd)
	#print str
	#print cmd