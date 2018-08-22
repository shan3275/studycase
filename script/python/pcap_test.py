#!/usr/bin/env python
from scapy.all import *
try:
    # This import works from the project directory
    import scapy_http.http
except ImportError:
    # If you installed this package via pip, you just need to execute this
    from scapy.layers import http

 
eth = Ether(dst='00:00:00:00:00:01', src='00:00:00:00:00:02',type=0x8100)
vlan= Dot1Q(vlan=13,type=0x8061)
packets = rdpcap('/Users/liudeshan/i.pcap')
for p in packets:
    print('=' * 78)
    print p.show()
    #pring p['Ethernet'].type
    #print 'dst:%s' %(p.dst)
    #print 'src:%s' %(p.src)
    #print 'type:0x%x' %(p.type)
    p.type = 0xFFFF
    #print 'type:0x%x' %(p.type)
    pkt = eth/vlan/p
    pktdump = PcapWriter("/Users/liudeshan/banana.pcap", append=True, sync=True)
    pktdump.write(pkt)

