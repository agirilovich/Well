#include <Arduino.h>

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
const char *ConfigMessage  = "{\"name\": \"well\", \"device_class\": \"distance\", \"state_class\": \"measurement\",\"unit_of_measurement\": \"mm\", \"state_topic\": StateTopic}";       // Message for Autodiscovery

//UltrasonicSensor definitions
#include "ultrasonic.h"
unsigned int WaterLevel = 0;
#define ArrayLenght 30
unsigned int LevelsArray[ArrayLenght] = { 0 };

//Multitask definitions
#include <TaskScheduler.h>
#define _TASK_SLEEP_ON_IDLE_RUN  // Enable 1 ms SLEEP_IDLE powerdowns between runs if no callback methods were invoked during the pass

Scheduler runner;

void UltrasonicSensorCallback();
void mqttDelayer();
void MQTTMessageCallback();

Task UltrasonicThread(10 * TASK_SECOND, TASK_FOREVER, &UltrasonicSensorCallback, &runner, true);  //Initially only task is enabled. It runs every 10 seconds indefinitely.
Task mqttThreadDelay(5 * TASK_MINUTE, TASK_ONCE, &mqttDelayer, &runner, true);  //Delay for first run of MQTT publisher.
Task mqttThread(5 * TASK_MINUTE, TASK_FOREVER, &MQTTMessageCallback, &runner);  //Runs every 10 minutes after several measurements of Ultrasonic Sensor

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

  runner.startNow();  // This creates a new scheduling starting point for all ACTIVE tasks.

  initializeUltrasonic();

}

void loop()
{
  runner.execute();
}


void mqttDelayer()
{
  mqttThread.enable();
  runner.addTask(mqttThread);
}

void MQTTMessageCallback()
{
  publishMQTTPayload(mqtt, mqtt_user, mqtt_pass, StateTopic, WaterLevel);
}

void UltrasonicSensorCallback()
{
  Serial.print("Triggering distance measure...");
  unsigned int LastLevel = UltrasonicGetDistance();
  Serial.print("Received value: ");
  Serial.print(LastLevel);
  if(LastLevel > 0){
    //shifting array to right direction
    int i;
    for(i=ArrayLenght-1; i>0; i--)
    {
      LevelsArray[i] = LevelsArray[i-1];
    }
    LevelsArray[0] = LastLevel;
  }

  //calculate expectation value
  float prb = (1 / ArrayLenght);
  float sum = 0;
  for (int i = 0; i < ArrayLenght; i++)
  {
    sum += LevelsArray[i] * prb;
  }
  WaterLevel = (unsigned int)sum;
  Serial.print("Last expected value is:");
  Serial.print(WaterLevel);
}