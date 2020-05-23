#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>        /* Include the Wi-Fi library */
#include <Arduino.h>
#include <init_config.h>
#include <FS.h>
#include <WiFiClientSecure.h>
#include <time.h>

#define DEBUG 0



/* Function with string parameters only */
#if HVAC_SUBST==1
typedef void (*callback_function)(String, char*); /* type for conciseness */
#else
typedef void (*callback_function)(String);  
#endif

class MQTT{
    private:
        /* Wifi client for secure connections */
        WiFiClientSecure wifiClient;
        PubSubClient client;
        callback_function returnFunct = nullptr; /* variable to store function pointer type */

        /*  Gets whole traffic from MQTT and interprets it */
        void callback(char* topic, byte* payload, unsigned int length);

        void reconnect();
    public:
        /*  In constructot a library configures mqtt and setup callback function  */
        MQTT();

        /*  This should be called in loop function in main device file  */
        void loop();
        /* Function to load a certificate from SPIFFS filesystem placed into ESP
           Return 0 for OK, 1 for failed to mount file system,
           2 for failed to open certificate, 3 for load certificate failed
        */
        int load_certificate();
        /* Function to set time and date in ESP */
        void set_clock();
        /*  Sets a pointer on the function that callback should be returned   */
        void set_return_function(callback_function _returnFunct){
            this->returnFunct = _returnFunct;
        }
        /*   Send a message to a specific topic. It's overloaded for a lot of scenarios  */
        void send(const char* s){client.publish(mqttOutputTopic, s);};
        void send(String s){client.publish(mqttOutputTopic,s.c_str());};
        void send(const char* t, const char* s){client.publish(t,s);};
        void send(const char* t, String s){client.publish(t,s.c_str());};
        void send(String t, String s){client.publish(t.c_str(),s.c_str());};
        void send(String t, const char* s){client.publish(t.c_str(),s);};

};


#endif
