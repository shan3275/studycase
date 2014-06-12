#!/usr/bin/python
#Filename:for.py

ip = 0
for i in range(1,1000000, 1):
        ip += 1
	print i
        str = "10.%d.%d.%d" %((ip>>16)&0xff, (ip>>8)&0xff, (ip>>0)&0xff)
        print str
else:
	print 'The for loop is over'
