#!/usr/bin/python
#Filename:for.py
import time
import sys, os

ip = 0
for i in range(1,1000000, 1):
        times = time.strftime('%Y-%m-%d %H:%M:%S\n')
        begin_str = 'Statistics of date and time: ' + times
        url = "ITF_IPKTS                               : 819724\nITF_IBYTS                               : 459240552\nOUTERL3_IPV4                            : 819724\nOUTERL4_TCP                             : 819724\nAPP_HTTP_UP                             : 783782\nAPP_HTTP_UP_OVER                        : 783782\nAPP_HTTP_DN                             : 62\nAPP_OTHER                               : 35880\nAPP_HTTP_GET                            : 783775\nAPP_HTTP_OTHER                          : 7\nNET_PKTS                                : 819724\nNET_UNURLPKTS                           : 35949\nNET_URLPKTS                             : 783775\nNET_UNMATCHPKTS                         : 783775\nDNET_PKTS                               : 819724\nDNET_UNURLPKTS                          : 35949\nDNET_URLPKTS                            : 783775\nDNET_UNMATCHPKTS                        : 783775\nURL_PKTS                                : 1512268\nURL_MATCHED                             : 55843\nURL_DISMATCH                            : 1456424\nMASK_PKTS                               : 728493\nMASK_ZERO                               : 728493\nACL_PKTS                                : 55281\nACL_URLPUSH_PKTS                        : 55281\nACL_URLPUSH_DROP_GET_OR_POST            : 18450\nACL_URLPUSH_FILTER_DROP                 : 16907\nACL_URLPUSH_TX_PKTS                     : 19924\n"
        other = "ACL_URLPUSH_TX_SUCCESS                  : %d\n" %(i)
        str = begin_str + url + other
        print str
        time.sleep(1)
        f = open("stat-log", 'a')
        f.write(str)
        f.flush()
        f.close()
else:
	print 'The for loop is over'
