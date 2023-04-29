#include <MQTTPubSubClient.h>

void initializeMQTTTopic(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *Topic, const String SensorConfig);

void publishMQTTPayload(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *Topic, unsigned int PayloadMessage);

void publishMQTTStatus(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *Topic, bool Status);


