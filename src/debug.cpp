#include <Arduino.h>
#include "debug.h"
#include "distances.h"

//This prints the given given array to serial and adds spaces around the current position.
void printArray(int arr[], int length, int curr_pos)
{
  Serial.print("[");
  for (int i = 0; i<length;i++)
  {
    if (i== curr_pos)
    {
      Serial.print(" ");
    }

    Serial.print(arr[i]);

    if (i== curr_pos )
    {
      Serial.print(" ");
    }
    if ((i+1) != length)
    {
      Serial.print(",");
    }
  }
  Serial.println("]");
}

//This outputs the current values of distances and changed to serial.
void printValues()
{
  Serial.println("Distances");
  printArray(getDistances(),MAX_COUNT,getPos());
  Serial.println("Changed");
  printArray(getChanged(),MAX_COUNT,getPos());
}