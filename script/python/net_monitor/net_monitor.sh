#!/bin/sh
date >> /root/net_monitor/monitor_eth0.log
python /root/net_monitor/net_monitor.py -i eth0 -d tx -r 700 -f "SC 98-Server 发送端口eth0"   &>> /root/net_monitor/monitor_eth0.log
date >> /root/net_monitor/monitor_eth2.log
python /root/net_monitor/net_monitor.py -i eth2 -d rx -r 6500 -f "SC 98-Server 接收端口eth2"   &>> /root/net_monitor/monitor_eth2.log
date >> /root/net_monitor/monitor_eth3.log
python /root/net_monitor/net_monitor.py -i eth3 -d rx -r 6500 -f "SC 98-Server 接收端口eth3"   &>> /root/net_monitor/monitor_eth3.log
python /root/net_monitor/disk_monitor.py  -f "SC 98-Server"  &>> /root/net_monitor/disk_monitor.log
