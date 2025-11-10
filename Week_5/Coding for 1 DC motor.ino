//This is the code to control 1 DC motor only applicable using the motor A pin
const int ENA = 10; // Control speed of motor A
const int INA = 12; // Control direction of motor A

void setup() 
{
  pinMode(ENA, OUTPUT);
  pinMode(INA, OUTPUT);
}

void loop() 
{
  digitalWrite(INA, HIGH); //Move forward
  analogWrite(ENA, 255);  
  delay(3000);
  digitalWrite(INA, LOW); //Move backward
  analogWrite(ENA, 255);
  delay(3000);
}
