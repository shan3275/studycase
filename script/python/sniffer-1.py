#!/usr/bin/python
#coding:utf-8
import pcap
import dpkt
import sys
aaa='a'
bbb='b'
pc=pcap.pcap()    #注，参数可为网卡名，如eth0
pc.setfilter('tcp port 80')    #设置监听过滤器
for ptime,pdata in pc:    #ptime为收到时间，pdata为收到数据
    p=dpkt.ethernet.Ethernet(pdata)
    if p.data.__class__.__name__=='IP':
        ip='%d.%d.%d.%d'%tuple(map(ord,list(p.data.dst)))
        #print ip
        if p.data.data.__class__.__name__=='TCP':
            if p.data.data.dport==80:
                sStr1 = p.data.data.data
                sStr2 = 'Host: '
                sStr3 = 'Connection'
                sStr4 = 'GET /'
                sStr5 = ' HTTP/1.1'
                nPos = sStr1.find(sStr3)
                nPosa = sStr1.find(sStr5)
                for n in range(sStr1.find(sStr2)+6,nPos-1):
                    aaa=sStr1[sStr1.find(sStr2)+6:n]
                for n in range(sStr1.find(sStr4)+4,nPosa+1):
                    bbb=sStr1[sStr1.find(sStr4)+4:n]
                ccc=aaa+bbb
                print ccc
