#include "HUSKYLENS.h"
#include <Wire.h>

HUSKYLENS huskylens;

// --- RGB LED PIN DEFINITIONS (ESP32) ---
const int RED_PIN   = 15;
const int GREEN_PIN = 2;
const int BLUE_PIN  = 4;

void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void printResult(HUSKYLENSResult result);

void setup() {
    Serial.begin(115200);
    
    // Initialize RGB Pins
    pinMode(RED_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
    
    // Turn off LED initially
    setColor(0, 0, 0); 

    Wire.begin(21, 22);
    while (!huskylens.begin(Wire)) {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(700);
    }
}

void loop() {
    if (!huskylens.request()) {
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    }
    else if(!huskylens.isLearned()) {
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
        setColor(0, 0, 0); // Turn off if nothing is learned
    }
    else if(!huskylens.available()) {
        Serial.println(F("No block or arrow appears on the screen!"));
        setColor(0, 0, 0); // Turn off if nothing is detected
    }
    else {
        Serial.println(F("###########"));
        while (huskylens.available()) {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);

            // --- COLOR LOGIC BASED ON ID ---
            if (result.command == COMMAND_RETURN_BLOCK || result.command == COMMAND_RETURN_ARROW) {
                switch (result.ID) {
                    case 1:
                        // ID 1 detected -> RED
                        Serial.println(F(">> ID 1 Detected: Red Color"));
                        setColor(255, 0, 0);
                        break;
                    
                    case 2:
                        // ID 2 detected -> GREEN
                        Serial.println(F(">> ID 2 Detected: Green Color"));
                        setColor(0, 255, 0);
                        break;

                    case 3:
                        // ID 3 detected -> BLUE
                        Serial.println(F(">> ID 3 Detected: Blue Color"));
                        setColor(0, 0, 255);
                        break;
                }
            }
        }    
    }
}

void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}