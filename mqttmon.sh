#!/bin/bash
echo mosquitto subscribed to home/hum/1/tx
mosquitto_sub -t 'home/hum/1/tx' > /var/www/html/hum1.csv &
echo mosquitto subscribed to home/temp/1/tx
mosquitto_sub -t 'home/temp/1/tx' > /var/www/html/outtemp1.csv &
echo mosquitto subscribed to home/watertemp/1/tx 
mosquitto_sub -t 'home/watertemp/1/tx' > /var/www/html/watertemp1.csv &

hum=$(tail -1 /var/www/html/hum1.csv)
hum=${hum:0:2}
timestamp=$(date +'20%y/%m/%d-%T')

printf "$timestamp","$hum\n"
echo "$timestamp,$hum" >> /var/www/html/hum.csv


temp=$(tail -1 /var/www/html/outtemp1.csv)
#the below command failed when the temperature was below 10
#(( temp=$temp+0 ))
if [ $((temp)) -lt 10 ]
then
        temp=${temp:2:3}
        #temp=${temp:0:2}
        echo $temp
fi
printf "$timestamp","$temp\n"
echo "$timestamp,$temp" >> /var/www/html/outtemp.csv


wtemp=$(tail -1 /var/www/html/watertemp1.csv)
wtemp=${wtemp:0:4}
printf "$timestamp","$wtemp\n"
echo "$timestamp,$wtemp" >> /var/www/html/watertemp.csv

power=$(tail -1 /var/www/html/power1.csv)
printf "$timestamp","$power\n"
echo "$timestamp,$power" >> /var/www/html/power.csv

potential=$(tail -1 /var/www/html/potential1.csv)
printf "$timestamp","$potential\n"
echo "$timestamp,$potential" >> /var/www/html/potential.csv

echo "$timestamp,$temp,$hum,$wtemp,$power,$potential" >> /var/www/html/mqttstats.csv


kill -9 $(pidof mosquitto_sub)
