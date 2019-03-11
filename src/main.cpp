#include <Arduino.h>
#include <Ultrasonic.h>
#include "movement.h"
#include "distances.h"
#include "main.h"
#include "mqtt.h"

//The used Ultrasonic sensor
Ultrasonic ultrasonic(32,35); // (Trig PIN,Echo PIN)

//The current delay
int del = MAX_DEL;

//This outputs an scala to serial that shows the current distance to the sensor. 
//The output show the distance in cm, the scala and the change that happened during MIN_MOVE_LENGTH 
void printLevel()
{
  int dist = getCurrentDistance();
  char meter[16];
  meter[15] = 0;
  int units = dist / 4;
  for (int i = 0; i< 15 ; i++)
  {
    if (i<units)
    {
      meter[i] = 'I';
    }
    else{
      meter[i] = ' ';
    }
  }
  int sum = sumChanges(MIN_MOVE_LENGTH);
  Serial.printf("%2d %s %3d \n", dist,meter,sum);

}

//This Methode is run when the controller starts. It enables the serial connection and initiates the values.
void setup() {
  Serial.begin(9600);
  initDistances();
  initMQTT();
}


//This Methode is called indefinitly during runtime. Everything that should happen during runtime gets called from here.
void loop()
{
  //Save current value
  setDistance(ultrasonic.Ranging(CM));// CM or INC
  if (distanceChanged())
  {
    //Serial.println("Change");
    del = 50;
    if (longMove())
    {
      detectMove();
    }
  }
  else
  {
    if (del < MAX_DEL)
    {
      del = del + 30;
    }
  }
  //printLevel();
  delay(del);
}






