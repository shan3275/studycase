#!/usr/bin/python2.7 -tt

from scapy.all import *

pkts = []
iter = 0
pcapnum = 0

def makecap(x):
    global pkts
    global iter
    global pcapnum
    pkts.append(x)
    iter += 1
    if iter == 500:
        pcapnum += 1
        pname = "pcap%d.pcap" % pcapnum
        wrpcap(pname, pkts)
        pkts = []
        iter = 0

while 1:
    sniff(prn=makecap)
