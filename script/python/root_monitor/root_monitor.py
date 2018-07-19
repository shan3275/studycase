#!/usr/bin/python
#coding=utf-8
import pexpect
import sys,re,time,os
import requests
import getopt
from optparse import OptionParser
import threading
import logging
from logging import handlers

global logger

def argv_probe(argv):
   user      = ''
   host = ''
   passwd = ''
   try:
       opts, args = getopt.getopt(argv[1:],"hu:p:i:",["user=","passwd=","ip=","flag="])
   except getopt.GetoptError:
      print ("%s -u <user> -p <passwd> -i <ip>" %(argv[0]))
      print ("   -u --user")
      print ("   -p --passwd")
      print ("   -i --host ip address")
      print ("   for example:")
      print ("   %s -u root -p 1333 -i 192.168.1.100" %(argv[0]))
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
          print ("%s -u <user> -p <passwd> -i <ip>" %(argv[0]))
          print ("   -u --user")
          print ("   -p --passwd")
          print ("   -i --host ip address")
          print ("   for example:")
          print ("   %s -u root -p 1333 -i 192.168.1.100" %(argv[0]))
          sys.exit()
      elif opt in ("-u", "--user"):
         user = arg
      elif opt in ("-p", "--passwd"):
         passwd = arg
      elif opt in ("-i", "--ip"):
         host = arg

   if ( user == '' or host == '' or passwd == '' ):
      print ("%s -u <user> -p <passwd> -i <ip>" %(argv[0]))
      print ("   -u --user")
      print ("   -p --passwd")
      print ("   -i --host ip address")
      print ("   for example:")
      print ("   %s -u root -p 1333 -i 192.168.1.100" %(argv[0]))
      sys.exit(2)
   return (host, user, passwd)

def send_simple_message(message):
    return requests.post(
        "https://api.mailgun.net/v3/mg.oncompress.com/messages",
        auth=("api", "key-6c77894e772490822d55b4e4b6651894"),
        data={"from": "Admin <postmaster@mg.oncompress.com>",
              "to": ["shan275@qq.com"],
              "subject": "监控报警",
              "text": message})

#SSH连接成功时的命令行交互窗口中前面的提示字符的集合
PROMPT = ['# ','>>> ','> ','\$ ', '~', 'shan']

def send_command(child,cmd):
    #发送一条命令
    child.sendline(cmd)
    #期望有命令行提示字符出现
    child.expect(PROMPT)
    #将之前的内容都输出
    #logger.info(child.before)

def send_sudo_command(child,user, password):
    #发送一条命令
    connStr = 'password for ' + user + ':'
    child.sendline('sudo -s')
    ret = child.expect([pexpect.TIMEOUT,connStr])
    if ret == 1:
        child.sendline(password)
        child.expect(PROMPT)
	logger.info('sudo -s success')
        return True
    else:
        logger.info('sudo -s failed')
        return False

