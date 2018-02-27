#!/usr/bin/env python
#-*- coding:UTF-8 -*-
import sys
import webbrowser
import os
import time
sys.path.append("libs")


url = 'http://y.code.17tanwan.com/htmlcode/15892.html'

for i in range(1,51, 1):
        webbrowser.open(url)
        print '次数：%d, %s' %(i, webbrowser.get())
        time.sleep(6)
        #os.system("killall -9 firefox")
        #os.system("killall -9 Safari")
        os.system("killall -9 Google\ Chrome")
else:
	print 'The for loop is over'

