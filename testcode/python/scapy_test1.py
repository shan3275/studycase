#!/usr/bin/python2.7
#Filename:scapy_test.py
import sys
import struct
from scapy.all import *

data = struct.pack('=BHI', 0x12, 20, 1000)
pkts = IP(src='192.168.1.81', dst='192.168.1.10')/UDP(sport=12345,dport=5555)/data
wrpcap("temp.cap",pkts)
pkts = IP(src='192.168.1.82', dst='192.168.1.10')/UDP(sport=12345,dport=5555)/data
wrpcap("temp.cap",pkts)
#send(pkt, inter=1, count=5)
