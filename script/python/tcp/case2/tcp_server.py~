#!/usr/bin/python
#Filename:tcp_server
from socket import *
import thread

PORT=5648
BUFSIZE=1024

def th():
	while True:
		data=raw_input("")
		if not data:
			break
		tcpCliSock.send(data)
ADDR=("",PORT)
tcpSerSock=socket(AF_INET,SOCK_STREAM)
tcpSerSock.bind(ADDR)
tcpSerSock.listen(5)
print "waiting fot connection..."
tcpCliSock,addr=tcpSerSock.accept()
print "connection from :%s" % str(addr)
thread.start_new_thread(th,())
while True:
	data=tcpCliSock.recv(BUFSIZE)
	if not data:
		break
	print data
tcpSerSock.close()

