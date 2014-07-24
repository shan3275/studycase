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
	Host = '192.168.40.3'
	Username = 'root'
	Password = 'root'
	timeout = 10 
	tn = telnetlib.Telnet(Host, port=16000)  


    print tn.read_until('Please input usrname:\n')
	tn.write(Username + '\r')

    print tn.read_until('Please input password:\n')
	tn.write(Password + '\r')

	tn.read_until('mdu# ')
	tn.write('show version\r')
    print tn.read_until('Number:')
	tn.close()
	break
else:
	print 'The while loop is over.'
	#Do anything else you want to do here

print '\n'
print time.ctime()
print 'Done'



