#include <Arduino.h>
#include <Ultrasonic.h>
#include <WiFi.h>
#include <AsyncMqttClient.h>

//The maximal amount of saved distances
#define MAX_COUNT 20

//The Maximal delay between to messurements
#define MAX_DEL 500

//The minimal distance of a movement to be counted as movement
#define MIN_MOVE_LENGTH 5

//The delta defines a range in which 2 distances are treated as the same distance
#define DELTA 2



//The used Ultrasonic sensor
Ultrasonic ultrasonic(32,35); // (Trig PIN,Echo PIN)

//The current delay
int del = MAX_DEL;

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
void insert(int value)
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
  printArray(distances,MAX_COUNT,pos);
  Serial.println("Changed");
  printArray(changed,MAX_COUNT,pos);
}

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

//This detects any movements
void detectMove()
{
  //Serial.println(sumChanges(MIN_MOVE_LENGTH));
}

//This outputs an scala to serial that shows the current distance to the sensor. 
//The output show the distance in cm, the scala and the change that happened during MIN_MOVE_LENGTH 
void printLevel()
{
  //clear();
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
  for (int i = 0 ; i< MAX_COUNT;i++)
  {
    insert(0);
  } 
}


//This Methode is called indefinitly during runtime. Everything that should happen during runtime gets called from here.
void loop()
{
  //Save current value
  insert(ultrasonic.Ranging(CM));// CM or INC
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
  //Serial.println(pos);
  //printValues();
  printLevel();
  delay(del);
}






