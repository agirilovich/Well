#include <MQTTPubSubClient.h>

void initializeMQTTTopic(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *StateTopic, const char* ConfigTopic, const char *ConfigMessage);

void publishMQTTPayload(MQTTPubSubClient mqtt, const char *mqtt_user, const char *mqtt_pass, const char *StateTopic, unsigned int PayloadMessage);


