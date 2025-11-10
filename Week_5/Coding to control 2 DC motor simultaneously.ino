const int ENA = 10; // Control speed of motor A
const int ENB = 11; // Control speed of motor B
const int INA = 12; // Control direction of motor A
const int INB = 13; // Contorl direction of motor B

void setup() 
{
  pinMode(ENA, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() 
{
  Motor('A', 'F', 255); //Control motor A
  delay(5000);
  Motor('B', 'F', 255); //Control motor B
  delay(5000);
  Motor('C', 'F', 255); //Control both motor simultaneously
  delay(5000);
  Motor('C', 'B', 255);
  delay(5000);
  Motor('C', 'B', 0); //soft stoping both motor simultaneously
  delay(5000);
}

void Motor(char mot, char direction, int speed)
{
  switch (mot)
  {
    case 'A':
    if(direction == 'F') //Forward
    {
      digitalWrite(INA, HIGH);
    }
    else if(direction == 'B') //Backward
    {
      digitalWrite(INA, LOW);
    }
    analogWrite(ENA, speed);
    break;

    case 'B':
    if(direction == 'F')
    {
      digitalWrite(INB, HIGH);
    }
    else if(direction == 'B')
    {
      digitalWrite(INB, LOW);
    }
    analogWrite(ENB, speed);
    break;

    case 'C':
     if(direction == 'F')
    {
      digitalWrite(INA, HIGH);
      digitalWrite(INB, HIGH);
    }
    else if(direction == 'B')
    {
      digitalWrite(INA, LOW);
      digitalWrite(INB, LOW);
    }
    analogWrite(ENA, speed);
    analogWrite(ENB, speed);
    break;
  }
}
