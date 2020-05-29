#include <Servo.h>
#include "init_config.h"
#include "MQTT.h"
#include "Network.h"
#include <ArduinoJson.h>

#define NO_OF_SERVOS 4

/* defining PIN's for Servo */
#define SERVO_1H_PIN 16 /* D0 */
#define SERVO_1C_PIN 5  /* D1 */
#define SERVO_2H_PIN 4  /* D2 */
#define SERVO_2C_PIN 0  /* D3 */

/* defining ID's for Servos */
/* order in array: Room 1 heating, Room 1 cooling, Room 2 heating, Room 2 cooling */
const int servo_ID[NO_OF_SERVOS] = {Servo_1H_ID, Servo_1C_ID, Servo_2H_ID, Servo_2C_ID};

/* variable storing angles for servo's */
int servo_ANGLE[NO_OF_SERVOS];

MQTT mqtt;
Network network(ssid, passwd);

/* servo objects */
Servo servo_1H;
Servo servo_1C;
Servo servo_2H;
Servo servo_2C;

String type = "Servo";
int notifCaller = 0;

int getPin_fromServoID(int id) 
{
    switch(id) {
        case Servo_1H_ID:  return SERVO_1H_PIN;
        case Servo_1C_ID:  return SERVO_1C_PIN;
        case Servo_2H_ID:  return SERVO_2H_PIN;
        case Servo_2C_ID:  return SERVO_2C_PIN;
    }
}

String output_JSON(int a, int id, String t = type)
{
    DynamicJsonDocument root(200);
    String json_output = "";
    root["id"] = id;
    root["type"] = t;
    root["angle"] = a;
    serializeJson(root, json_output);
    return json_output;
}

void incoming_JSON(String json_input)
{
    int i;
    
    DynamicJsonDocument json_doc(2048);
    DeserializationError error = deserializeJson(json_doc, json_input);
    
    if (error) {
        return;
    }
       
    if(json_doc["type"].as<String>().equals(type) 
       && json_doc["angle"] >= 0 
       && json_doc["angle"] <=180) {

        for (i = 0; i <= NO_OF_SERVOS; i++) {
            if (servo_ID[i] == json_doc["id"].as<int>()) {
                servo_ANGLE[i] = json_doc["angle"];
            }
        }
    }
}

void setup()
{
    int i;
    
    servo_1H.attach(getPin_fromServoID(servo_ID[0]));
    servo_1C.attach(getPin_fromServoID(servo_ID[1]));
    servo_2H.attach(getPin_fromServoID(servo_ID[2]));
    servo_2C.attach(getPin_fromServoID(servo_ID[3]));
     
    servo_1H.write(10);
    servo_1C.write(10);
    servo_2H.write(10);
    servo_2H.write(10);
 
    Serial.begin(9600);
    network.init();
    mqtt.set_return_function(incoming_JSON);
}


void loop()
{
    int i;
    

    mqtt.loop();

    servo_1H.write(servo_ANGLE[0]);
    servo_1C.write(servo_ANGLE[1]);
    servo_2H.write(servo_ANGLE[2]);
    servo_2C.write(servo_ANGLE[3]);
    
    if (++notifCaller == 1000) {
        for (i = 0; i < NO_OF_SERVOS; i++) {
            mqtt.send(output_JSON(servo_ANGLE[i], servo_ID[i]));
        }
        notifCaller = 0;
    }

    delay(1);

}
