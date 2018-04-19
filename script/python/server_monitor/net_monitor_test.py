#/usr/local/bin/python
#coding=utf-8
import sys,re,time,os
import json
import requests
import getopt

def argv_probe(argv):
   info      = ''
   interface = ''
   direction = ''
   rate = ''
   try:
       opts, args = getopt.getopt(argv[1:],"hi:d:r:f:",["interface=","direction=","rate=","flag="])
   except getopt.GetoptError:
      print ("%s -i <interface> -d <rx|tx> -r <rate> -f <flag_info>" %(argv[0]))
      print ("   -i --interface eth0|eth1|enp0....")
      print ("   -d --direction tx|rx")
      print ("   -r --rate uint:KByteps")
      print ("   -f --flag interface flag info")
      print ("   for example:")
      print ("   %s -i eth0 -d tx -r 20 -f server1_if1" %(argv[0]))
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print ("%s -i <interface> -d <rx|tx> -r <rate> -f <flag_info>" %(argv[0]))
         print ("   -i --interface eth0|eth1|enp0....")
         print ("   -d --direction tx|rx")
         print ("   -r --rate uint:KByteps")
         print ("   -f --flag interface flag info")
         print ("   for example:")
         print ("   %s -i eth0 -d tx -r 20 -f server1_if1" %(argv[0]))
         sys.exit()
      elif opt in ("-i", "--interface"):
         interface = arg
      elif opt in ("-d", "--direction"):
         direction = arg
      elif opt in ("-r", "--rate"):
         rate = arg
      elif opt in ("-f", "--flag"):
         info = arg

   if ( interface == '' or direction == '' or rate == '' or info == ''):
      print ("%s -i <interface> -d <rx|tx> -r <rate> -f <flag_info>" %(argv[0]))
      print ("   -i --interface eth0|eth1|enp0....")
      print ("   -d --direction tx|rx")
      print ("   -r --rate uint:KByteps")
      print ("   -f --flag interface flag info")
      print ("   for example:")
      print ("   %s -i eth0 -d tx -r 20 -f server1_if1" %(argv[0]))
      sys.exit(2)
   
   return (interface, direction, int(rate), info)   


netcard = '/proc/net/dev'
def get_net_data(interface, direction):
    nc = netcard or '/proc/net/dev'
    fd = open(nc, "r")
    netcardstatus = False
    for line in fd.readlines():
        if line.find(interface) >= 0 :
            netcardstatus = True
            field = line.split()
            if direction.lower() == 'rx':
               Bytes = field[0].split(":")[1]
               Bytes = Bytes or field[1]
            elif direction.lower() == 'tx':
               Bytes = field[8]
            else:
               fd.close()
               sys.exit()
    if not netcardstatus:
        fd.close()
        print("net interface name error!")
        sys.exit()

    fd.close()
    return float(Bytes)


def send_simple_message(message):
    return requests.post(
        "https://api.mailgun.net/v3/mg.oncompress.com/messages",
        auth=("api", "key-6c77894e772490822d55b4e4b6651894"),
        data={"from": "Admin <postmaster@mg.oncompress.com>",
              "to": ["shan275@qq.com"],
              "subject": "监控报警",
              "text": message})

def net_loop(interface, direction, rate, info):
    Bytes =  get_net_data(interface,direction)
    time.sleep(60)
    new_Bytes = get_net_data(interface, direction)
    Bytes_data = (new_Bytes - Bytes)/1024
    print ("%s data: %s kB" %(direction, Bytes_data))
    real_rate = int(Bytes_data/60);
    if real_rate < rate :
       message = "流量告警! %s %s %s方向一分钟流量警告,检查实时平均速率: %s KBps, 设置速率门阀值: %s KBps,请各位大佬关注服务器流量状况，发大财！！！"%(time.strftime('%Y-%m-%d %H:%M:%S'),info, direction, real_rate,rate)
       send_simple_message(message)
    else:
       message = "流量信息  %s %s %s方向一分钟流量,检查实时平均速率: %s KBps, 设置速率门阀值: %s KBps,发大财！！！"%(time.strftime('%Y-%m-%d %H:%M:%S'),info, direction, real_rate,rate)
    print  message
    


if __name__ == "__main__":
   interface,direction,rate,info = argv_probe(sys.argv)
   print '接口为    ：', interface
   print '方向为    ：', direction
   print '速率(KBps)：', rate
   print '标示信息  ：', info
   net_loop(interface,direction,rate,info)
