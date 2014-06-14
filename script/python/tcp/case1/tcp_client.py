#!/usr/bin/python
#coding:utf-8

import socket
import sys
import time

HOST='192.168.2.36'
PORT=443
BUFSIZ=1024
while True:
	client = socket.socket()
	client.connect((HOST,PORT))
	client.send('switch.php')
	data = client.recv(BUFSIZ)
	print data
	time.sleep(2)
