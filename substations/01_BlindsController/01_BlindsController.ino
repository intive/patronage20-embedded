/* BlindsController by Kuba Bartoszewicz */

#include "blinds.h"
#include "MQTT.h"
#include "init_config.h"

/* Defining pins for Stepper controller */
#define IN1 16 
#define IN2 5 
#define IN3 4 
#define IN4 0 

/* Defining pins for switch */
#define SW1 14
#define SW2 12

/* Defining substation ID number */
#define SUBST_ID 1

MQTT mqtt;

/* Recive and interpret JSON, move blinds if proper JSON recived */
void MQTT_blinds_move(String s) 
{
    StaticJsonDocument<100> doc;
    deserializeJson(doc, s);
    if (doc["id"].as<int>() == SUBST_ID) {
        blinds_move_targetValue(doc["value"]);
    }
    Serial.print("Current value: ");
    Serial.println(convert_to_percent(currentVal));

}


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

    /* Starting WiFi connection */
    WiFi.begin(ssid, passwd);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    
    /* Calibrating and setting the range of blind */
    range = blinds_calibrate(SW2);

    mqtt.set_return_function(MQTT_blinds_move);

} 


void loop() 
{ 
    
    mqtt.loop();
       
    
    /* Manually move (by switch) blinds up until fully open or stopped by user */
    if (digitalRead(SW1) == LOW) {
        delay(300);
        blinds_move_up(0, switches_pin);
        Serial.print("Current value: ");
        Serial.println(convert_to_percent(currentVal));
        mqtt.send(topic, return_JSON(SUBST_ID, convert_to_percent(currentVal)));
    }
    
    /* Manually move (by switch) blinds down until fully closed or stopped by user */
    if (digitalRead(SW2) == LOW) {
        delay(300);
        blinds_move_down(range, switches_pin);
        Serial.print("Current value: ");
        Serial.println(convert_to_percent(currentVal));
        mqtt.send(topic, return_JSON(SUBST_ID, convert_to_percent(currentVal)));
    }

    /* Move blinds to specific value - outside signal (gatevay, serial port) */
    if (Serial.available() > 1) {
        blinds_move_targetValue(Serial.parseInt());
        Serial.print("Current value: ");
        Serial.println(convert_to_percent(currentVal));
        mqtt.send(topic, return_JSON(SUBST_ID, convert_to_percent(currentVal)));
    }

    


}
