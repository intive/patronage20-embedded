#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <Arduino.h>

#define DEBUG 1

const char broker[] = "broker.hivemq.com";
int        port     = 1883;
const char topic[]  = "intive/embedded";
const char outTopic[] = "intive/embedded_o";

const char serialnumber[] = "12345678";

class MQTT{
    private:
        WiFiClient wifiClient;
        PubSubClient client;

        void callback(char* topic, byte* payload, unsigned int length);
        // void callback(char* topic, byte* payload, unsigned int length, );
        void reconnect();
    public:
        MQTT(){
            client.setClient(wifiClient);
            client.setServer(broker, port);

            std::function<void(char*, uint8_t*, unsigned int)> WhereCatsDoMEWMEW = std::bind(&MQTT::callback, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
            client.setCallback(WhereCatsDoMEWMEW);
        };
        void loop(){
            if(WiFi.isConnected()){
                if (!client.connected()) {
                    reconnect();
                }
            }
            client.loop();
        };
};

#endif