#!/usr/bin/python
#Filename:scapy_test.py
import os
import sys
import struct

cmd0 = "restore"
cmd2 = "rule five-tuple commit"
IP   = "192.168.1.89" 
user = "root"
pwd  = "root"
#f=open('result.log','w')

for i in range(0, 4, 1):
        src = 1000 * (2**i)
        cmd1    = "rule five-tuple add 1 1.1.1.1+%d 2.2.2.2+%d 10000+%d 20000+%d tcp iifgrp 1 action forward oifgrp 1" %(src, src, src, src)
        print cmd1
        CMD0 = "./fc7000_cmd.py %s %s %s %s" %(IP, user, pwd, cmd0)
        CMD1 = "./fc7000_cmd.py %s %s %s \"%s\"" %(IP, user, pwd, cmd1)
        CMD2 = "./fc7000_cmd.py %s %s %s \"%s\"" %(IP, user, pwd, cmd2)

        p = os.popen(CMD0)
        x=p.read()
        print x

        p = os.popen(CMD1)
        x=p.read()
        print x

        p = os.popen(CMD2)
        x=p.read()
        print x
        #f.write(filter)
else:
    #f.close()
    print 'The for loop is over'
    print "loop %d" %(i) 


