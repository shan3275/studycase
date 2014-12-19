#!/usr/bin/python
#Filename:scapy_test.py
import sys
import struct

src = 0
dst = 0
sp  = 0
dp  = 20000
byte = 0
f=open('packageset.txt','w')
for i in range(0,16384, 1):
        src += 1
        dst += 1
        sp  += 1
        dp  += 1
        src_str = "10.%d.%d.%d" %((src>>16)&0xff, (src>>8)&0xff, (src>>0)&0xff)
        dst_str = "20.%d.%d.%d" %((dst>>16)&0xff, (dst>>8)&0xff, (dst>>0)&0xff)
        filter = "@%s %s %d %d\n" %(src_str, dst_str, sp, dp)
        #print '%s' %(filter)
        f.write(filter)
else:
    f.close()
    print 'The for loop is over'
    print "send %d packet %d bytes" %(src, byte) 


