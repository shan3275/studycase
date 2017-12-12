# coding:utf-8

"""
file name: disk_manager.py
env: python 2.7

Monitor on the target disk
If disk usage out of limit bounds
Execute the cleanup operation
"""

import os
import sys
import psutil # pip install psutil
import time
import requests
import getopt

def argv_probe(argv):
   info      = ''
   try:
       opts, args = getopt.getopt(argv[1:],"hf:",["flag="])
   except getopt.GetoptError:
      print ("%s  -f <flag_info>" %(argv[0]))
      print ("   -f --flag interface flag info")
      print ("   for example:")
      print ("   %s  -f server1_info" %(argv[0]))
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print ("%s  -f <flag_info>" %(argv[0]))
         print ("   -f --flag interface flag info")
         print ("   for example:")
         print ("   %s  -f server1_info" %(argv[0]))
         sys.exit()
      elif opt in ("-f", "--flag"):
         info = arg

   if ( info == ''):
      print ("%s  -f <flag_info>" %(argv[0]))
      print ("   -f --flag interface flag info")
      print ("   for example:")
      print ("   %s  -f server1_info" %(argv[0]))
      sys.exit(2)

   return (info)

def send_simple_message(message):
    return requests.post(
        "https://api.mailgun.net/v3/mg.oncompress.com/messages",
        auth=("api", "key-6c77894e772490822d55b4e4b6651894"),
        data={"from": "Admin <postmaster@mg.oncompress.com>",
              "to": ["shan275@qq.com"],
              "subject": "监控报警",
              "text": message})

DISK_USAGE_LIMT = 30 # 磁盘使用界限值70%
DIST_DISK       = '/' # 目标监控磁盘

# disk_usage判断是否超出界限值
def monitor_disk(info):
    global DISK_USAGE_LIMT
    global DIST_DISK
    disk_percent = psutil.disk_usage(DIST_DISK).percent
    if disk_percent > DISK_USAGE_LIMT:
        message = "磁盘告警！%s %s Disk space usage: %s%%,  报警报警,请注意清理磁盘！！" %(time.strftime('%Y-%m-%d %H:%M:%S'), info, disk_percent)
	send_simple_message(message)
    else:
        message = "磁盘信息  %s %s Disk space usage: %s%%" %(time.strftime('%Y-%m-%d %H:%M:%S'), info, disk_percent)
    print message


if __name__ == "__main__":
    info = argv_probe(sys.argv)
    monitor_disk(info)
