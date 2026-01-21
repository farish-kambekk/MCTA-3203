#include <HardwareSerial.h>
#include <TinyGPSPlus.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#define I2C_ADDRESS 0x08

// ================= USER SETTINGS =================
const char* ssid     = "Farish's A03";
const char* password = "987654321";

String botToken1 = "8577043634:AAFhyefn-ZJnzDp_CImmEkBLv6t-nCOvc00";                  
String chatID1 = "7223390435";

// ================= PIN MAP =================
#define GPS_RX      16
#define GPS_TX      17
#define SENSOR_SDA  14 // For MPU and LIDAR
#define SENSOR_SCL  15  // For MPU and LIDAR
#define SERVO_PIN   18
#define RELAY_PIN   23
#define LDR_PIN     33

// ================= OBJECTS =================
HardwareSerial gpsSerial(2); 
TinyGPSPlus gps;
Servo nudgeServo;
Adafruit_MPU6050 mpu;
TwoWire I2C_Sensors = TwoWire(1); // Dedicated bus for sensors

unsigned long fallCooldown = 0;
bool mpuAvailable = false;

void setup() 
{
  Serial.begin(115200);

  // 1. Setup I2C Slave for Raspberry Pi (Default pins 21 & 22)
  Wire.begin(I2C_ADDRESS);
  Wire.onReceive(receiveEvent);
  Serial.println("I2C Slave (Pi 5) ready on pins 21/22");

  // 2. Setup I2C Master for Sensors (Pins 14 & 15)
  I2C_Sensors.begin(SENSOR_SDA, SENSOR_SCL, 100000);

  // 3. Initialize Sensors on the Sensor Bus
  if (!mpu.begin(0x68, &I2C_Sensors)) 
  { 
    Serial.println("MPU6050 Not Found!");
    mpuAvailable = false;
  } 
  else 
  {
    Serial.println("MPU6050 Ready!");
    mpuAvailable = true;
  }

  // 4. Other Hardware
  gpsSerial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);
  nudgeServo.attach(SERVO_PIN);
  nudgeServo.write(0);

  // 5. WiFi
  WiFi.begin(ssid, password);
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) 
  {
    delay(500);
    Serial.print(".");
    timeout++;
  }
  if (WiFi.status() == WL_CONNECTED) 
  {
    sendTelegramMessage("System Online: Smart Cane Ready.");
  }
}

void loop() 
{
  // GPS
  while (gpsSerial.available() > 0) 
  { 
    gps.encode(gpsSerial.read());
  }

  // Light Control
  int light = analogRead(LDR_PIN);
  digitalWrite(RELAY_PIN, (light < 700) ? LOW : HIGH); //If LDR detect light below 700 threshold then, it will turn LOW. If not it will stay HIGH

  // LIDAR (Pin 14 and 15)
  static unsigned long lastLidarTime = 0;
  if (millis() - lastLidarTime > 100) 
  {
    lastLidarTime = millis();
    int dist = readTFLuna();
    if (dist > 30 && dist < 80) //distance between 30cm and 80cm
    {
      nudgeServo.write(90); //move 90 degrees
      delay(200);
      nudgeServo.write(0);
    }
  }
  
  // MPU Fall Detection (Pin 14 and 15)
  if (mpuAvailable) 
  {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    float totalAccel = sqrt(pow(a.acceleration.x, 2) + pow(a.acceleration.y, 2) + pow(a.acceleration.z, 2)); 

    if (!isnan(totalAccel) && totalAccel > 30.0 && (millis() - fallCooldown > 5000)) //isnan refers to "Is the total acceleration a valid number" and the millis refers to the time more than 5 sec
    { 
      sendTelegramAlert("FALL DETECTED");
      fallCooldown = millis();
    }
  }
}

void receiveEvent(int howMany) //howMany tells the ESP32 device exactly how many bytes of data were sent by the Rasp pi 5
{
  while (Wire.available()) 
  {
    int command = Wire.read(); // Read as integer
    Serial.print("I2C Command: ");
    Serial.println(command);
    if (command == 1) //receive 1 from rasp pi 5
    {
      sendTelegramAlert("SOS BUTTON PRESSED");
    }
  }
}

// ================= TELEGRAM & SENSORS =================

void sendTelegramAlert(String reason) 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    String message = "🚨 EMERGENCY 🚨\nReason: " + reason;
    if (gps.location.isValid()) 
    {
      message += "\n\n❌ Location:\nhttps://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6); // used to tell the location
    } 
    else 
    {
      message += "\n\n❌ No Fix Location; 
    }
    sendTelegramMessage(message);
  }
}

void sendTelegramMessage(String msg) 
{
  WiFiClientSecure client;
  client.setInsecure();
  HTTPClient https;
  msg.replace(" ", "%20");
  msg.replace("\n", "%0A");

  String url = "https://api.telegram.org/bot" + botToken1 + "/sendMessage?chat_id=" + chatID1 + "&text=" + msg; //used to tell which telegram bot to used
  if (https.begin(client, url)) 
  {
    int httpCode = https.GET();
    https.end();
  }
}

int readTFLuna() //basic example from arduino IDE to read TFluna
{
  I2C_Sensors.beginTransmission(0x10);
  I2C_Sensors.write(0x5A); I2C_Sensors.write(0x05); I2C_Sensors.write(0x00);
  I2C_Sensors.write(0x01); I2C_Sensors.write(0x60);
  I2C_Sensors.endTransmission();
  
  I2C_Sensors.requestFrom(0x10, 9);
  if (I2C_Sensors.available() >= 9) 
  {
    I2C_Sensors.read(); I2C_Sensors.read();
    int dL = I2C_Sensors.read();
    int dH = I2C_Sensors.read();
    while(I2C_Sensors.available()) I2C_Sensors.read(); 
    return (dH * 256) + dL;
  }
  return -1;
}