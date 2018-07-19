#!/usr/bin/python
#-*-coding:utf-8-*-

import zmq
import time

context = zmq.Context()
socket = context.socket(zmq.PUSH)
#socket.connect('tcp://localhost:5558')
#socket.connect('tcp://118.31.77.163:9988')
socket.connect('tcp://47.98.106.37:9988')

while True:
    data = raw_input('input your data:')
    socket.send(data)
