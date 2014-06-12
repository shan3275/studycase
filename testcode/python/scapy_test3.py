#!/usr/bin/python2.7
#Filename:scapy_test.py
import sys
import struct
from scapy.all import *

data = struct.pack('=BHI', 0x12, 20, 1000)
src = 0
dst = 0
for i in range(1,10, 1):
        src += 1
        dst += 1
        src_str = "10.%d.%d.%d" %((src>>16)&0xff, (src>>8)&0xff, (src>>0)&0xff)
        dst_str = "20.%d.%d.%d" %((dst>>16)&0xff, (dst>>8)&0xff, (dst>>0)&0xff)
        pkt = IP(src=src_str, dst=dst_str)/UDP(sport=12345,dport=5555)/data
        eth = Ether()
        #pkt.show()
        sendp(eth/pkt, iface="lo")
        #send(pkt, iface="lo")
else:
	print 'The for loop is over'
