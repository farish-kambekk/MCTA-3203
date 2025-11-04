#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int ACCEL_THRESHOLD = 16400;  
const int GYRO_THRESHOLD = 1700;
int ax,ay,az,gx,gy,gz;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
}

void loop() 
{
  mpu.getAcceleration(&ax, &ay, &az);
  int gesture = detectGesture(); 
  Serial.print(ax); 
  Serial.print(",");
  Serial.print(ay); 
  Serial.print(",");
  Serial.print(az);
  Serial.print(",");
  Serial.println(gesture);
  delay(50);
}

int detectGesture() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  long accel_magnitude_sq = (long)ax*ax + (long)ay*ay + (long)az*az;
  long gyro_magnitude_sq = (long)gx*gx + (long)gy*gy + (long)gz*gz;

  if (accel_magnitude_sq > (long)ACCEL_THRESHOLD * ACCEL_THRESHOLD && gyro_magnitude_sq > (long)GYRO_THRESHOLD * GYRO_THRESHOLD)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}