#include <Arduino.h>
#include "distances.h"

//This is are all saved distantces.
int distances[MAX_COUNT];

//This saves if the distance was changed at that point in time.
//It uses the same indices as distances.
int changed[MAX_COUNT];

//The current index in distances and changed
int pos = 0;

//This gets the index of the value that is 'change' messurements away from the current messurement.
//Warning: if change is greater than MAY_COUNT this methode returns indexes but the values are already deleted.
int computePos(int change)
{
  return (pos + change + MAX_COUNT)%MAX_COUNT;
}

//This returns the current distance to the sensor
int getCurrentDistance()
{
  return distances[pos];
}

//This returns the last saved distance to the sensor
int getLastDistance()
{
  return distances[computePos(-1)];
}

//This returns the distance on the mesurement that is 'last' messurements before the current.
int getLastDistance(int last)
{
  return distances[computePos(-last)];
}

//This computes if the Distance changed since the last messurement and saves the value into the 'changed' array. 
int getDistanceChanged()
{
  return getCurrentDistance() - getLastDistance();
}

//This inserts the given value at the next position into the array and calculates and inserts the changees into changed.
void setDistance(int value)
{
  pos = (pos+1) % MAX_COUNT;
  distances[pos] = value;
  changed[pos] = getDistanceChanged();
}

//This chacks if the distance changed at the given time. If it changed this returns true.
bool distanceChanged(int last)
{
  return abs(changed[computePos(-last)])>DELTA;
}

//This chacks if the distance changed in the last messurement. If it changed this returns true.
bool distanceChanged()
{
  return abs(changed[pos])>DELTA;
}

//This returns the Sum of the last 'count' changes. 
int sumChanges(int count)
{
  int sum = 0;
  for (int i= 0; i< count; i++)
  {
    sum += changed[computePos(-i)];
  }
  return sum;
}

void initDistances()
{
  for (int i = 0 ; i< MAX_COUNT;i++)
  {
    setDistance(0);
  } 
}


//-----------------------------------------------------------------------------
//Only for Debugging
int* getDistances()
{
    return distances;
}

//Only for Debugging
int* getChanged()
{
    return changed;
}

//Only for Debugging
int getPos()
{
    return pos;
}