#ifndef MQTT_H
#define MQTT_H

void initMQTT();

#include <WString.h>
#include <stdint.h>

void publishMessage(String topic, String message);

uint16_t publishMessage(String topic, String message, int qos);
#endif