# Wetterstation für MQTT

## Benötigte Bibliotheken:
- ESP8266Wifi
- Adafruit BME280
- Adafruit Unified Sensor
- PubSubClient (für MQTT)

## Hardware-Setup
BME280 SDA -> D2

BME280 SCL -> D1

ESP D0 -> RST

## Software-Setup
### Wifi
SSID und Passwort ersetzen

### MQTT
Broker-Hostname, Benutzername und Passwort eingeben

Topics setzen

## Sketch hochladen und ggf. mit [MQTT.fx](https://mqttfx.jensd.de) debuggen.
