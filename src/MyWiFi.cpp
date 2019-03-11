#include <WiFi.h>
#include <AsyncMqttClient.h>
#include <WiFi_Settings.h>
#include "MyWiFi.h"

//Client for the MQTT-connection
AsyncMqttClient mqttClient;

//Timer for MQTT
TimerHandle_t mqttReconnectTimer;

//Timer for the WiFi
TimerHandle_t wifiReconnectTimer;
