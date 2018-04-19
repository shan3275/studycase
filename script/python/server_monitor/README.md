说明：
server_monitro.sh 为系统调用文件，比较简单。
net_monitor.py 为核心文件，读取系统日志，计算速率，和门阀值进行比较，低于门阀值邮件报警。
net_monitor_test.py 测试文件，供调试使用。
disk_monitor.py 为磁盘监控模块，低于门阀制报警。
disk_monitor.py 为磁盘监控测试文件
crontab  为cron定时任务配置文件，10分钟调用一次。

改进建议：
1.日志增加时间；
2.邮件增加时间；
