将该下面内容添加到/etc/crontab 文件中，脚本路径根据实际情况变化
1   0 * * * root /home/shan/tshark/tshark_start.sh
59 23 * * * root /home/shan/tshark/tshark_stop.sh
*/5  * * * * root /home/shan/tshark/tshark_monitor.sh
