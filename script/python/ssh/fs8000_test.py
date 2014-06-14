#!/usr/bin/python
import sys
import telnetlib  
import time

counter = 0
running = True
while running:
	counter += 1
	print 'test %d times' %counter
	##login into main control board and reboot
	Host = "192.168.1.43"
	timeout = 60 
	tn = telnetlib.Telnet(Host)  
	tn.read_until("/ # ",timeout)  
	cmd = "reboot"
	tcmd = cmd + "\n"
	tn.write(tcmd)  
	tn.read_until("/ # ",timeout)  
	tn.close()

	##delay and wait data board boot over
	print 'reboot ing.....,please waiting'
	time.sleep(360)

	##login into data board
	Host = "192.168.1.10"
	timeout = 60 
	tn = telnetlib.Telnet(Host)  
	tn.read_until("/ # ",timeout)  
	## up card 
	cmd = "dtweak cs1999 reg up 0"
	tcmd = cmd + "\n"
	tn.write(tcmd)  
	tn.read_until(cmd, timeout)  
	tn.read_until("\n",timeout)  
	result = tn.read_until("/ # ",timeout)  
	print result
	if 'd3e5' in result:
		print 'cs1999 up init success'
	else:
		print 'cs1999 up init fail'
		break
	## dn card
	cmd = "dtweak cs1999 reg down 0"
	tcmd = cmd + "\n"
	tn.write(tcmd)  
	tn.read_until(cmd, timeout)  
	tn.read_until("\n",timeout)  
	result = tn.read_until("/ # ",timeout)  
	print result
	if 'd3e5' in result:
		print 'cs1999 dn init success'
	else:
		print 'cs1999 dn init fail'
		break
	tn.close()
else:
	print 'The while loop is over.'
	#Do anything else you want to do here

print 'Done'
