#!/usr/bin/python
#Filename:scapy_test.py
import sys
import struct
import socket
from scapy.all import *

data = struct.pack('=BHIIIIIIIIIII', 0x12, 20, 1000,1001,1002,1003,1004,1005,1006,1007,1008,1009,10010)
src = 0
dst = 0
byte = 0
for i in range(0,10, 1):
        src += 1
        #dst += 1
        src_str = "10.%d.%d.%d" %((src>>16)&0xff, (src>>8)&0xff, (src>>0)&0xff)
        dst_str = "20.%d.%d.%d" %((dst>>16)&0xff, (dst>>8)&0xff, (dst>>0)&0xff)
        pkt = IP(src=src_str, dst=dst_str)/TCP(sport=12345,dport=5555)/data/data/data
        eth = Ether(dst="AA:BB:CC:DD:EE:FF")
        #pkt.show()
        sendp(eth/pkt, iface="en0", verbose=0)
        #send(pkt, iface="lo")
        byte += len(eth/pkt)
        #wrpcap("aa.pcap",pkt)
        pktdump = PcapWriter("banana.pcap", append=True, sync=True)
        pktdump.write(pkt)
else:
	print 'The for loop is over'
	print "send %d packet %d bytes" %(src, byte)

