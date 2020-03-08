#include <math.h>

int ReadAnalogTemperature (int inPin) {
 double Temp;
 Temp = log (((10240000/analogRead (inPin)) - 10000));
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp)) * Temp);
 Temp = Temp - 273.15; // Convert Kelvin to Celcius
return int (Temp);
}
/*
int ReadAnalogTemperature (int inPin) {
  double V_NTC = (double)analogRead (inPin) / 1024;
  double R_NTC = (150000 * V_NTC) / (3.3 - V_NTC);
  R_NTC = log(R_NTC);
  double Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * R_NTC * R_NTC ))* R_NTC );
  Temp = Temp - 273.15; 
  return int (Temp);
}*/
