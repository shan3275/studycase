#!/usr/bin/python
# Filename : fc7000_cmd.py
import sys
import telnetlib

finish = "/ # "
config_finish = "MDU(config)# "
timeout = 20

IP  = sys.argv[1]
user= sys.argv[2]
pwd = sys.argv[3]
cmd = sys.argv[4]

tn = telnetlib.Telnet(IP, 2601)  

tn.read_until("Username: ",timeout)  
tn.write(user)
tn.write("\n")  

tn.read_until("Password: ",timeout)  
tn.write(pwd)
tn.write("\n")

tn.read_until("MDU> ",timeout)
tn.write("enable\n")  

tn.read_until("Password: ",timeout)  
tn.write("enable\n")  

tn.read_until("MDU# ",timeout)
tn.write("config\n")  

tn.read_until(config_finish,timeout)

tn.write(cmd)
tn.write("\n")
print tn.read_until(config_finish,timeout)

tn.write("exit\n")

tn.read_until("MDU# ",timeout)
tn.write("exit\n")  

tn.read_until("$ ", timeout)

tn.close()

# End of as5000_cmd.py
