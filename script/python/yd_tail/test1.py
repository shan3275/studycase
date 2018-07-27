#!/usr/bin/python
#Filename:for.py
import time
import sys, os

with open('naga.log20180712') as f:
    for line in f:
       print(line)
       f = open("naga.log"+time.strftime('%Y%m%d'), 'a')
       f.write(line)
       f.flush()
       f.close()
       time.sleep(0.5)
