# Bluepill based device, that measure level of water in a well

Device measure levle of water in a well via ultrasonic AJ-SR04M module. Than filter data and delivery it to MQTT broker.
HomeAsstant service display level of water as a binary sensor value.

# Used modules

* BluePill STM32C06T06 mikrocontroller
* AJ-SR04M ultrasonic module, switched to "Serial Port triger" mode (Mode 4)
* DC-DC Step-Down inverter for conversation 18-25V power from solar panel to stable 5V
* DC-DC Step-Down inverter DS-MINI-360 for powering WT8266 module
* ESP-07S Wi-Fi module

![Device module schematic](well_schematic.png)