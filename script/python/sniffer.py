#!/usr/bin/python
import pcap
import dpkt
pc=pcap.pcap()
pc.setfilter('tcp')
for ptime,pdata in pc:
    #print ptime,pdata
    print ptime
    print `dpkt.ethernet.Ethernet(pdata)`
