#ifndef READANALOGTEMPERATURE_H
#define READANALOGTEMPERATURE_H

/* Coefficients Steinhart–Hart equation */
#define a_coefficient 0.002733685404044381f
#define b_coefficient 0.00000845292513839221f
#define c_coefficient 0.0000011238524235783645f

/* The function calculates the temperature from analog input, uses a Steinhart–Hart equation */

#include <math.h>

int read_analog_temperature(int in_pin)
{
    float resistance, resistance_cube, voltage_output;
    int temperature;

    /* Calculate resistance on thermistor, assumption, 10k Ohm resistor on board
       A voltage divider was used in the equation, the output voltage is measured between the resistor and ground
    */
    voltage_output = (float)analogRead(in_pin);
    resistance = (10240000.f / voltage_output - 10000.f);

    resistance = logf(resistance);
    resistance_cube = resistance * resistance * resistance;

    /* Calculating resistance, Steinhart–Hart equation */
    resistance = 1.f / (a_coefficient + b_coefficient * resistance + c_coefficient * resistance_cube);

    /* Convert Kelvin to Celcius in tens of degrees of Celsius */
    temperature = (resistance - 273.15f) * 10.f;

    /* Return value of the temperature */
    return temperature;
}
#endif /* READANALOGTEMPERATURE_H */
