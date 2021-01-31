int Sensor = 13; // Declaration of the sensor input pin
 
void setup ()
{
  Serial.begin(115200); // Initialization serial output
  pinMode (Sensor, INPUT) ; // Initialization sensor pin
}
 
 
void loop ()
{
  bool val = digitalRead (Sensor) ;
 
  if (val == HIGH) // If a signal is detected,
  {
    Serial.println("No obstacle");
  }
  else
  {
    Serial.println("Obstacle detected");
  }
  //Serial.println("------------------------------------");
  delay(100); // Break of 500ms
}
