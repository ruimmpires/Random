/*
 * Use the `SevSeg` library to display an increasing number on
 * our four-digit-display.
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "SevSeg.h"
SevSeg sevseg; //Instantiate a seven segment controller object

//This version shows only the water temp, using the library sevseg

// Update these with values suitable for your network.
const char* ssid = "MEO";
const char* password = "g0mesp1res";
const char* mqtt_server = "192.168.1.201";
#define mqtt_port 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_PUBLISH "home/humtemp/display"
#define MQTT_RECEIVER "home/watertemp/1/tx"
float number = 0;
char* hour_and_date;

int period = 360000;
unsigned long time_now = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup_wifi() {
    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA); //added 6Oct2022 because thus was behaving as an AP
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    randomSeed(micros());
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
      publishSerialData(hour_and_date);
      publishSerialData("home/humtemp/display is online");
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
    payload[length]=0;
    number = atof((char*)(payload));
}


void publishSerialData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_PUBLISH, serialData);
}

void setup()
{
   byte numDigits = 4;

   //
   // In the table above see how these refer to digits.
   //
   byte digitPins[] = {13,15,9,10};

   //
   // In the table above see how these refer to the segments,
   // in alphabetical order :)
   //
   byte segmentPins[] = {16,5,4,0,2,14,12,3};

   bool resistorsOnSegments = false;
   byte hardwareConfig = COMMON_CATHODE;

   sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments);
   sevseg.setBrightness(30);

   //Serial.begin(115200);
   //Serial.setTimeout(500);// Set time out for 
   setup_wifi();
   client.setServer(mqtt_server, mqtt_port);
   client.setCallback(callback);
   reconnect();
}


//
// The number we're displaying
//
float num=0;

void loop()
{
 
  client.loop();
  sevseg.setNumber(number*100,2);
  sevseg.refreshDisplay();  // Must run repeatedly; don't use blocking code (ex: delay()) in the loop() function or this won't work right

    if(millis() >= time_now + period){
        time_now += period;
        reconnect();
    }
  
}
