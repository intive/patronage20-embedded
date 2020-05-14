/* BlindsController by Kuba Bartoszewicz */

#include "blinds.h"
#include "MQTT.h"
#include "init_config.h"
#include "Network.h"
#include <Servo.h>

/* Defining pins for Stepper controller */
#define IN1 16 
#define IN2 5 
#define IN3 4 
#define IN4 0 

/* Defining pins for switch */
#define SW1 14
#define SW2 12

/* PIN for Servo */
#define PIN 15

/* Defining substation ID number */
int id_blind = windowBlinds_ID;
int id_servo = Servo_1_ID;
unsigned int notifCaller = 0;
int servoAng;

MQTT mqtt;
Network network(ssid, passwd);
Servo servo;

/* Recive and interpret JSON, move blinds if proper JSON recived */
void MQTT_blinds_move(String s) 
{
    StaticJsonDocument<100> doc;
    deserializeJson(doc, s);
    
    /* blinds */
    if (doc["id"].as<int>() == id_blind) {
        blinds_move_targetValue(doc["position"]);
        Serial.print("Current value: ");
        Serial.println(convert_to_percent(currentVal));
    }
    
    /* servo */    
    if(doc["id"].as<int>() == id_servo)
        if(doc["type"].as<String>().equals("Servo")) {
            servoAng = doc["angle"].as<int>();
            servo.write(servoAng);
        }
}

/* Servo output */
String output_JSON_SERVO(int a, int id, String t)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = t;
    root["angle"] = a;
    serializeJson(root, json_output);
    return json_output;
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
    network.init();
    
    /* Calibrating and setting the range of blind */
    range = blinds_calibrate(SW2);

    /* Servo attach */
    servo.attach(PIN);
    servo.write(10);
    
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
        mqtt.send(return_JSON(id_blind, convert_to_percent(currentVal)));
    }
    
    /* Manually move (by switch) blinds down until fully closed or stopped by user */
    if (digitalRead(SW2) == LOW) {
        delay(300);
        blinds_move_down(range, switches_pin);
        Serial.print("Current value: ");
        Serial.println(convert_to_percent(currentVal));
        mqtt.send(return_JSON(id_blind, convert_to_percent(currentVal)));
    }

    /* Move blinds to specific value - outside signal (gatevay, serial port) */
    if (Serial.available() > 1) {
        blinds_move_targetValue(Serial.parseInt());
        Serial.print("Current value: ");
        Serial.println(convert_to_percent(currentVal));
        mqtt.send(return_JSON(id_blind, convert_to_percent(currentVal)));
    }

    /* notify the position value every 1 sec */
    if (++notifCaller >= 1000) {
        mqtt.send(return_JSON(id_blind, convert_to_percent(currentVal)));
        mqtt.send(output_JSON_SERVO(servoAng, id_servo, "Servo"));
        notifCaller = 0;
    }
    
    delay(1);
    

}
