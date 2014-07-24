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
	Host = '192.168.1.168'
	Username = 'admin'
	Password = 'admin'
	timeout = 10 
	tn = telnetlib.Telnet(Host, port=8000)  


	print tn.read_until('Username: ')
	tn.write(Username + '\r')

	print tn.read_until('Password:')
	tn.write(Password + '\r')

	tn.read_until('FS6400(admin)# ')
	tn.write('show version\r')
	print tn.read_until('(GuangZhou)')
	tn.close()
	break
else:
	print 'The while loop is over.'
	#Do anything else you want to do here

print '\n'
print time.ctime()
print 'Done'



