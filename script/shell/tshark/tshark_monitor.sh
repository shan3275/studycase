procID=`pgrep tshark`
if [ "" == "$procID" ];
then
	/home/shan/tshark/tshark_start.sh
fi
