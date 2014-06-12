#!/usr/bin/python

# Filename : clitel.py

import sys

import telnetlib  
  

Host = sys.argv[1]
cmd = sys.argv[2]
timeout = 60 
#finish = "FS9000(admin)# "    
  
tn = telnetlib.Telnet(Host)  
 
tn.read_until("FS9000 login: ",timeout)  
tn.write("root\n")
tn.read_until("Password: ",timeout)
tn.write("root\n")

tn.read_until("root@FS9000:~# ",timeout)
tn.write("telnet 127.0.0.1 10000\n")
tn.read_until("username:",timeout)
tn.write("admin")
tn.write("\n")
tn.read_until("password:",timeout)
tn.write("admin")
tn.write("\n")
tn.read_until("FS9000(admin)# ",timeout)

if len(sys.argv)==4:
	file_load = sys.argv[3]
	f = open(file_load)
	line = f.readline()
	while 1:
		line = f.readline()
		if not line:
			break;
		tcmd = line + "\n"
		tn.write(tcmd)  
		output = tn.read_until("FS9000(admin)# ",timeout)
		st = output.find('Error')
		if st != -1:
			print line
			tn.close()
else:
	tcmd = cmd + "\n"
	tn.write(tcmd)  
	print tn.read_until("FS9000(admin)# ",timeout)
tn.close()

# End of clitel.phy
