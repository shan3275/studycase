#!/usr/bin/python
# coding:utf-8
import socket
byte = 1024
#两个端口要保持一致
port = 6666  
host = ""
addr = (host, port)

#创建套接字
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
#绑定
sock.bind(addr)
print("waiting to receive messages...")

while True:
    (data, addr) = sock.recvfrom(byte)
    text = data.decode('utf-8')
    if text == 'exit':
        break
    else :
        print('The client at {} says {!r}'.format(addr, text))
        text = 'Your data was {}bytes long'.format(len(data))
        data = text.encode('utf-8')
        #sock.sendto(data, addr)

#关闭套接字
sock.close()
