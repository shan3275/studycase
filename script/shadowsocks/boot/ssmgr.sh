setsid ss-manager -m aes-256-cfb -u --manager-address 127.0.0.1:4000 &
setsid ssmgr -c ~/.ssmgr/ss.yml &
setsid ssmgr -c ~/.ssmgr/webgui.yml &
