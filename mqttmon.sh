#!/bin/bash
echo mosquitto subscribed to home/hum/1/tx
mosquitto_sub -t 'home/hum/1/tx' > /var/www/html/hum1.csv &
echo mosquitto subscribed to home/temp/1/tx
mosquitto_sub -t 'home/temp/1/tx' > /var/www/html/outtemp1.csv &
echo mosquitto subscribed to home/watertemp/1/tx 
mosquitto_sub -t 'home/watertemp/1/tx' > /var/www/html/watertemp1.csv &

echo press q to stop
sleep 15
while true
do
	#hum=$(cat /var/www/html/hum1.csv)
	hum=$(tail -1 /var/www/html/hum1.csv)
	hum=${hum:0:2}
	timestamp=$(date +'20%y/%m/%d-%T')
	#timestamp2=$(date +'%y/%m/%d-%T')
	printf "$timestamp","$hum\n"
	echo "$timestamp,$hum" >> /var/www/html/hum.csv
	
	#temp=$(cat /var/www/html/outtemp1.csv)
	temp=$(tail -1 /var/www/html/outtemp1.csv)
	temp=${temp:0:2}
	printf "$timestamp","$temp\n"
	echo "$timestamp,$temp" >> /var/www/html/outtemp.csv
	
	#wtemp=$(cat /var/www/html/watertemp1.csv)
	wtemp=$(tail -1 /var/www/html/watertemp1.csv)
	wtemp=${wtemp:0:4}
	printf "$timestamp","$wtemp\n"
	echo "$timestamp,$wtemp" >> /var/www/html/watertemp.csv
	
	echo "$timestamp,$temp,$hum,$wtemp" >> /var/www/html/mqttstats.csv
		
	if [[ $input = "q" ]] || [[ $input = "Q" ]]; then
        echo end of script
        kill -9 $(pidof mosquitto_sub)
        break 
    fi
    
    sleep 15m
done

