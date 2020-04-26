#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <functional>

#include "MQTTClient.h"

#define ADDRESS "ssl://gate.patronage2020-iot.intive-projects.com:8883"
#define CLIENTID "gateway"
#define TOPIC_I "intive/embedded"
#define TOPIC_O "intive/embedded_o"
#define CAFILE "DST.pem"
#define QOS 1
#define TIMEOUT 10000L

volatile MQTTClient_deliveryToken deliveredtoken;
typedef void (*cb_function)(const char *); /* type for conciseness */

MQTTClient client;
MQTTClient_connectOptions conn_opts;
MQTTClient_SSLOptions ssl_opts;
cb_function returnFunct = nullptr; /* variable to store function pointer type */

void delivered(void *context, MQTTClient_deliveryToken dt) {
    deliveredtoken = dt;
}
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    int i;
    char payloadptr_r[message->payloadlen];
    char *payloadptr = (char *)message->payload;

    for (i = 0; i < message->payloadlen; i++)
        payloadptr_r[i] = *payloadptr++;

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    if (returnFunct != nullptr)
        returnFunct((const char *)payloadptr_r);

    return 1;
}

void connlost(void *context, char *cause) {
    printf("\nConnection lost\n");
    printf("     cause: %s\n", cause);
}

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
        /* exit(EXIT_FAILURE); */
    }
    MQTTClient_subscribe(client, topic, QOS);
}
void MQTT_set_return_function(cb_function _returnFunct) {
    returnFunct = _returnFunct;
}

void MQTT_disconnect() {
    MQTTClient_unsubscribe(client, TOPIC_I);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
}

int MQTT_send(const char *msg, const char *topic = TOPIC_O) {
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    pubmsg.payload = (char *)msg;
    pubmsg.payloadlen = strlen(msg);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    return MQTTClient_publishMessage(client, topic, &pubmsg, &token);
}
