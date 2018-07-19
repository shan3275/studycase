procID=`ps -aux |grep ss.yml |grep -v grep`
if [ "" == "$procID" ];
then
	setsid ssmgr -c /home/ubuntu/.ssmgr/ss.yml &
fi

procID=`ps -aux |grep ss-manager |grep -v grep`
if [ "" == "$procID" ];
then
	setsid ss-manager -m aes-256-cfb -u --manager-address 127.0.0.1:4000 &
fi

procID=`ps -aux |grep webgui.yml |grep -v grep`
if [ "" == "$procID" ];
then
	setsid ssmgr -c /home/ubuntu/.ssmgr/webgui.yml &
fi
