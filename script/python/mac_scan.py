#!/usr/bin/python
"""
 LAN Mac Scan
 date 2013.4.22
"""
import sys, os
import time
from scapy.all import *

if os.geteuid() != 0:
    print "This program must be run as root. Aborting."
    sys.exit()
if len(sys.argv) < 2:
    print "Please Use %s x.x.x" % (sys.argv[0])
    exit()
conf.verb = 0
ipscan = sys.argv[1] + ".0/24"
f = file(time.strftime('%Y%m%d')+"Mac_list.txt", 'w')
ans, unans = srp(Ether(dst = "FF:FF:FF:FF:FF:FF")/ARP(pdst = ipscan), timeout = 2)
print ans[0]
scan_time = time.strftime('%Y-%m-%d %H:%M:%S')
for snd, rcv in ans:
    #print rcv
    list_mac = rcv.sprintf(" %Ether.src% -> %ARP.psrc%")
    print rcv.sprintf(" %Ether.src% -> %ARP.psrc%")
    f.write(scan_time+list_mac+'\n')
f.close()
