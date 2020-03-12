#!/bin/sh
#For ufw configuration: https://webinsider.pl/linux-firewall-ufw/
#MQTT tutorial: https://www.vultr.com/docs/how-to-install-mosquitto-mqtt-broker-server-on-ubuntu-16-04

sudo apt-get update
sudo apt-get install mosquitto

sudo ufw allow from any to any port 1883 proto tcp

#If we want a mqtt clien
#sudo apt-get install mosquitto-clients

#subscribe topic
# mosquitto_sub -t "intive/embedded_o"

#publish
# mosquitto_pub -m "1" -t "intive/embedded"
