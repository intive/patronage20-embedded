#include <Servo.h>

#define PIN 15

Servo servo;

int servoAng;

void setup() {
  servo.attach(PIN);
  servo.write(10);
  Serial.begin(9600);
}


void loop() 
{ 
    if (Serial.available() > 1) {
        servoAng = Serial.parseInt();
        servo.write(servoAng);
    }
} 
