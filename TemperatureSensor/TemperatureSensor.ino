/* Demonstration how to use read_analog_temperature and read_digital_temperature function */

#include "getTemperature.h"
#include "esp_pins.h"

int digital_pin = D2; 
int analog_pin = A0; 

void setup() 
{
    Serial.begin(9600); 
    while (!Serial) {    
    }

    pinMode(digital_pin, INPUT);
    pinMode(D8, INPUT);
}

void loop() 
{
    Serial.print("Digital temperature: ");
    Serial.println(read_digital_temperature(digital_pin));
    Serial.print("Analog temperature: ");
    Serial.println(read_analog_temperature(analog_pin));
    if(digitalRead(D8)==HIGH )Serial.println("high");
    delay(2000);
}
