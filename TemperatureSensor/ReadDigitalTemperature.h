#ifndef READDIGITALTEMPERATURE_H
#define READDIGITALTEMPERATURE_H

#define START_CONVERSATION 0x44
#define READ_SCRATCHPAD 0xBE

/* Function for reading digital temperature */

#include <OneWire.h>
 
int read_digital_temperature(int in_pin) 
{
    int high_byte, low_byte, t_reading, temperature_in_celsius;
    byte i;
    byte present = 0;
    byte data[12];
    byte addr[8];
    OneWire ds(in_pin);
  
    if (!ds.search(addr)) {
        ds.reset_search();
        /*didn't find any sensor, return value under absolute zero */
        return -300;
    }
 
    ds.reset();
    ds.select(addr);
    /* Start conversion, with parasite power on at the end */
    ds.write(START_CONVERSATION, 1); 
    present = ds.reset();
    ds.select(addr);    
    /* Setup for reading */ 
    ds.write(READ_SCRATCHPAD); 
 
    for (i = 0; i < 9; i++) { 
        data[i] = ds.read();
    }
       
    low_byte = data[0];
    high_byte = data[1];
    t_reading = (high_byte << 8) + low_byte;
    
    temperature_in_celsius = t_reading >> 4;

    /* Return temperature in tens of degrees of Celsius */
    return temperature_in_celsius * 10;
}
#endif /* READDIGITALTEMPERATURE_H */
