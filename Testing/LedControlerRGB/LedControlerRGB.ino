#include <ArduinoJson.h>
#include "HSVtoRGB.h"
#include "pins_arduino.h"

String JsonOutput = "";

int h,s,v;

bool SetLED_RGB_Value(int redPIN, int greenPIN, int bluePIN, String JsonInput)
    {
        int RGB[3];
        uint32_t  hue, saturation, value;  
        StaticJsonDocument<200> doc;

        DeserializationError error = deserializeJson(doc, JsonInput);

        if (error)
            return false;
            
        hue = doc["hue"];
        saturation = doc["saturation"];
        value = doc["value"];

        Serial.print("LED H S V "); Serial.print(hue);Serial.print(saturation);Serial.println(value);
        HSVtoRGB(hue, double (saturation)/double(100), double (value)/double(100), RGB);
        Serial.print("R: "); Serial.println(RGB[0]);
         Serial.print("G: "); Serial.println(RGB[1]);
          Serial.print("B: "); Serial.println(RGB[2]);
        analogWrite(redPIN, RGB[0]);        //set PWM value for red
        analogWrite(greenPIN, RGB[1]);     //set PWM value for blue
        analogWrite(bluePIN,RGB[2]);       //set PWM value for green

        return true;
    }
void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(10));
  pinMode(D3, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);

}

// the loop function runs over and over again forever
void loop() {
  StaticJsonDocument<200> root;
  JsonOutput = "";
  root["hue"] = random(0,360);
  root["saturation"] = random(0,100);
  root["value"] = random(0,100);

  serializeJson(root, JsonOutput);
  SetLED_RGB_Value(D3, D2, D4, JsonOutput);
  delay(100);  
  }// wait for a second
