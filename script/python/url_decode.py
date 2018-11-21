#!/usr/bin/python
import urllib
import sys
string = sys.argv[1]
string = unicode(string,"gbk")
gbk_string=string.encode("gbk")
gbk=urllib.quote(gbk_string)
print gbk
#utf8_string = string.encode("utf-8")
#utf8=urllib.quote(utf8_string)
#print utf8