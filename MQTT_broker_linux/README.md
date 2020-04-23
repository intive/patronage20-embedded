# MQTT Broker install

## Instalation

**For more information visit this MQTT [Online tutorial](https://www.vultr.com/docs/how-to-install-mosquitto-mqtt-broker-server-on-ubuntu-16-04)**

## Prerequisites

* Debian-based system with **root** access
* Open **TCP:1883** port on the firewall

## First step: install on Debian-based system: 

Update package list and install Mosquitto broker.

```bash
sudo apt-get update
sudo apt-get install mosquitto
```

Mosquitto service will start after installation.

## For testing purposes, install MQTT client

Only for testing purposes, we'll install the MQTT client. It can be deleted later.

```bash
sudo apt-get install mosquitto-clients
```

## Configuring

### Change port

Default port is 1883. If you want to change it open configuration file. If not go to **Testing**

```bash
sudo nano /etc/mosquitto/mosquitto.conf
```
or if you're hardcore Linux user
```bash
sudo vi /etc/mosquitto/mosquitto.conf
```

Add additional instruction at end of file

```bash
port $PORT_NUMBER
```

After it, we need to restart MQTT service

```bash
sudo service mosquitto reload
```

Service should work on new port. Remember to open new port on firewall.


## Testing

Open two terminals and copy below commands

## First terminal - subscribe topic
```bash
mosquitto_sub -t "TEST/TOPIC"
```

## Second terminal - send text on our topic
```bash
mosquitto_pub -m "TEST_MESSAGE" -t "TEST/TOPIC"
```

You should see your message on first terminal.

## Extras

### UFW rule

If the server uses ufw firewall, add a new rule to list

```bash
sudo ufw allow from any to any port 1883 proto tcp
```