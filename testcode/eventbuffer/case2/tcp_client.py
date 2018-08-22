#!/usr/bin/python
#Filename:tcp_client.pys
from socket import *
import thread
import time

HOST='localhost'
#PORT=5648
PORT=40713
BUFSIZE=1024
ADDR=(HOST,PORT)
def th(sock):
	while True:
		#data=raw_input("")
		time.sleep(1)
		data='hello world\n'
		if not data:
			break
		sock.send(data)

tcpCliSock=socket(AF_INET,SOCK_STREAM)
tcpCliSock.connect(ADDR)

thread.start_new_thread(th,(tcpCliSock,))
while True:
	data=tcpCliSock.recv(BUFSIZE)
	print data
tcpCliSock.close()


