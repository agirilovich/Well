#include <Arduino.h>

//define UART3 port
HardwareSerial Serial3(PB11, PB10);

//#define BOARD_NAME "STM32Well"

//Import Wi-Fi credentials from external file out of git repo
#include <Credentials.h>
const char *ssid = ssid_name;
const char *password = ssid_password;

#include "WiFi.h" 
WiFiClient client;

#include <MQTTPubSubClient.h>
MQTTPubSubClient mqtt;

#define MQTT_SERVER           "ha.local"
#define MQTT_PORT             1883

const char *PubTopic    = "/mqttPubSub";                                  // Topic to publish
const char *PubMessage  = "Hello from " BOARD_NAME;       // Topic Message to publish

void setup()
{
  // Debug console
  Serial.begin(115200);

  while (!Serial && millis() < 5000);

  Serial.print(F("\nStart WiFiMQTT on "));
  Serial.print(BOARD_NAME);
  
  initializeWiFiShield();
  Serial.println(F("WiFi shield init done"));

  Serial.print(F("Connecting to SSID: "));
  Serial.println(ssid);
  establishWiFi(ssid, password);

  // you're connected now, so print out the data
  printWifiStatus();

  Serial.print("Connecting to host ");
  Serial.println(MQTT_SERVER);

  while (!client.connect(MQTT_SERVER, MQTT_PORT))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nConnected!");

  // initialize mqtt client
  mqtt.begin(client);
  
  Serial.print("Connecting to mqtt broker...");

  while (!mqtt.connect("arduino", "public", "public"))
  {
    Serial.print(".");
    delay(1000);
  }
   

  Serial.println(" connected!");

  // subscribe callback which is called when every packet has come
  mqtt.subscribe([](const String & topic, const String & payload, const size_t size)
  {
    (void) size;

    Serial.println("MQTT received: " + topic + " - " + payload);
  });

  // subscribe topic and callback which is called when /hello has come
  mqtt.subscribe(PubTopic, [](const String & payload, const size_t size)
  {
    (void) size;

    Serial.print("Subcribed to ");
    Serial.print(PubTopic);
    Serial.print(" => ");
    Serial.println(payload);
  });

  mqtt.publish(PubTopic, PubMessage);
  
}

void loop()
{
  
  mqtt.update();  // should be called

  // publish message
  static uint32_t prev_ms = millis();

  if (millis() > prev_ms + 30000)
  {
    prev_ms = millis();
    mqtt.publish(PubTopic, PubMessage);
  }
  
}