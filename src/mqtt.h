#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>

void initMQTT();

void publishMessage(char* topic, char* message);

uint16_t publishMessage(char* topic, char* message, int qos);
#endif