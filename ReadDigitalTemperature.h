#include <OneWire.h>
 
int ReadDigitalTemperature(int inPin) 
    {
        int HighByte, LowByte, TReading, SignBit, Tc_100, Whole;
        byte i;
        byte present = 0;
        byte data[12];
        byte addr[8];
        OneWire ds(inPin);
  
       if ( !ds.search(addr)) {
           ds.reset_search();
           return -1;
       }
 
       ds.reset();
       ds.select(addr);
       ds.write(0x44,1); 
  
       present = ds.reset();
       ds.select(addr);    
       ds.write(0xBE);  
 
       for ( i = 0; i < 9; i++) { 
           data[i] = ds.read();
       }
       
       LowByte = data[0];
       HighByte = data[1];
       TReading = (HighByte << 8) + LowByte;
       SignBit = TReading & 0x8000;  
       
       if (SignBit) {
           TReading = (TReading ^ 0xffff) + 1;
       }
       
       Tc_100 = (6 * TReading) + TReading / 4; 
       Whole = Tc_100 / 100; 

       if (SignBit)
           return -Whole;else
           return  Whole;
    }
