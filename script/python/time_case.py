#!/usr/bin/python
#coding:UTF-8
#Filename:time.py
import time
import datetime
print time.clock()
print time.time()
print time.ctime()
print time.strftime('%Y-%m-%d %H:%M:%S')
print time.strftime('%Y%m%d')
print time.strftime('%Y')

##day dec one day
yesterday =  datetime.date.today() - datetime.timedelta(days=1)
print yesterday.strftime('%Y%m%d')

print datetime.date.today()
#print time.__doc__

dt = "2016-05-05 20:28:00"

#转换成时间数组
timeArray = time.strptime(dt, "%Y-%m-%d %H:%M:%S")
print timeArray
#转换成时间戳
timestamp = time.mktime(timeArray)

print timestamp



print(datetime.datetime.now())
print(datetime.datetime.now() + datetime.timedelta(days=1))
print(datetime.datetime.now() - datetime.timedelta(hours=1))
print(datetime.datetime.now() + datetime.timedelta(minutes=1))
print(datetime.datetime.now() + datetime.timedelta(seconds=1))
