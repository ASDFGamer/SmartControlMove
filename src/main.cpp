#include <Arduino.h>
#include <Ultrasonic.h>

#define MAX_COUNT 20
#define MAX_DEL 500
#define MIN_MOVE_LENGTH 5

#define DELTA 2



Ultrasonic ultrasonic(32,35); // (Trig PIN,Echo PIN)

int del = MAX_DEL;

//This is the distants at that point.
int distances[MAX_COUNT];

//This saves if the distance was changed at that point.
//It uses the same points as distances.
int changed[MAX_COUNT];

int pos = 0;

int computePos(int change)
{
  return (pos + change + MAX_COUNT)%MAX_COUNT;
}

int getCurrentDistance()
{
  return distances[pos];
}

int getLastDistance()
{
  return distances[computePos(-1)];
}


int getLastDistance(int last)
{
  return distances[computePos(-last)];
}


void computeDistanceChanges()
{
  changed[pos] = getCurrentDistance() - getLastDistance();
}
void insert(int value)
{
  pos = (pos+1) % MAX_COUNT;
  distances[pos] = value;
  computeDistanceChanges();
}

bool distanceChanged(int last)
{
  return abs(changed[computePos(-last)])>DELTA;
}

bool distanceChanged()
{
  return distanceChanged(0);
}

void setup() {
  Serial.begin(9600);
  for (int i = 0 ; i< MAX_COUNT;i++)
  {
    insert(0);
  } 
}

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

void printValues()
{
  Serial.println("Distances");
  printArray(distances,MAX_COUNT,pos);
  Serial.println("Changed");
  printArray(changed,MAX_COUNT,pos);
}

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

int sumChanges(int length)
{
  int sum = 0;
  for (int i= 0; i< length; i++)
  {
    sum += changed[computePos(-i)];
  }
  return sum;
}

void detectMove()
{
  //Serial.println(sumChanges(MIN_MOVE_LENGTH));
}

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






