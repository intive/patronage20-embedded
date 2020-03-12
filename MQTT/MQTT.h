#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <Arduino.h>

#define DEBUG 1

// Public broker
// const char broker[] = "broker.hivemq.com";
// int        port     = 1883;
// const char topic[]  = "intive/embedded";
// const char outTopic[] = "intive/embedded_o";

// Our broker
const char broker[] = "pszczodrowski.pl";
int        port     = 1883;
const char topic[]  = "intive/embedded";
const char outTopic[] = "intive/embedded_o";

const char serialnumber[] = "12345678";
// Function with string parameters only
typedef void (*callback_function)(String); // type for conciseness

class MQTT{
    private:
        WiFiClient wifiClient;
        PubSubClient client;
        callback_function returnFunct = nullptr; // variable to store function pointer type
        //TODO:Add pointer to function or handle how to get callback return
        void callback(char* topic, byte* payload, unsigned int length);
        void reconnect();
    public:
        MQTT();
        void loop();
        void setReturnFunct(callback_function _returnFunct){
            this->returnFunct = _returnFunct;
        }
        void send(const char* s){client.publish(outTopic, s);};
        void send(String s){client.publish(outTopic,s.c_str());};
        void send(const char* t, const char* s){client.publish(t,s);};
        void send(const char* t, String s){client.publish(t,s.c_str());};
        void send(String t, String s){client.publish(t.c_str(),s.c_str());};
        void send(String t, const char* s){client.publish(t.c_str(),s);};

};


#endif