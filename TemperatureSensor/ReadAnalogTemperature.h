#ifndef READANALOGTEMPERATURE_H
#define READANALOGTEMPERATURE_H

/* Coefficients Steinhart–Hart equation */
#define a 0.002733685404044381
#define b 0.00000845292513839221
#define c 0.0000011238524235783645

/* The function calculates the temperature from analog input, uses a Steinhart–Hart equation */

#include <math.h>

int read_analog_temperature(int in_pin) 
{
    float resistance, resistance_cube;
    int temperature;
    
    /* Calculate resistance on thermistor, assumption, 10,000 ohm thermistor on board */
    resistance = (10240000 / (float)analogRead(in_pin) - 10000); 
     
    resistance = log(resistance);
    resistance_cube = resistance * resistance * resistance;
    
    /* Calculating resistance, Steinhart–Hart equation */
    resistance = 1 / (a + b * resistance + c * resistance_cube);
    
    /* Convert Kelvin to Celcius */
    temperature = resistance - 273.15; 

    /* Return calibrated temperature value */
    return temperature; 
}
#endif /* READANALOGTEMPERATURE_H */
