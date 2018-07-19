#!/usr/bin/python
#Filename:readline.py
from urllib import unquote
import time
for line in open("4.txt"):
    line_str = line.split()
    if len(line_str) > 8:
        #print line_str[7]
        if 'url=' in line_str[7]:
            apk_url = line_str[7].split('url=', 1)[1]
            if len(apk_url) > 2:
                #print apk_url
                s=unquote(apk_url)
                print s

