#include "distances.h"
#include "movement.h"

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
  //Serial.println(sumChanges(MIN_MOVE_LENGTH));
}