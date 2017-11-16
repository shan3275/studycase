#!/usr/bin/env python
#-*- coding:UTF-8 -*-
import sys
import webbrowser
import os
import time
sys.path.append("libs")


url = 'http://www.lanmeidata.com'

for i in range(1,200, 1):
        webbrowser.open(url)
        print webbrowser.get()
        time.sleep(4)
        os.system("killall -9 firefox")
else:
	print 'The for loop is over'

