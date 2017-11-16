#!/bin/sh
date >> /root/net_monitor/monitor_eth0.log
python /root/net_monitor/net_monitor.py -i eth0 -d tx -r 40   -f "GZ 247-Server 发送端口eth0"   &>> /root/net_monitor/monitor_eth0.log
date >> /root/net_monitor/monitor_eth2.log
python /root/net_monitor/net_monitor.py -i eth2 -d rx -r 3700 -f "GZ 247-Server 接收端口eth2"   &>> /root/net_monitor/monitor_eth2.log
