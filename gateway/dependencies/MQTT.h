/*
MQTT C function
Use MQTT_connect() for connect with our server with default options
Then use MQTT_set_return_function(print); to link function

Sample code:

#include <unistd.h>

#include "MQTT.h"
using namespace std;
char s = 0;
void print(const char* c){
    printf("%s\n",c);
}

int main(){
    MQTT_connect();
    MQTT_set_return_function(print);
    MQTT_send("test","intive/embedded");
    
    printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
               "Press Q<Enter> to quit\n\n",
               TOPIC_I, CLIENTID, QOS);
    int ch;
    do
        {
            usleep(100000L);
        } while (ch != 'Q' && ch != 'q');
}
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <functional>

#include "MQTTClient.h"

#define ADDRESS "ssl://gate.patronage2020-iot.intive-projects.com:8883"
#define CLIENTID "gateway"
#define TOPIC_I "intive/embedded"
#define TOPIC_O "intive/embedded_o"
#define CAFILE "/etc/letsencrypt/live/gate.patronage2020-iot.intive-projects.com/fullchain.pem"
#define QOS 1
#define TIMEOUT 10000L

volatile MQTTClient_deliveryToken deliveredtoken;
typedef void (*cb_function)(const char *); /* type for conciseness */

MQTTClient client;
MQTTClient_connectOptions conn_opts;
MQTTClient_SSLOptions ssl_opts;
cb_function returnFunct = nullptr; /* variable to store function pointer type */

/*
This function is needed by MQTT Paho
*/
void delivered(void *context, MQTTClient_deliveryToken dt) {
    deliveredtoken = dt;
}
/*
Callback function
*/
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char payloadptr_r[message->payloadlen + 1];
    char *payloadptr = (char *)message->payload;
    printf("%d\n", message->payloadlen);
    for (i = 0; i < message->payloadlen; i++) {
        payloadptr_r[i] = *payloadptr++;
    }
    payloadptr_r[message->payloadlen] = '\0';

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    if (returnFunct != nullptr)
        returnFunct((const char *)payloadptr_r);

    return 1;
}

/*
This function is needed by MQTT Paho
*/
void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

/*
Use this for connect to server
*/
void MQTT_connect(const char *host = ADDRESS, const char *clientID = CLIENTID, const char *topic = TOPIC_I, bool isSSL = true, const char *caFile = CAFILE) {
    conn_opts = MQTTClient_connectOptions_initializer;
    if (isSSL) {
        ssl_opts = MQTTClient_SSLOptions_initializer;
        ssl_opts.enableServerCertAuth = 1;
        ssl_opts.trustStore = caFile;
    }

    MQTTClient_create(&client, host, clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if (isSSL)
        conn_opts.ssl = &ssl_opts;

    MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
    int rc;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        printf("Failed to connect, return code %d\n", rc);
    }
    MQTTClient_subscribe(client, topic, QOS);
}

/*
Set pointer to function, which callback will call when payload comes
*/
void MQTT_set_return_function(cb_function _returnFunct) {
    returnFunct = _returnFunct;
}

/*
Use this if you want to manually disconnect from server
*/
void MQTT_disconnect() {
    MQTTClient_unsubscribe(client, TOPIC_I);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

/*
MQTT sending function. Set const char * topic for custom topic
*/
int MQTT_send(const char *msg, const char *topic = TOPIC_O) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    pubmsg.payload = (char *)msg;
    pubmsg.payloadlen = strlen(msg);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    return MQTTClient_publishMessage(client, topic, &pubmsg, &token);
}
