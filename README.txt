To make your ESP and sensor work properly make sure to:

Change inside EspMQTTClient client object:
a) SSID -> to your network SSID
b) password -> to your network password
c) brokerIP -> to your broker IP
d) deviceName -> to whatever name you like, it's not neccesary!
e) 1234 -> to your host port

Change digitalPin to pin number on your board:
- In default on ESP8266 pin D8's number is 15 so it is set.

Change topic onto whatever topic you'd like to publish on.