#ifndef NETWORK_H
#define NETWORK_H

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <Arduino.h>
#include "Config.h"

const char *ssid = STASSID;
const char *password = STAPASS;
#ifdef STAMAC
uint8_t *mac;
#endif
class Network
{
private:
    WiFiClient wifiClient;

    int str2mac(const char *mac, uint8_t *values)
    {
        if (6 == sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5]))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }

public:
    Network(){};
    void connect()
    {
        WiFi.mode(WIFI_STA);

#ifdef STAMAC
        int code = str2mac(STAMAC, mac);
        if (code)
        {
#ifdef ESP32
            esp_wifi_set_mac(ESP_IF_WIFI_STA, &mac[0]);
#else
            wifi_set_macaddr(STATION_IF, &mac[0]);
#endif
        }
#endif

        reconnect();
    }
    void reconnect()
    {
        WiFi.begin(ssid, password);

        while (WiFi.status() != WL_CONNECTED)
            delay(1000);
    }
    WiFiClient init()
    {
        connect();
    };
    void loop()
    {
        if (!WiFi.status() != WL_CONNECTED)
            reconnect();
    }
};

#endif