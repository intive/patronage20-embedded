#include <math.h>

int ReadAnalogTemperature (int inPin) 
    {
        double Temp;
        Temp = log (((10240000/analogRead (inPin)) - 10000));
        Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
        Temp = Temp - 273.15; // Convert Kelvin to Celcius
        return int (Temp);
    }
