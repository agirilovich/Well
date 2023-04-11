#include <Arduino.h>

//define UART3 port
HardwareSerial Serial3(PB11, PB10);

//#define BOARD_NAME "STM32Well"

//Import credentials from external file out of git repo
#include <Credentials.h>
const char *ssid = ssid_name;
const char *password = ssid_password;

const char *mqtt_host = mqtt_server;
const int mqtt_port = 1883;
const char *mqtt_user = mqtt_username;
const char *mqtt_pass = mqtt_password;


#include "controlWiFi.h" 
WiFiClient client;

#include "MQTT_task.h"
MQTTPubSubClient mqtt;

const char *StateTopic    = "/homeassistant/sensor/well/config";   // State Topic
const char *ConfigTopic    = "/homeassistant/sensor/well/config";   // Autodiscovery topic
const char *ConfigMessage  = "{\"name\": \"well\", \"device_class\": \"distance\", \"state_class\": \"measurement\",\"unit_of_measurement\": \"cm\", \"state_topic\": StateTopic}";       // Message for Autodiscovery

int WaterLevel = 0;

void setup()
{
  // Debug console
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  Serial.print(F("\nStart WiFiMQTT on "));
  Serial.print(BOARD_NAME);
  
  initializeWiFiShield();
  Serial.println(F("WiFi shield init done"));

  Serial.print(F("Connecting to WiFi network"));
  Serial.print(ssid);
  establishWiFi(ssid, password);

  // you're connected now, so print out the data
  printWifiStatus();
  
  Serial.print("Connecting to MQTT broker host: ");
  Serial.println(mqtt_host);

  while (!client.connect(mqtt_host, mqtt_port))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected!");

  mqtt.begin(client);

  //Initialise MQTT autodiscovery topic and sensor
  initializeMQTTTopic(mqtt, mqtt_user, mqtt_pass, StateTopic, ConfigTopic, ConfigMessage);

}

void loop()
{
  publishMQTTPayload(mqtt, mqtt_user, mqtt_pass, StateTopic, WaterLevel);
     
}