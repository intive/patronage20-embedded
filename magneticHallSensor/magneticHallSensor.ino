#include "EspMQTTClient.h"
#include <ArduinoJson.h>

#define digitalPin 15

EspMQTTClient client(
  "internetDunska",
  "H4sdviwGksvduev",
  "broker.hivemq.com",
  "ESP_SZYMON",
  1883
);


int value;
const char *topic = "Embedded/1";

void setup()
{
  Serial.begin(115200);
  pinMode(digitalPin, INPUT);
  client.enableDebuggingMessages();
  client.enableLastWillMessage(topic, "Device went offline");
}

void onConnectionEstablished()
{
  client.subscribe("Embedded/#", [](const String & message){
    Serial.println(message);  
  });

  client.publish(topic, "Device says hello!");
}


void loop() 
{
  client.loop();
  client.publish(topic, prepareJson(digitalRead(digitalPin)));
  delay(100);
}

String prepareJson(int digValue)
{
  String output = "";
  StaticJsonDocument<100> doc;

  doc["id"] = 7;
  doc["type"] = "windowSensors";
  if (digitalRead(digitalPin) == LOW)
    doc["value"] = "closed";
  else
    doc["value"] = "open";
  serializeJsonPretty(doc, output);
  return output;
}
