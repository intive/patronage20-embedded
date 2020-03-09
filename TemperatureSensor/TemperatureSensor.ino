#include "mqtt_wifi_init.h"
#include "getTemperature.h"

/*
 * To configure please setup varibles in mqtt_wifi_init.h
 * #define SECRET_SSID ""
 * #define SECRET_PASS ""
 * 
 * and pins in getTemperature.h for ReadDigitalTemperature(),
 * ReadAnalogTemperature()
 * 
 * Default broker is set in mqtt_wifi_init.h
 * const char broker[] = "broker.hivemq.com";
 * int        port     = 1883;
 * const char topic[]  = "Embedded"
 *  
 */
void setup() {
      
    mqtt_and_wifi_setup();

}

void loop() {
  
    mqttClient.poll(); //keep mqtt connection alive
      
    SendMessageToBroker( getTemperature(true) ); //sending digital temperature sensor value
    delay(1000);
}
