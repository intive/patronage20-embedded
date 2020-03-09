#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h> 

#define SECRET_SSID "R_E_D_O_X"
#define SECRET_PASS "10293000"

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;  

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "broker.hivemq.com";
int        port     = 1883;
const char topic[]  = "Embedded";

const long interval = 1000;
unsigned long previousMillis = 0;

void mqtt_and_wifi_setup() 
    {
        Serial.begin(9600); //Initialize serial and wait for port to open:
        while (!Serial);    // Wait for serial port to connect. Needed for native USB port only

        // attempt to connect to Wifi network:
        Serial.print("Attempting to connect to WPA SSID: "); 
        Serial.println(ssid);
        WiFi.mode(WIFI_STA);
        
        while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
            // failed, retry
            Serial.print(".");
            delay(5000);
        }

        Serial.println("You're connected to the network");
        Serial.println();

       // You can provide a unique client ID, if not set the library uses Arduino-millis()
       // Each client must have a unique client ID
       // mqttClient.setId("clientId");

       // You can provide a username and password for authentication
       // mqttClient.setUsernamePassword("username", "password");

       Serial.print("Attempting to connect to the MQTT broker: ");
       Serial.println(broker);

       if (!mqttClient.connect(broker, port)) {
           Serial.print("MQTT connection failed! Error code = ");
           Serial.println(mqttClient.connectError());
           while (1);
       }

       Serial.println("You're connected to the MQTT broker!");
       Serial.println();
    }

void SendMessageToBroker(String message) 
    {
        Serial.print("Sending message to topic: ");
        Serial.println(topic);

        // send message, the Print interface can be used to set the message contents
        mqttClient.beginMessage(topic);
        mqttClient.print(message);
        mqttClient.endMessage();

        Serial.println();
    }
