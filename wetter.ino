#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define wifi_ssid "SSID"
#define wifi_password "PASSWORD"

#define mqtt_server "MQTT-SERVER-HOST"
#define mqtt_user "USER"
#define mqtt_password "PASSWORD"

#define temperature_topic "TEMPERATURE-TOPIC"
#define humidity_topic "HUMIDITY-TOPIC"
#define pressure_topic "PRESSURE-TOPIC"

Adafruit_BME280 bme;
WiFiClient espClient;
PubSubClient client(espClient);

unsigned long delayTime;

void setup() {
  Serial.begin(9600);
  Serial.println("JH Wetterstation fuer BME280");

  setupWifi();

  client.setServer(mqtt_server, 1883);

  bool status;
  
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a BME280 sensor!");
    while (1);
  }

  delayTime = 5000;
  Serial.println();
}


void setupWifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in a seconds");
      delay(1000);
    }
  }
}

void loop() { 
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  double temperature = (int)(bme.readTemperature() * 10 + 0.5) / 10.0;
  String temp = String(temperature);
  temp.remove(4);
 
  int humidity = round(bme.readHumidity());
  int pressure = round(bme.readPressure() / 100.0F);

  Serial.println(String(temp) + " °C");
  Serial.println(String(humidity) + " %");
  Serial.println(String(pressure) + " hPa");
  
  client.publish(temperature_topic, String(temp).c_str(), true);
  client.publish(humidity_topic, String(humidity).c_str(), true);
  
  if(pressure < 1100 && pressure > 700) {
    client.publish(pressure_topic, String(pressure).c_str(), true);
  }

  client.publish("outdoors/frontyard/sensors/battery", String(66).c_str(), true);

  client.disconnect();

  ESP.deepSleep(3e8); // 5 minutes 
}
