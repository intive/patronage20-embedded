/* Function for reading digital temperature */

#include <OneWire.h>
 
int read_digital_temperature(int in_pin) 
{
    int high_byte, low_byte, t_reading, sign_bit, tc_100, whole;
    byte i;
    byte present = 0;
    byte data[12];
    byte addr[8];
    OneWire ds(in_pin);
  
    if (!ds.search(addr)) {
        ds.reset_search();
        return -1;
    }
 
    ds.reset();
    ds.select(addr);
    ds.write(0x44,1); 
  
    present = ds.reset();
    ds.select(addr);    
    ds.write(0xBE);  
 
    for (i = 0; i < 9; i++) { 
        data[i] = ds.read();
    }
       
    low_byte = data[0];
    high_byte = data[1];
    t_reading = (high_byte << 8) + low_byte;
    sign_bit = t_reading & 0x8000;  
       
    if (sign_bit) {
        t_reading = (t_reading ^ 0xffff) + 1;
    }
       
    tc_100 = (6 * t_reading) + t_reading / 4; 
    whole = tc_100 / 100; 

    if (sign_bit) {
        return -whole;
    } else {
        return  whole;
    }
}
