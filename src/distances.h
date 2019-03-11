#ifndef DISTANCES_H
#define DISTANCES_H

//The maximal amount of saved distances
#define MAX_COUNT 20

//The delta defines a range in which 2 distances are treated as the same distance
#define DELTA 2

//For Debugging
int* getDistances();

//For Debugging
int* getChanged();

//For Debugging
int getPos();

int getCurrentDistance();

int getLastDistance();

int getLastDistance(int last);

int getDistanceChanged();

bool distanceChanged(int last);

bool distanceChanged();

int sumChanges(int count);

void setDistance(int value);

void initDistances();
#endif