#include "MQTT.h"

void MQTT::reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    #if DEBUG==1
    Serial.print("Attempting MQTT connection...");
    #endif
    // Attempt to connect
    if (client.connect(serialnumber)) {
        #if DEBUG==1
        Serial.println("connected");
        #endif
      // Once connected, publish an announcement...
        // client.publish(outTopic, serialnumber);
        client.subscribe(topic);
    } else {
        #if DEBUG==1
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        #endif
    // Wait 5 seconds before retrying
        delay(5000);
    }
  }
}

void MQTT::callback(char* topic, byte* payload, unsigned int length) {
  // DEBUG
  #if DEBUG==1
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  #endif
  //
  
  String payload_string = "";
  for (int i=0;i<length;i++) {
    payload_string+=(char)payload[i];
  }
  #if DEBUG==1
  Serial.print(payload_string);
  #endif
  if(returnFunct!=nullptr)
    returnFunct(payload_string);
}

void MQTT::loop(){
  if(WiFi.isConnected())
    if (!client.connected())
      reconnect();
  
  client.loop();
}

MQTT::MQTT(){
  client.setClient(wifiClient);
  client.setServer(broker, port);

  std::function<void(char*, uint8_t*, unsigned int)> WhereCatsDoMEWMEW = std::bind(&MQTT::callback, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
  client.setCallback(WhereCatsDoMEWMEW);         
}