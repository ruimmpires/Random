/*
433MHz receiver and transmitter
antenna 173mm
using XKFST as trasnmitter
using XLC-RF as receiver
https://github.com/sui77/rc-switch/wiki
https://www.instructables.com/RF-315433-MHz-Transmitter-receiver-Module-and-Ardu/
https://www.youtube.com/watch?v=KA_YE7AvFn0
https://www.instructables.com/Using-an-ESP8266-to-Control-Mains-Sockets-Using-43/
https://www.liwen.id.au/arduino-rf-codes/
https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
https://github.com/ninjablocks/433Utils/tree/master/RPi_utils
https://lastminuteengineers.com/433mhz-rf-wireless-arduino-tutorial/?utm_content=cmp-true
https://github.com/sui77/rc-switch
https://www.espboards.dev/blog/esp32-433-rmt-ev1527/

LIFEDOM
Decimal: 16762196 (24Bit) Binary: 111111111100010101010100 Tri-State: 111110FFFFF0 PulseLength: 158 microseconds Protocol: 1
Raw data: 4944,456,192,452,196,456,196,456,196,396,16,16,16,208,456,196,460,196,84,20,3984,16,624,16,16,52,24,48,20,376,16,524,456,16,24,16,56,16,128,20,20,16,28,16,64,20,32,132,20,

all 5 bis 11111
A 1111110000
C 1111100100
D 1111100010

A on    111111111100010101010100  16762196
A off   111111111100010101010001  16762193
B on    111111111101000101010100  16765268
B off   111111111101000101010001  16765265
C ON    11-11-11-11-11-01-01-00-01-01.0100  16766036
C OFF   11111111110101000101.0001  16766033
D on    111111111101010100010100  16766228
D off   111111111101010100010001  16766225
all 5 bits 00000
010101010100010101010100

5bits 10000
110101010100010101010100

tristate info
https://sui77.wordpress.com/2011/04/12/163/



Comando COATI
21 / 24bit Protocol: 1
16404 / 24bit Protocol: 1
61653 / 24bit Protocol: 1 
20501 / 24bit Protocol: 1
20500 / 24bit Protocol: 1



*/

#include <RCSwitch.h>
#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <time.h>

RCSwitch mySwitch = RCSwitch();

// Update these with values suitable for your network.
const char* ssid = "MEO";
const char* password = "...";
const char* mqtt_server = "192.168.1.201";
#define mqtt_port 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_PUBLISH "home/rf433_1_tx"
#define MQTT_RECEIVER "home/rf433_1_rx"
char* hour_and_date;
float code_to_be_relayed = 0;


//Inputs and outputs
int i = 0;
int period = 1000; //delay between send and receive
int TimeOut = 10; //timeout until the connection to the broker is done. 
//This is the number of loops. If period = 1000 (1s), then the timeout will be 30*1s = 30s
unsigned long time_now = 0;
//int code1 = 0;


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
      publishSerialData(hour_and_date);
      publishSerialData("home/rf433_1 is online");
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
    //Serial.print("payload length"); 
    //Serial.println(length); 
    payload[length]=0;
    //received = atoi((char*)(payload));   
    code_to_be_relayed = atof((char*)(payload)); 
}


void setup() {
  Serial.begin(115200);
   // Transmitter is connected to Arduino Pin #0  D3
  mySwitch.enableTransmit(0);  // Optional set pulse length.
  // Receiver is connected to Arduino Pin #4  D2
  mySwitch.enableReceive(4);
  // mySwitch.setPulseLength(320);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);
  
  Serial.setTimeout(500);// Set time out for 
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();

  pinMode(2, OUTPUT);     // Initialize GPIO2 pin as an output, internal LED
}


void publishSerialData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_PUBLISH, serialData);
}


void transmit_433(int code){
    Serial.print("Command sent: ");
    Serial.println(code);
    mySwitch.send(code, 24);
    digitalWrite(2, LOW);   // Turn the LED on by making the voltage LOW
}


void receive_433(){
  if (mySwitch.available()) {
    Serial.print("mySwitch.available ");
    int value = mySwitch.getReceivedValue();
    
    if (value == 0) 
    {
      Serial.print("Unknown encoding");
    } 
    else 
    {
      digitalWrite(2, LOW);   // Turn the LED on by making the voltage LOW
      Serial.print("Received ");
      Serial.print( mySwitch.getReceivedValue() );
      Serial.print(" / ");
      Serial.print( mySwitch.getReceivedBitlength() );
      Serial.print("bit ");
      Serial.print("Protocol: ");
      Serial.println( mySwitch.getReceivedProtocol() );

      //SEND DATA TO MQTT if there are changes to the previous value
      char copy[3];
      String dataString = String(mySwitch.getReceivedValue());
      dataString.toCharArray(copy, 32);
      publishSerialData(copy);
    }

    mySwitch.resetAvailable();
  }
}
  
void loop() {
  digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  client.loop();
  receive_433();
  if (code_to_be_relayed != 0 ) {
    transmit_433(code_to_be_relayed);
    code_to_be_relayed = 0;
  }
}
