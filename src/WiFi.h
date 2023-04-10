#include <Arduino.h>
#include "WiFiEspAT.h"

//Definition for Wi-Fi ESP_AT module
#define EspSerial       Serial2
#define _WIFI_LOGLEVEL_             1

void printWifiStatus();
void initializeWiFiShield();
void establishWiFi(const char *ssid, const char *password);
