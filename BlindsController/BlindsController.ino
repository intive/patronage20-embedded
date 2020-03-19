/* BlindsController by Kuba Bartoszewicz */

#include "blinds.h"

/* Defining pins for Stepper controller */
#define IN1 16 
#define IN2 5 
#define IN3 4 
#define IN4 0 

/* Defining pins for switch */
#define SW1 14
#define SW2 12

void setup() 
{ 
    /* Setting stepper pins */
    stepper_pin[0] = IN1;
    stepper_pin[1] = IN2;
    stepper_pin[2] = IN3;
    stepper_pin[3] = IN4;
    
    /* Setting switches pins */
    switches_pin[0] = SW1;
    switches_pin[1] = SW2;
    
    /* Setting pinMode */
    pinMode(IN1, OUTPUT);  
    pinMode(IN2, OUTPUT);  
    pinMode(IN3, OUTPUT);  
    pinMode(IN4, OUTPUT); 
    pinMode(SW1, INPUT_PULLUP); 
    pinMode(SW2, INPUT_PULLUP); 

    /* Starting SerialPort */
    Serial.begin(9600);
    
    /* Calibrating and setting the range of blind */
    range = blinds_calibrate(SW2);

} 

void loop() 
{ 
    /* Manually move (by switch) blinds up until fully open or stopped by user */
    if (digitalRead(SW1) == LOW) {
        delay(300);
        blinds_move_up(0, switches_pin);
        Serial.println(convert_currVal_to_prcVal(currentVal));
    }
    
    /* Manually move (by switch) blinds down until fully closed or stopped by user */
    if (digitalRead(SW2) == LOW) {
        delay(300);
        blinds_move_down(range, switches_pin);
        Serial.println(convert_currVal_to_prcVal(currentVal));
    }

    /* Move blinds to specific value - outside signal (gatevay, serial port) */
    if (Serial.available() > 1) {
        blinds_move_settedPrcValue(Serial.parseInt());
        Serial.println(convert_currVal_to_prcVal(currentVal));
    }
}
