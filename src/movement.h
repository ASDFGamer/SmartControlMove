#ifndef MOVEMENT_H
#define MOVEMENT_H

//The minimal distance of a movement to be counted as movement
#define MIN_MOVE_LENGTH 5

void detectMove();

bool shouldDetectHold();

void detectHold();

#endif