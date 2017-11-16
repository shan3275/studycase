#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys, getopt

def argv_probe(argv):
   interface = ''
   direction = ''
   rate = ''
   try:
       opts, args = getopt.getopt(argv[1:],"hi:d:r:",["interface=","direction=","rate="])
   except getopt.GetoptError:
      print '%s -i <interface> -d <rx|tx> -r <rate>' %(argv[0])
      print '   -i --interface eth0|eth1|enp0....'
      print '   -d --direction tx|rx'
      print '   -r --rate uint:KByteps'
      print '   for example:'
      print '   %s -i eth0 -d tx -r 20' %(argv[0])
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print '%s -i <interface> -d <rx|tx> -r <rate>' %(argv[0])
         print '   -i --interface eth0|eth1|enp0....'
         print '   -d --direction tx|rx'
         print '   -r --rate uint:KByteps'
         print '   for example:'
         print '   %s -i eth0 -d tx -r 20' %(argv[0])
         sys.exit()
      elif opt in ("-i", "--interface"):
         interface = arg
      elif opt in ("-d", "--direction"):
         direction = arg
      elif opt in ("-r", "--rate"):
         rate = arg

   print '接口为：    ', interface
   print '方向为：    ', direction
   print '速率(KBps)：', rate

   if ( interface == '' or direction == '' or rate == '' ):
      print '%s -i <interface> -d <rx|tx> -r <rate>' %(argv[0])
      print '   -i --interface eth0|eth1|enp0....'
      print '   -d --direction tx|rx'
      print '   -r --rate uint:KByteps'
      print '   for example:'
      print '   %s -i eth0 -d tx -r 20' %(argv[0])
      sys.exit(2)


if __name__ == "__main__":
   argv_probe(sys.argv)
