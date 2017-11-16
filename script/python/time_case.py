#!/usr/bin/python
#Filename:time.py
import time
import datetime
print time.clock()
print time.time()
print time.ctime()
print time.strftime('%Y-%m-%d %H:%M:%S')
print time.strftime('%Y')

##day dec one day
print datetime.date.today() - datetime.timedelta(days=1)
print datetime.date.today()
#print time.__doc__