def send_clear_command(child,user,password):
    if send_sudo_command(child,user,password) is True:
        ##恢复crontab文件
	logger.info('开始清除crontab')
        send_command(child,'echo "SHELL=/bin/bash" > /etc/crontab')
        send_command(child,'echo "PATH=/sbin:/bin:/usr/sbin:/usr/bin" >> /etc/crontab')
        send_command(child,'echo "MAILTO=root" >> /etc/crontab')
        send_command(child,'echo "HOME=/" >> /etc/crontab')
        send_command(child,'echo " " >> /etc/crontab')
        send_command(child,'echo "# For details see man 4 crontabs" >> /etc/crontab')
        send_command(child,'echo " " >> /etc/crontab')
        send_command(child,'echo "# Example of job definition:" >> /etc/crontab')
        send_command(child,'echo "# .---------------- minute (0 - 59)" >> /etc/crontab')
        send_command(child,'echo "# |  .------------- hour (0 - 23)"   >> /etc/crontab')
        send_command(child,'echo "# |  |  .---------- day of month (1 - 31)" >> /etc/crontab')
        send_command(child,'echo "# |  |  |  .------- month (1 - 12) OR jan,feb,mar,apr ..." >> /etc/crontab')
        send_command(child,'echo "# |  |  |  |  .---- day of week (0 - 6) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat" >> /etc/crontab')
        send_command(child,'echo "# |  |  |  |  |" >> /etc/crontab')
        send_command(child,'echo "# *  *  *  *  * user-name command to be executed" >> /etc/crontab')
	logger.info('完成清除crontab')
	logger.info('开始清除hosts.allow')
        ##清除hosts.allow文件内容
        send_command(child,'echo "#" > /etc/hosts.allow')
        send_command(child,'echo "# hosts.allow    This file contains access rules which are used to" >> /etc/hosts.allow')
        send_command(child,'echo "#                allow or deny connections to network services that" >> /etc/hosts.allow')
        send_command(child,'echo "#                either use the tcp_wrappers library or that have been" >> /etc/hosts.allow')
        send_command(child,'echo "#                started through a tcp_wrappers-enabled xinetd." >> /etc/hosts.allow')
        send_command(child,'echo "#" >> /etc/hosts.allow')
        send_command(child,'echo "#                See \'man 5 hosts_options\' and \'man 5 hosts_access\'" >> /etc/hosts.allow')
        send_command(child,'echo "#                for information on rule syntax." >> /etc/hosts.allow')
        send_command(child,'echo "#                See \'man tcpd\' for information on tcp_wrappers" >> /etc/hosts.allow')
        send_command(child,'echo "#" >> /etc/hosts.allow')
	logger.info('完成清除hosts.allow')
        ##清除应用1
	logger.info('开始清除应用1')
        send_command(child,'killall -9 dpi2svc')
        send_command(child,'killall -9 ADPIAssist')
        send_command(child,'rm -rf /dev/shm/.cpu0')
	logger.info('完成清除应用1')
        ##清除应用2
	logger.info('开始清除应用2')
        send_command(child,'killall -9 python')
        send_command(child,'killall -9 tcpshark')
        send_command(child,'killall -9 scnat_task')
        send_command(child,'rm -rf /usr/local/none')
        send_command(child,'rm -rf /usr/local/http')
	logger.info('完成清除应用2')
        ##清除应用3
	logger.info('开始清除应用3')
        send_command(child,'killall -9 naga')
        send_command(child,'rm -rf /usr/local/.qx')
        send_command(child,'rm -rf /usr/local/.log')
        send_command(child,'killall -9 round_v2.py')
        send_command(child,'killall -9 upload.py')
        send_command(child,'rm -rf /usr/local/.wb')
	logger.info('完成清除应用3')
        ##关闭网卡混杂模式
        send_command(child,'ifconfig eth3 -promisc')
        send_command(child,'ifconfig eth8 -promisc')
        ##设置管理网口IP为DHCP模式
	logger.info('开始清除网卡配置')
        send_command(child,'echo "TYPE=Ethernet" > /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "BOOTPROTO=dhcp" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "DEFROUTE=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "PEERDNS=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "PEERROUTES=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "IPV4_FAILURE_FATAL=no" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "IPV6INIT=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "IPV6_AUTOCONF=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "IPV6_DEFROUTE=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "IPV6_PEERDNS=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "IPV6_PEERROUTES=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "IPV6_FAILURE_FATAL=no" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "NAME=eth0" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "UUID=0327ea00-9b76-4b87-ba4c-d6cfcabbf488" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "DEVICE=eth0" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "HWADDR=3C:A8:2A:23:FA:58" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
        send_command(child,'echo "ONBOOT=yes" >> /etc/sysconfig/network-scripts/ifcfg-eth0')
	logger.info('完成清除网卡配置')
        ##清楚操作记录
	logger.info('开始清除操作记录')
        #send_command(child,'>~/.bash_history')
        #send_command(child,'history -c')
	logger.info('完成清除操作记录')
        send_command(child,'exit')

def send_who_command(child,host,user,password):
    #发送一条命令
    child.sendline('who')
    ret = child.expect([pexpect.TIMEOUT,'root'])
    if ret == 1:
	logger.info('find root login, alarm')
        send_clear_command(child,user,password)
        send_simple_message(host + ' find root login, alarm')
    else:
        logger.info('not find root login, please releax')

def connect(user,host,password):
        #表示主机已使用一个新的公钥的消息
        ssh_newkey = 'Are you sure you want to continue connecting'
        #connStr = 'ssh ' + user + '@' + host + ' -p 37090'
        connStr = 'ssh ' + user + '@' + host

        #为ssh命令生成一个spawn类的对象
        child = pexpect.spawn(connStr)

        #期望有ssh_newkey字符、提示输入密码的字符出现，否则超时
        ret = child.expect([pexpect.TIMEOUT,ssh_newkey,'[P|p]assword: '])

        #匹配到超时TIMEOUT
        if ret == 0:
	    logger.info('[-] Error Connecting')
            return None

        #匹配到ssh_newkey
        if ret == 1:
            #发送yes回应ssh_newkey并期望提示输入密码的字符出现
            child.sendline('yes')
            ret = child.expect([pexpect.TIMEOUT,'[P|p]assword: '])

        #匹配到超时TIMEOUT
        if ret == 0:
	    logger.info('[-] Error Connecting')
            return None

        #发送mima
        child.sendline(password)
        child.expect(PROMPT)
        return child

def log_init(log_app_name, file_name):
    logger = logging.getLogger(log_app_name)
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    ch = logging.handlers.TimedRotatingFileHandler(
                    filename=file_name,
                    when='midnight',
                    backupCount=3
                    )
    ch.setLevel(logging.DEBUG)
    ch.setFormatter(formatter)
    # add ch to logger
    logger.addHandler(ch)

    #控制台输出
    sh = logging.StreamHandler()
    sh.setFormatter(formatter)
    #logger.addHandler(sh)

    return logger

def fun_timer(child,user,password):
    logger.info('fuc host    : %s', host)
    logger.info('fuc user    : %s', user)
    logger.info('fuc password: %s', password)
    send_who_command(child,user,password)
    timer = threading.Timer(5, fun_timer)
    timer.start()

if __name__ == '__main__':
    logger = log_init('MM-21', 'mm-21.log')
    host,user,password = argv_probe(sys.argv)
    logger.info('host    : %s', host)
    logger.info('user    : %s', user)
    logger.info('password: %s', password)
    child=connect(user,host,password)
    if child is None:
        logger.info('child failed, exit')
        exit()
    #child.logfile = sys.stdout
    child.delaybeforesend = 0
    child.timeout = 10
    #timer = threading.Timer(5, fun_timer, [child,user,password])
    #timer.start()
    send_who_command(child,host,user,password)
    child.close(force=True)
