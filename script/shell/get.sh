killall monitor
killall mdu
rm mdu
scp shan@192.168.40.87:/home/shan/Documents/code/fc5000/v5/mpp/mdu/mdu ./
./mdu -p fcasting -f /etc/mdu/mpp.conf
