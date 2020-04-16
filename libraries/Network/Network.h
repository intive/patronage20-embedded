#ifndef NETWORK_H
#define NETWORK_H


#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include <stdio.h>      /* printf, scanf, NULL */
#include <stdlib.h>     /* malloc, free, rand */
#include <init_config.h>

class Network{
private:
    String ssid_o = ssid;
    String password = passwd;

    WiFiClient wifiClient;

    /*
        Convert from const char* to uint8_t*

        @param char array looks like this: "xx:xx:xx:xx:xx:xx"
        @return Array of 6 uint8_t elements
    */
    uint8_t* str2mac(const char *mac){
        int bytes[6];
        uint8_t* mac_bytes = (uint8_t*) malloc(6);

        if( 6 == sscanf( mac, "%x:%x:%x:%x:%x:%x%*c",
            &bytes[0], &bytes[1], &bytes[2],
            &bytes[3], &bytes[4], &bytes[5] )){
            /* convert to uint8_t */        
            for(int i = 0; i < 6; ++i )
                mac_bytes[i] = (uint8_t) bytes[i];

            return mac_bytes;
        }else{
            return nullptr;
        }
    }

    /*
        Convert from const char* to IPAddress

        @param char array looks like this: "255.255.255.255"
        @return IPAddress object with ip address in it
    */
    IPAddress str2ip(const char* ip){
        int bytes[4];
        uint8_t* ip_bytes = (uint8_t*) malloc(4);
        if( 4 == sscanf(ip, "%d.%d.%d.%d%*c", 
            &bytes[0],&bytes[1],
            &bytes[2],&bytes[3])){
            /* convert to uint8_t */ 
            for(int i = 0; i < 4; ++i )
                ip_bytes[i] = (uint8_t) bytes[i];
            return IPAddress(ip_bytes);
        }else{
            return NULL;
        }
    }

public:
    Network(){};
    Network(String _ssid, String _pass) : ssid_o(_ssid), password(_pass){};
    Network(String _ssid, String _pass, const char *mac) : ssid_o(_ssid), password(_pass){
        this->change_mac_address(mac);
    };

    void set_static_ip(const char *_ip, const char *_gateway, const char *_subnet){
        WiFi.config(str2ip(_ip),str2ip(_gateway),str2ip(_subnet));
    }
    void set_static_ip(const char *_ip, const char *_gateway, 
        const char *_subnet, const char* _dns){
        
        WiFi.config(str2ip(_ip),str2ip(_gateway),
        str2ip(_subnet),str2ip(_dns));
    }
    void set_static_ip(const char *_ip, const char *_gateway, 
        const char *_subnet, const char* _dns, const char* _dns2){
        
        WiFi.config(str2ip(_ip),str2ip(_gateway),
        str2ip(_subnet),str2ip(_dns),str2ip(_dns2));
    }

    /*
        Changes mac address for ESP32 and ESP8266
    */
    void change_mac_address(const char *MAC){
        // uint8_t *mac = (uint8_t*) malloc(6);
        uint8_t *mac = str2mac(MAC);
        #ifdef ESP32
            esp_wifi_set_mac(ESP_IF_WIFI_STA, &mac[0]);
        #else
            wifi_set_macaddr(STATION_IF, &mac[0]);
        #endif
    }

    /*
        Force ESP to reconnect wifi connection
    */
    void reconnect(){
        WiFi.begin(ssid_o, password);
        WiFi.mode(WIFI_STA);
        while (WiFi.status() != WL_CONNECTED)
            delay(1000);
    }

    /*
        Put in init() function on main file
    */
    WiFiClient init(){
        reconnect();
    };

    /*
        Put in loop() function on main file
    */
    void loop(){
        if (!WiFi.status() != WL_CONNECTED)
            reconnect();
    }
};

#endif