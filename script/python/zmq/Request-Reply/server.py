#!/usr/bin/python
#-*-coding:utf-8-*-
import time
import zmq

context = zmq.Context()
socket = context.socket(zmq.REP)
socket.bind("tcp://*:5555")

while True:
    message = socket.recv()
    print message
    #time.sleep(1)
    socket.send("server response!")
