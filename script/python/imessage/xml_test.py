#!/usr/bin/python
# -*- coding=utf-8 -*-
# version: 0.1 
import plistlib
pl = plistlib.readPlist('./config-1.plist')
print pl
#print pl['RtVariables']['MLB']
#print pl['RtVariables']['ROM']
#plistlib.writePlist(pl, './out.plist')
