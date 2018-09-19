#!/usr/bin/python
# coding:utf-8
import socket

host = '192.168.1.7'    
#host = '18.191.243.72'
#两个段口必须一致
port = 6666 
addr = (host, port)
byte = 1024
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
while True:
    data = input('Please input the message: ')
    text = data.encode('utf-8')
    sock.sendto(text, addr)

    #输入'exit'退出
    if data == 'exit':
        break
    else:    
        #getsockname返回当前套接字的信息(IP,端口号)
        print("The OS assigned me the address {}".format(sock.getsockname()))
        #data, addr = sock.recvfrom(byte)
        #text = data.decode("utf-8")
        #print('The server {} replied{!r}'.format(addr, text))

sock.close()
