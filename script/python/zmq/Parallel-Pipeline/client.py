#!/usr/bin/python
#-*-coding:utf-8-*-

import zmq
import time

context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.connect('tcp://localhost:5558')

while True:
    data = raw_input('input your data:')
    socket.send(data)
