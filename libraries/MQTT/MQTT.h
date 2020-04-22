#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>        /* Include the Wi-Fi library */
#include <Arduino.h>
#include <init_config.h>
#include <FS.h>
#include <WiFiClientSecure.h>
#include <time.h>

#define DEBUG 1


/* Function with string parameters only */
typedef void (*callback_function)(String); /* type for conciseness */

class MQTT{
    private:
        /* Wifi client for secure connections */
        WiFiClientSecure wifiClient;
        PubSubClient client;
        callback_function returnFunct = nullptr; /* variable to store function pointer type */
        bool enabled_tls; /* Enable secure connection by default */

        /*  Gets whole traffic from MQTT and interprets it */
        void callback(char* topic, byte* payload, unsigned int length);

        void reconnect();
    public:
        /*  In constructot a library configures mqtt and setup callback function  */
        MQTT();

        /*  This should be called in loop function in main device file  */
        void loop();
        /* Function to load a certificate from SPIFFS filesystem placed into ESP */
        void load_certificate();
        /* Function to set time and date in ESP */
        void set_clock();
        /*  Sets a pointer on the function that callback should be returned   */
        void set_return_function(callback_function _returnFunct){
            this->returnFunct = _returnFunct;
        }
        bool is_tls() {return enabled_tls;};
        void enable_tls(bool status) {enabled_tls = status;};
        /*   Send a message to a specific topic. It's overloaded for a lot of scenarios  */
        void send(const char* s){client.publish(outTopic, s);};
        void send(String s){client.publish(outTopic,s.c_str());};
        void send(const char* t, const char* s){client.publish(t,s);};
        void send(const char* t, String s){client.publish(t,s.c_str());};
        void send(String t, String s){client.publish(t.c_str(),s.c_str());};
        void send(String t, const char* s){client.publish(t.c_str(),s);};

};


#endif
