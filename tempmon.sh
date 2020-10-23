#!/bin/bash
printf "%-15s%5s\n" " TIMESTAMP" "TEMP(ºC)"
printf "%20s\n" " ............................"
#while true
#do
	#temp=$(vcgencmd measure_temp | egrep -o '[0-9]*\.[0-9]*')
	temp=$(vcgencmd measure_temp)
	temp=${temp:5:2}
	timestamp=$(date +'20%y/%m/%d-%T')
	printf "$timestamp","$temp"
	
	if [[ $temp -gt 55 ]]
	then
		python fanon.py
		sleep 1m
	#	python fanoff.py
	#else 
	#	python fanoff.py
	fi
	python fanoff.py
	echo "$timestamp,$temp" >> /var/www/html/rtemp.csv
#	sleep 15m
#done
