#!/usr/bin/python
# -*- coding: UTF-8 -*-
#Filename:using_sys.py

import sys

print 'The comm and line arguments are:'
for i in sys.argv:
	print i

print '参数个数为:', len(sys.argv), '个参数'
print '参数列表:%s', str(sys.argv)


print '\n\nThe PYTHON PATH is', sys.path, '\n'
