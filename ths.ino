#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

Servo servo;

// Update these with values suitable for your network.
const char* ssid = "ESCRITORIO";
const char* password = "g0mesp1res";
const char* mqtt_server = "192.168.1.201";
#define mqtt_port 1883
#define MQTT_USER ""
#define MQTT_PASSWORD ""
#define MQTT_PUBLISH "home/ths/1/tx"
#define MQTT_RECEIVER "home/ths/1/rx"

String dataString = "1";

WiFiClient wifiClient;
PubSubClient client(wifiClient);

int angle = 0;
int IRsensor = 4;  // IR sensor PIN D2
int val ; // define numeric variables val


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
      publishSerialData("home/ths/1 is online");
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

void publishSerialData(char *serialData){
  if (!client.connected()) {
    reconnect();
  }
  client.publish(MQTT_PUBLISH, serialData);
}


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(500);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  reconnect();
  servo.attach(5);  // servo control sensor PIN D1
  servo.write(angle);
  pinMode (IRsensor, INPUT) ; // define the obstacle avoidance sensor output interface
}

void loop() 
{ 
  client.loop();
  servo.write(0);               
//  delay(50000);
//  servo.write(170);               
//  delay(1000);
  
  val = digitalRead (IRsensor) ;// digital interface
  if (val == HIGH) // When the obstacle avoidance sensor detects a signal
  {
    servo.write(0);               
    //SEND DATA TO MQTT if there are changes to the previous value
    char copy[3];
    dataString.toCharArray(copy, 32);
    publishSerialData(copy);
    delay(1000);
  }
  Serial.print("val:");
  Serial.println(val);
}
