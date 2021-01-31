#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance


// Update these with values suitable for your network.
const char* ssid = "MEO";                         //your WiFI SSID
const char* password = "*******";                 //Your WiFi password
const char* mqtt_server = "192.168.1.201";        //Your MQTT broker IP
#define mqtt_port 1883
#define MQTT_USER ""                              //Fill in case your MQTT broker has user
#define MQTT_PASSWORD ""                          //Fill in case your MQTT broker has user
#define MQTT_PUBLISH1 "home/clamp/1"
#define MQTT_RECEIVER "home/powerMeter/1"
#define MQTT_PUBLISH "home/factor/1"

//float t = 0;
char* hour_and_date; 
int pfPin = 5;       // pin D1, GPIO5
//CLAMP in A0; //ADC pin for the clamp


int period = 10000; //delay between samples in ms
int i = 0;
int TimeOut = 1000; //timeout until the connection to the broker is done. 
//This is the number of loops. If period = 10000 (10s), then the timeout will be 1000*10s = 2h46m
unsigned long time_now = 0;


WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    //Connect to NTP and acquire time
    configTime(1 * 3600, 1, "pool.ntp.org", "time.nist.gov");
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
      Serial.print(".");
      delay(500);
    }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      time_t now = time(nullptr);
      hour_and_date = ctime(&now);
      publishSerialData1(hour_and_date);
      publishSerialData1("home/watertemp/1 is online");
      // ... and resubscribe
      client.subscribe(MQTT_RECEIVER);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    Serial.print("data:");  
    Serial.write(payload, length);
    Serial.println();
    payload[length]=0;
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);
  emon1.current(A0, 97);             // Current: input pin, calibration.
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
}

void publishSerialData1(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_PUBLISH1, serialData);
}

  
void publishSerialData2(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_PUBLISH, serialData);
}
  


void readsensors(){
  String dataString;
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only

  //Serial.print(Irms*230.0);         // Apparent power
  //Serial.print(" ");
  //Serial.println(Irms);          // Irms
 
  //int h = dht.readHumidity();
  //int t2 = dht.readTemperature();
  //int clamp = analogRead(clampPin);
  //int pf = digitalRead(pfPin);
  char copy[3];
   

  //CLAMP
  //transform the data into a string and send it
  dataString = String(Irms*230.0);
  //SEND DATA TO MQTT as VA
  dataString.toCharArray(copy, 32);
  publishSerialData1(copy);

  //dataString = String(ca)+","+String(pf);
  Serial.println(dataString);
}
  

void loop() {
  client.loop();
  
  if(millis() >= time_now + period){
        time_now += period;
        readsensors();
        i++;
        //Serial.print("i ");
        //Serial.println(i);
        if (i>TimeOut){
          reconnect();
          i=0;
        }
    }
  
  

}
