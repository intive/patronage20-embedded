#ifndef MQTT_H
#define MQTT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTAsync.h"

#if !defined(WIN32)
#include <unistd.h>
#else
#include <windows.h>
#endif

// #define ADDRESS "tcp://pszczodrowski.pl:1883"
#define ADDRESS "tcp://gate.patronage2020 iot.intive projects.com:8883"
#define CLIENTID "ExampleClientSub"
#define TOPIC "intive/embedded"
#define PAYLOAD "Hello World!"
#define QOS 1
#define TIMEOUT 10000L

volatile MQTTAsync_token deliveredtoken;

class MQTT
{
private:
	MQTTAsync client;
	MQTTAsync_connectOptions conn_opts;
	MQTTAsync_disconnectOptions disc_opts;

	int disc_finished = 0;

	static void onConnectFailure(void *context, MQTTAsync_failureData *response){
		printf("Connect failed, rc %d\n", response ? response->code : 0);
		// return response ? response->code != NULL : 0;
		// finished = 1;
	}
	static void onConnect(void *context, MQTTAsync_successData *response){
		static_cast<MQTT*>(context)->onConnect(context,response);
		MQTTAsync client = (MQTTAsync)context;
		MQTTAsync_responseOptions opts = MQTTAsync_responseOptions_initializer;
		int rc;

		printf("Successful connection\n");

		// opts.onSuccess = onSubscribe;
		// opts.onFailure = onSubscribeFailure;
		opts.context = client;

		deliveredtoken = 0;

		if ((rc = MQTTAsync_subscribe(client, TOPIC, QOS, &opts)) != MQTTASYNC_SUCCESS)
		{
			printf("Failed to start subscribe, return code %d\n", rc);
			exit(EXIT_FAILURE);
			// return rc;
		}
	}
	
	static void connlost(void *context, char *cause){
		MQTTAsync client = (MQTTAsync)context;
		MQTTAsync_connectOptions conn_opts = MQTTAsync_connectOptions_initializer;
		int rc;

		printf("\nConnection lost\n");
		if (cause)
			printf("     cause: %s\n", cause);

		printf("Reconnecting\n");
		conn_opts.keepAliveInterval = 20;
		conn_opts.cleansession = 1;
		if ((rc = MQTTAsync_connect(client, &conn_opts)) != MQTTASYNC_SUCCESS)
		{
			printf("Failed to start connect, return code %d\n", rc);
			exit(rc);
		}
	}
	static int msgarrvd(void *context, char *topicName, int topicLen, MQTTAsync_message *message){
		int i;
		char *payloadptr;

		printf("Message arrived\n");
		printf("     topic: %s\n", topicName);
		printf("   message: ");

		payloadptr = (char *)message->payload;
		for (i = 0; i < message->payloadlen; i++)
		{
			putchar(*payloadptr++);
		}
		putchar('\n');
		MQTTAsync_freeMessage(&message);
		MQTTAsync_free(topicName);
		return 1;
	}
	static void onDisconnect(void* context, MQTTAsync_successData* response){
		static_cast<MQTT*>(context)->onDisconnect(context,response);
		printf("Successful disconnection\n");
		// this->disc_finished = 1;
	}
public:
	void loop(){
		int ch;
		int rc;
		printf("loop");
		do 
		{
			ch = getchar();
		} while (ch!='Q' && ch != 'q');

		// std::function<void(void*, MQTTAsync_successData*)> onDisconnect_bind = std::bind(&MQTT::onDisconnect, this, std::placeholders::_1, std::placeholders::_2);

		disc_opts.onSuccess = onDisconnect;
		if ((rc = MQTTAsync_disconnect(client, &disc_opts)) != MQTTASYNC_SUCCESS)
		{
			printf("Failed to start disconnect, return code %d\n", rc);
			exit(EXIT_FAILURE);
		}
		while	(!disc_finished)
			#if defined(WIN32)
				Sleep(100);
			#else
				usleep(10000L);
			#endif
	}
	void connect(){
		int rc;
		conn_opts = MQTTAsync_connectOptions_initializer;
		conn_opts.keepAliveInterval = 20;
		conn_opts.cleansession = 1;
		printf("mqtt1\n");
		// std::function<void(void *, MQTTAsync_failureData *)> onConnect_bind = std::bind(&MQTT::onConnect, this, std::placeholders::_1, std::placeholders::_2);
		// std::function<void(void *, MQTTAsync_successData *)> onConnectFailure_bind = std::bind(&MQTT::onConnectFailure, this, std::placeholders::_1, std::placeholders::_2);
	// printf("mqtt\n");
		MQTTAsync_create(&this->client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
		MQTTAsync_setCallbacks(this->client, this, connlost, msgarrvd, NULL);

		conn_opts.onSuccess = MQTT::onConnect;
		printf("mqtt2\n");
		conn_opts.onFailure = MQTT::onConnectFailure;
		printf("mqtt3\n");
		conn_opts.context = this->client;
		printf("mqtt4\n");
		// MQTTAsync_connectOptions const& conn = conn_opts;
		// MQTTAsync_connectOptions *conn_opts_point = &conn_opts;
		// MQTTAsync_connect(this->client, &conn_opts);
		if ((rc = MQTTAsync_connect(this->client, &conn_opts)) != MQTTASYNC_SUCCESS)
		{
			printf("mqtt5\n");
			printf("Failed to start connect, return code %d\n", rc);
			exit(EXIT_FAILURE);
		}
		printf("mqtt6\n");
	}

	MQTTAsync_connectOptions conn_opts_const() const {
        return this->conn_opts;    
    }

	MQTT(){
	// 	int rc;
	// 	conn_opts = MQTTAsync_connectOptions_initializer;
	// 	conn_opts.keepAliveInterval = 20;
	// 	conn_opts.cleansession = 1;
	// printf("mqtt1\n");
	// 	// std::function<void(void *, MQTTAsync_failureData *)> onConnect_bind = std::bind(&MQTT::onConnect, this, std::placeholders::_1, std::placeholders::_2);
	// 	// std::function<void(void *, MQTTAsync_successData *)> onConnectFailure_bind = std::bind(&MQTT::onConnectFailure, this, std::placeholders::_1, std::placeholders::_2);
	// // printf("mqtt\n");
	// 	MQTTAsync_create(&this->client, ADDRESS, CLIENTID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	// 	MQTTAsync_setCallbacks(this->client, this, connlost, msgarrvd, NULL);

	// 	conn_opts.onSuccess = MQTT::onConnect;
	// 	printf("mqtt2\n");
	// 	conn_opts.onFailure = MQTT::onConnectFailure;
	// 	printf("mqtt3\n");
	// 	conn_opts.context = this->client;
	// 	printf("mqtt4\n");
	// 	// MQTTAsync_connectOptions const& conn = conn_opts;
	// 	// MQTTAsync_connectOptions *conn_opts_point = &conn_opts;
	// 	// MQTTAsync_connect(this->client, &conn_opts);
	// 	if ((rc = MQTTAsync_connect(this->client, &conn_opts)) != MQTTASYNC_SUCCESS)
	// 	{
	// 		printf("mqtt5\n");
	// 		printf("Failed to start connect, return code %d\n", rc);
	// 		exit(EXIT_FAILURE);
	// 	}
	// 	printf("mqtt6\n");
	this->connect();
	}
};

#endif