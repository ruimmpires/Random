# inserts mqtt data that is stored in a csv file to the database
# Only the last line

import os
import csv
file = '/var/www/html/mqttstats.csv'

#with open('/var/www/html/mqttstats.csv', 'rb') as f:
#    try:  # catch OSError in case of a one line file 
#        f.seek(-2, os.SEEK_END)
#        while f.read(1) != b'\n':
#            f.seek(-2, os.SEEK_CUR)
#    except OSError:
#        f.seek(0)
#    last_line = f.readline().decode()
#print('last csv line: ',last_line)

with open(file) as f:
    try: 
        reader = csv.reader(f)
        data = list(reader)
    except IOError:
        print('File error')
last_line_list=(data[len(data) -1])
#print(last_line_list)
new_last_line_list=[last_line_list[0],last_line_list[1],last_line_list[2],last_line_list[3],last_line_list[4],'20']
print(new_last_line_list)


import mysql.connector
try:
    mydb = mysql.connector.connect(
        host="localhost",
        user="root",
        password="******",
        database="mqtt"
    )
    #print(mydb)
    mycursor = mydb.cursor()
except mysql.connector.Error as err:
    print(err)
    print("Error Code:", err.errno)
    print("SQLSTATE", err.sqlstate)
    print("Message", err.msg)



sql = "INSERT INTO sensor (date, outtemp, hum, wtemp, power, intemp) VALUES (%s, %s, %s, %s, %s, %s)"
val = (new_last_line_list)
try: 
    mycursor.execute(sql, val)
    mydb.commit()
    mydb.close()
except mysql.connector.Error as err:
    print(err)
    print("Error Code:", err.errno)
    print("SQLSTATE", err.sqlstate)
    print("Message", err.msg)

    
