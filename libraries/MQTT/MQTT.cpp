#include "MQTT.h"

void MQTT::reconnect() {
    /* Loop until we're reconnected */
    while (!client.connected()) {
        #if DEBUG==1
        Serial.print("Attempting MQTT connection...");
        #endif
      /* Attempt to connect */
        if (client.connect(serialnumber)) {
            #if DEBUG==1
            Serial.println("connected");
            #endif
            client.subscribe(topic);
        } else {
            #if DEBUG==1
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            #endif
          /* Wait 5 seconds before retrying */
            delay(5000);
      }
    }
  }

/*  Gets whole traffic from MQTT and interprets it */
void MQTT::callback(char* topic, byte* payload, unsigned int length) {
  /* DEBUG */
    #if DEBUG==1
    Serial.print("\nMessage arrived [");
    Serial.print(topic);
    Serial.print("] ");
    #endif
    /*End of debug*/

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

/*  This should be called in loop function in main device file  */
void MQTT::loop(){
    if(WiFi.isConnected())
        if (!client.connected()) {
            /* Load a certificate before connection, the ESP clock is set */
            if(enabled_tls)
                load_certificate();
            reconnect();
        }

    client.loop();
}

/*  In constructot a library configures mqtt and setup callback function  */
MQTT::MQTT(){
    client.setClient(wifiClient);
    client.setServer(broker, port);
    enabled_tls = true;
    std::function<void(char*, uint8_t*, unsigned int)> WhereCatsDoMEWMEW = std::bind(&MQTT::callback, this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3);
    client.setCallback(WhereCatsDoMEWMEW);
}

void MQTT::load_certificate() {
    /* Set the ESP clock time, needed for certificate authorization */
    set_clock();

    /* Try to mount SPIFFS file system to load certificate */
    if (!SPIFFS.begin()) {
        #if DEBUG==1
        Serial.println("Failed to mount file system");
        #endif
        return;
    }

    /* Try to load CA file from SPIFFS */
    File ca = SPIFFS.open("/CA.crt", "r");
    if (!ca) {
        #if DEBUG==1
        Serial.println("Failed to open certificate");
        #endif
    }
    else {
        #if DEBUG==1
        Serial.println("Success to open certificate");
        #endif
        delay(1000);
        /* Set server certificate file */
        if(wifiClient.loadCACert(ca)) {
            #if DEBUG==1
            Serial.println("Certificate loaded");
            #endif
        }
        else {
            #if DEBUG==1
            Serial.println("Load certificate failed");
            #endif
        }
    }
}
void MQTT::set_clock() {
    /* Set timezone, day light offset and time servers */
    configTime(2 * 3600, 0, "pool.ntp.org", "time.nist.gov");

    #if DEBUG==1
    Serial.print("Waiting for NTP time sync: ");
    #endif

    time_t now = time(nullptr);
    /* Until time synchronization is complete, the time(nullptr) function returns time around unix zero time*/
    while (now < 8 * 3600 * 2) {
        delay(500);

        #if DEBUG==1
        Serial.print(".");
        #endif

        now = time(nullptr);
    }
    #if DEBUG==1
    Serial.println("");

    /* Print time to serial */
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
    #endif
}
