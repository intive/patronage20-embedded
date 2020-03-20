/* Function for reading analog temperature */

#include <math.h>

int read_analog_temperature(int in_pin) 
{
    double temperature;
    temperature = log (((10240000/analogRead(in_pin)) - 10000));
    temperature = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * temperature * temperature)) * temperature);
    temperature = temperature - 273.15; /* Convert Kelvin to Celcius */
    return int (temperature - 34); /* Return calibrated temperature value */
}
