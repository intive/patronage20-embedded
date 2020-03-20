/* Demonstration how to use read_analog_temperature and read_digital_temperature function */

#include "getTemperature.h"

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

int digital_pin = D2; 
int analog_pin = A0; 

void setup() 
{
    Serial.begin(9600); 
    while (!Serial) {    
    }

    //pinMode(analog_pin, INPUT);
    pinMode(digital_pin, INPUT);
}

void loop() 
{
    Serial.print("Digital temperature: ");
    Serial.println(read_digital_temperature(digital_pin));
    Serial.print("Analog temperature: ");
    Serial.println(read_analog_temperature(analog_pin));

    delay(2000);
}
