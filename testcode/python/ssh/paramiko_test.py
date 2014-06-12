#!/usr/bin/python
#Filename:paramiko.py

import paramiko

ssh = paramiko.SSHClient()
ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
ssh.connect("192.168.1.35",22,"root"," ")
stdin, stdout, stderr = ssh.exec_command("date")
print stdout.readlines()
ssh.close()
