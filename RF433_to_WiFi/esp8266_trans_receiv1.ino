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

botao ewelink  14557224 / 24bit Protocol: 1   abrir portao rua
11-01-11-10-00-10-00-00-00-10-1000


Comando COATI
21 / 24bit Protocol: 1
16404 / 24bit Protocol: 1
61653 / 24bit Protocol: 1    4117   28885        abrir portao rua
20501 / 24bit Protocol: 1
20500 / 24bit Protocol: 1


vizinho 16777215 / 24bit Protocol: 6


*/

#include <RCSwitch.h>
RCSwitch mySwitch = RCSwitch();

int i = 0;
int period = 1000; //delay between send and receive
int TimeOut = 10; //timeout until the connection to the broker is done. 
//This is the number of loops. If period = 1000 (1s), then the timeout will be 30*1s = 30s
unsigned long time_now = 0;
//int code1 = 0;


void setup() {
  Serial.begin(115200);
  //pinMode(LED_BUILTIN,OUTPUT);  //board led

  
  // Transmitter is connected to Arduino Pin #0  D3
  mySwitch.enableTransmit(0);  // Optional set pulse length.
  // Receiver is connected to Arduino Pin #4  D2
  mySwitch.enableReceive(4);
  // mySwitch.setPulseLength(320);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
  // mySwitch.setRepeatTransmit(15);


  pinMode(2, OUTPUT);     // Initialize GPIO2 pin as an output, internal LED
}


void transmit_433(int code){
    //Serial.println("clone de comando ewelink 14557224 que devera abrir o portao");
    Serial.print("Command sent: ");
    Serial.println(code);
    mySwitch.send(code, 24);
    digitalWrite(2, LOW);   // Turn the LED on by making the voltage LOW
    /*Serial.println("clone de comando ewelink 61653 que devera abrir o portao");
    mySwitch.send(61653, 24);
    delay(1000);
    Serial.println("clone de comando ewelink 4117 que devera abrir o portao");
    mySwitch.send(4117, 24);
    delay(1000);
    Serial.println("clone de comando ewelink 28885 que devera abrir o portao");
    mySwitch.send(28885, 24);
    delay(1000);*/
    /*
    Serial.println("Sending ON");
    Serial.println("A ON 111111111100010101010100");
    mySwitch.send(16762196, 24);
    delay(2000);
    Serial.println("B ON 111111111101000101010100");
    mySwitch.send(16765268, 24);
    delay(2000);
    Serial.println("C ON 111111111101010001010100");
    mySwitch.send(16766036, 24);
    delay(2000);
    Serial.println("D ON 111111111101010100010100");
    mySwitch.send(16766228, 24);
    delay(2000);
    digitalWrite(LED_BUILTIN,HIGH);
    /*mySwitch.sendTriState("11111FFF0FF0");
    mySwitch.switchOn(1, 1); 
    mySwitch.switchOn(2, 1); 
    mySwitch.switchOn(3, 1); 
    mySwitch.switchOn(4, 1); */
    
    /*
    Serial.println("Sending OFF");
    Serial.println("A OFF 111111111100010101010001");
    mySwitch.send(16762193, 24);
    delay(2000);
    Serial.println("B OFF 111111111101000101010001");
    mySwitch.send(16765265, 24);
    delay(2000);
    Serial.println("C OFF 111111111101010001010001");
    mySwitch.send(16766033, 24);
    delay(2000);
    Serial.println("D OFF 111111111101010100010001");
    mySwitch.send(16766225, 24);
    delay(2000);
    digitalWrite(LED_BUILTIN,LOW);
    /*mySwitch.sendTriState("11111FFF0F0F");
    mySwitch.switchOff(1, 1);
    mySwitch.switchOff(2, 1);
    mySwitch.switchOff(3, 1);
    mySwitch.switchOff(4, 1);*/
}

void receive_433(){
  //Serial.print(".");

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
    }

    mySwitch.resetAvailable();
  }
}
  
void loop() {
  digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  receive_433();
  //delay(100);            // Wait a bit
  
  /*for (int codetx = 0; codetx <= 16777215 ; codetx++) {
    transmit_433(codetx);
    delay(100);            // Wait a bit
    digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  }*/
  /*if(millis() >= time_now + period){
        time_now += period;
        //receive_433();
        i++;
        if (i>TimeOut){
          code1 = 1234;
          transmit_433(code1);
          i=0;
        }
    }*/
}
