#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <time.h>
#include "DHT.h"
//#include <Arduino.h>
#include </home/rpires/Arduino/dht11/Hum_Temp__DS18B20_mqtt/DS18B20.h>

// Update these with values suitable for your network.
const char* ssid = "ESCRITORIO";
const char* password = "g0mesp1res";
const char* mqtt_server = "192.168.1.201";
#define mqtt_port 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_PUBLISH1 "home/watertemp/1/tx"
#define MQTT_RECEIVER "home/watertemp/1/rx"
#define MQTT_PUBLISH2 "home/humtemp/1/tx"
#define MQTT_PUBLISH3 "home/hum/1/tx"
#define MQTT_PUBLISH4 "home/temp/1/tx"

//float t = 0;
DS18B20 myDS18B20(2); //ESP8266 pin D4, GPIO2
char* hour_and_date; 
#define DHTPIN 4    //Fisicamente é o pino 2 da board // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

int period = 10000;
int i = 0;
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
    // The function randomSeed(seed) resets Arduino’s pseudorandom number generator.
    //randomSeed(micros());
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
  dht.begin();
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

  
void publishSerialData3(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_PUBLISH3, serialData);
}
  
void publishSerialData4(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_PUBLISH4, serialData);
}


void readsensors(){
  String dataString;
  int h = dht.readHumidity();
  int t2 = dht.readTemperature();
  float t = myDS18B20.Get_temp();
  char copy[3];

  //DHT11 data collection
  // Check if any reads failed and exit early (to try again).
  while (isnan(h) || isnan(t)){
    h = dht.readHumidity();
    t2 = dht.readTemperature();
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(500);
  }

  //HUMIDITY
  //Convert h to a datastring
  dataString = String(h);
  Serial.print("Humidity ");
  Serial.println(dataString);
  dataString.toCharArray(copy, 32);
  publishSerialData3(copy);
  
  //TEMPERATURE
  //when the temperature is between 0 an 9, I want a trailing 0
  if ((t2 < 10)&&(t2>=0)) dataString = String("0")+","+t2;
  else dataString = String(t2);
  Serial.print("temperature ");
  Serial.println(dataString);
  dataString.toCharArray(copy, 32);
  publishSerialData4(copy);

  //WATER TEMP
  //transform the data into a string and send it
  dataString = String(t);
  //SEND DATA TO MQTT 
  dataString.toCharArray(copy, 32);
  publishSerialData1(copy);
  Serial.print("water temperature ");
  Serial.println(dataString);
    
}
  

void loop() {
  client.loop();

  if(millis() >= time_now + period){
        time_now += period;
        readsensors();
        i++;
        Serial.print("i ");
        Serial.println(i);
        if (i>100){
          reconnect();
          i=0;
        }
    }
  
  

}
