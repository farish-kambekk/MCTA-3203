#include <MPU6050.h>
#include <Servo.h>
#include <Wire.h>
const int REDLED = 7;
const int GREENLED = 8;

MPU6050 mpu;
Servo myservo;

const int ACCEL_THRESHOLD = 16400;  
const int GYRO_THRESHOLD = 1700;

void setup()
{
  Wire.begin();
  mpu.initialize();
  myservo.attach(9);
  myservo.write(0);
  pinMode(REDLED, OUTPUT);
  pinMode(GREENLED,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  digitalWrite(REDLED,LOW);
  digitalWrite(GREENLED,LOW);
  int gesture = detectGesture();
  Serial.println(gesture);
  if(Serial.available() > 0)
  {
    char command = Serial.read();
    if(command == 'a')
    {
      myservo.write(90);
      digitalWrite(GREENLED,HIGH);
      digitalWrite(REDLED,LOW);
      delay(3000);
      myservo.write(0);
    }
    else if(command == 'b')
    {
      myservo.write(0);
      digitalWrite(GREENLED,LOW);
      digitalWrite(REDLED,HIGH);
      delay(3000);
    }
  }
}

int detectGesture() 
{
  int ax,ay,az,gx,gy,gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  long accel_magnitude_sq = (long)ax*ax + (long)ay*ay + (long)az*az;
  long gyro_magnitude_sq = (long)gx*gx + (long)gy*gy + (long)gz*gz;
  if (accel_magnitude_sq > (long)ACCEL_THRESHOLD * ACCEL_THRESHOLD && gyro_magnitude_sq > (long)GYRO_THRESHOLD * GYRO_THRESHOLD)
  {
    return 1; //circle
  }
  else
  {
    return 0;//no circle
  }
}