#!/usr/bin/python
#Filename:os.py

import os

#RUN ls cmd
if os.system('date') == 0:
	print 'Success run cmd'

#print os.__doc__
print os.name
print os.getcwd()
#print os.getenv('./')
#print os.listdir('./')
#for file in os.listdir('./'):
for file in os.path.split('./'):
	print file
