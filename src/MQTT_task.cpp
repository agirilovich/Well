#include "MQTT_task.h"

void initializeMQTTTopic(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *Topic, const String SensorConfig)
{

  Serial.print("Testing connection to mqtt broker...");

  while (!mqtt.connect(DEVICE_BOARD_NAME, mqtt_user, mqtt_pass))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println(" connected!");

  //Publish message to AutoDiscovery topic
  mqtt.publish(String(Topic), SensorConfig, true, 0);
  
  //Gracefully close connection to MQTT broker
  mqtt.disconnect();

}


void publishMQTTPayload(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *Topic, unsigned int PayloadMessage)
{
  mqtt.connect(DEVICE_BOARD_NAME, mqtt_user, mqtt_pass);
  //mqtt.update();
  mqtt.publish(Topic, String(PayloadMessage), false, 0);
  mqtt.disconnect();
}

void publishMQTTStatus(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *Topic, bool Status)
{
  mqtt.connect(DEVICE_BOARD_NAME, mqtt_user, mqtt_pass);
  if (Status)
  {
    mqtt.publish(Topic, "Online", false, 0);
  }
  else
  {
    mqtt.publish(Topic, "Offline", false, 0);
  }
  //mqtt.update();
  mqtt.disconnect();
}