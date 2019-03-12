#include "distances.h"
#include "movement.h"
#include "mqtt.h"
#include <string.h>
#include <Arduino.h>

bool enableHoldDetection = false;

//This detects if a long move happened
bool longMove()
{
  bool moved = true;
  int i = 0;
  do
  {
    i++;
    moved = distanceChanged(i);//Doesn't have to be && moved, because if moved is false it is already aborted.
  }while (moved && (i<MIN_MOVE_LENGTH));
  //Serial.print(i);
  return moved;
}

//This detects any movements
void detectMove()
{
  Serial.println("detect");
  if (getCurrentDistance()<(DELTA*2))
  {
    Serial.println("hold");
    enableHoldDetection = true;
  }
}

//This returns true, if the current distance should be processes even if there was no movement.
bool shouldDetectHold()
{
  return enableHoldDetection;
}

//This processes current distances when there was no movement but they should be processed.
void detectHold()
{
  if (getCurrentDistance()<(DELTA*2))
  {
    Serial.println("stop");
    publishMessage("smartControl/stop","1",1);
  }
  enableHoldDetection = false;
}