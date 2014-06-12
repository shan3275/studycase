#!/usr/bin/python

# Filename : ssh_cmd.py

import sys

import paramiko

serv_ip = sys.argv[1]
serv_username = sys.argv[2]
serv_password = sys.argv[3]

ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())

ssh.connect(serv_ip, 22, serv_username, serv_password)

for cmd in sys.argv[4:] : 
    stdin, stdout, stderr = ssh.exec_command(cmd)
    for  line in  stdout.readlines(): 
         print  line,;
ssh.close()

# End of ssh_cmd.py
