#include "MQTT_task.h"

void initializeMQTTTopic(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *StateTopic, const char* ConfigTopic, const char *ConfigMessage)
{

  Serial.print("Testing connection to mqtt broker...");

  while (!mqtt.connect("stm32_well", mqtt_user, mqtt_pass))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println(" connected!");

  //Publish message to AutoDiscovery topic
  mqtt.publish(ConfigTopic, ConfigMessage, false, 0);
  
  //Gracefully close connection to MQTT broker
  mqtt.disconnect();

}


void publishMQTTPayload(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *StateTopic, int PayloadMessage)
{
  mqtt.connect("stm32_well", mqtt_user, mqtt_pass);
  //mqtt.update();
  mqtt.publish(StateTopic, String(PayloadMessage), false, 0);
  mqtt.disconnect();
}