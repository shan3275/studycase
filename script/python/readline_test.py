#!/usr/bin/python
#Filename:readline.py
import time
for line in open("20160826Mac_list.txt"):
    str = line.split()
    f = file(str[2]+".log", 'aw')
    f.write(line)
    f.close()
