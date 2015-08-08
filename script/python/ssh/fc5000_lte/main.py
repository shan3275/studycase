#!/usr/bin/python
#Filename:scapy_test.py
import os
import sys
import struct

loop = 0

###############################################################
#              a. v5
#              b. v7
#              c. not fc5000
###############################################################
for i in range(10, 254, 1):
        host = "192.169.11.%s" %(i)
        print host
        CMD0 = "./fc5000.v5.expect %s" %(host)
        p  = os.popen(CMD0)
        x  = p.read()
        st = x.find('Mbps')
        if st != -1:
            # have found  out the date
            print '%s fc5000 v5, success!' %(host)
            loop += 1
        else:
            CMD1 = "./fc5000.v7.expect %s" %(host)
            p  = os.popen(CMD1)
            x  = p.read()
            st = x.find('Mbps')
            if st != -1:
                # have found out the date
                print '%s fc5000 v7, success!' %(host)
                loop += 1
            else:
                print 'not found'
                print '\n'
                continue

        print '\n'
        file_name = "%s.log" %(host)
        f=open(file_name,'w')
        f.write(x)
        f.close()

else:
    print 'The for loop is over'
    print "loop %d" %(loop) 


