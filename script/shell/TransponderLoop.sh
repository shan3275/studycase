# Program:
#       This program is used to Monitor Transponder in Wenzhou for FS8000.
# History:
# 2012/09/27         shan      First release
while [ true ]
do
        transponder_stat=$(dtweak msa -m 0 -a 40 -c 81 -l 0 -r 2 |cut -c68-69)
        sleep 1
        transponder_stat=$(dtweak msa -m 0 -a 40 -c 81 -l 0 -r 2 |cut -c68-69)
        #echo "up transponder stat $transponder_stat"
	if [ -n "$transponder_stat" ]; then
        	case $transponder_stat in
        	ff)
                	#echo -e "up transponder ok"
                	;;
        	*)
		date >> /root/transponder.log        
		echo "up transponder $transponder_stat" >> /root/transponder.log 
        	echo "reset up transponder" >> /root/transponder.log 
        	dtweak cpld reg 0 0x9 feff
        	sleep 1
        	dtweak cpld reg 0 0x9 ffff
        	;;
        	esac
	fi


        ## echeck dn transponder 
        transponder_stat=$(dtweak msa -m 1 -a 40 -c 81 -l 0 -r 2 |cut -c68-69)
        sleep 1
        transponder_stat=$(dtweak msa -m 1 -a 40 -c 81 -l 0 -r 2 |cut -c68-69)         
        #echo "dn transponder stat $transponder_stat"
	if [ -n "$transponder_stat" ]; then
        	case $transponder_stat in
        	ff)
                	#echo -e "dn transponder ok"
                	;;
        	*)
		date >> /root/transponder.log        
		echo "dn transponder $transponder_stat" >> /root/transponder.log 
        	echo "reset dn transponder" >> /root/transponder.log 
        	dtweak cpld reg 0 0xa 1f7
        	sleep 1
        	dtweak cpld reg 0 0xa 1ff
        	;;
        	esac
	fi

        sleep 4
done

